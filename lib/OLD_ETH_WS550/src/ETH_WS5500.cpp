/*
 ETH.h - espre ETH PHY support.
 Based on WiFi.h from Arduino WiFi shield library.
 Copyright (c) 2011-2014 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "ETH_WS5500.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_eth.h"
#include "esp_eth_phy.h"
#include "esp_eth_mac.h"
#include "esp_eth_com.h"
#include "soc/emac_ext_struct.h"
#include "soc/rtc.h"
//#include "soc/io_mux_reg.h"
//#include "hal/gpio_hal.h"
#include "lwip/err.h"
#include "lwip/dns.h"

extern void tcpipInit();
extern void add_esp_interface_netif(esp_interface_t interface, esp_netif_t* esp_netif); /* from WiFiGeneric */
esp_netif_ip_info_t *ETHClass::ip_info = nullptr;
IPAddress ETHClass::ip, ETHClass::gateway, ETHClass::netmask;

ETHClass::ETHClass(){}
ETHClass::~ETHClass(){}

void ETHClass::begin_ws5500(uint8_t *mac){
    // Initialize TCP/IP network interface (should be called only once in application)
    tcpipInit();

    // Create default event loop that running in background
    tcpip_adapter_set_default_eth_handlers();

    // Create instance(s) of esp-netif for SPI Ethernet(s)
    esp_netif_inherent_config_t esp_netif_config = ESP_NETIF_INHERENT_DEFAULT_ETH();
    esp_netif_config_t cfg = {
        .base = &esp_netif_config,
        .stack = ESP_NETIF_NETSTACK_DEFAULT_ETH
    };
    esp_netif_config.if_key = "ETH_SPI_0";
    esp_netif_config.if_desc = "eth0";
    esp_netif_config.route_prio = 30;
    esp_netif_t *eth_netif_spi = esp_netif_new(&cfg);

    // Init MAC and PHY configs to default
    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();

    // Install GPIO ISR handler to be able to service SPI Eth modlues interrupts
    gpio_install_isr_service(0);

    // Init SPI bus
    spi_device_handle_t spi_handle = NULL;
    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MOSI_GPIO,
        .miso_io_num = SPI_MISO_GPIO,
        .sclk_io_num = SPI_SCLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI_HOST_DEFINED, &buscfg, SPI_DMA_CH_AUTO));

    // Init specific SPI Ethernet module configuration from Kconfig (CS GPIO, Interrupt GPIO, etc.)
    spi_eth_module_config_t spi_eth_module_config;
    spi_eth_module_config.spi_cs_gpio = SPI_CS0_GPIO;
    spi_eth_module_config.int_gpio = SPI_INT0_GPIO;
    spi_eth_module_config.phy_reset_gpio = SPI_PHY_RST0_GPIO;
    spi_eth_module_config.phy_addr = SPI_PHY_ADDR0;

    // Configure SPI interface and Ethernet driver for specific SPI module
    esp_eth_mac_t *mac_spi;
    esp_eth_phy_t *phy_spi;

    spi_device_interface_config_t devcfg = {
        .command_bits = 16, // Actually it's the address phase in W5500 SPI frame
        .address_bits = 8,  // Actually it's the control phase in W5500 SPI frame
        .mode = 0,
        .clock_speed_hz = SPI_CLOCK_MHZ * 1000 * 1000,
        .queue_size = 20
    };

    // Set SPI module Chip Select GPIO w5500 ethernet driver is based on spi driver
    devcfg.spics_io_num = spi_eth_module_config.spi_cs_gpio;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI_HOST_DEFINED, &devcfg, &spi_handle));
    eth_w5500_config_t w5500_config = ETH_W5500_DEFAULT_CONFIG(spi_handle);

    // Set remaining GPIO numbers and configuration used by the SPI module
    w5500_config.int_gpio_num = spi_eth_module_config.int_gpio;
    phy_config.phy_addr = spi_eth_module_config.phy_addr;
    phy_config.reset_gpio_num = spi_eth_module_config.phy_reset_gpio;

    mac_spi = esp_eth_mac_new_w5500(&w5500_config, &mac_config);
    phy_spi = esp_eth_phy_new_w5500(&phy_config);

    eth_handle = NULL;
    esp_eth_config_t eth_config_spi = ETH_DEFAULT_CONFIG(mac_spi, phy_spi);
    if(esp_eth_driver_install(&eth_config_spi, &eth_handle) != ESP_OK || eth_handle == NULL){
        log_e("esp_eth_driver_install failed");
        return;
    }
    
    // Set MAC adress
    ESP_ERROR_CHECK(esp_eth_ioctl(eth_handle, ETH_CMD_S_MAC_ADDR, mac));

    // Attach Ethernet driver to TCP/IP stack
    if(esp_netif_attach(eth_netif_spi, esp_eth_new_netif_glue(eth_handle)) != ESP_OK){
        log_e("esp_netif_attach failed");
        return;
    }

    // Attach to WiFiGeneric to receive events
    add_esp_interface_netif(ESP_IF_ETH, eth_netif_spi);

    //ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));

    if(esp_eth_start(eth_handle) != ESP_OK){
        log_e("esp_eth_start failed");
        return;
    }

    delay(50);
}

IPAddress ETHClass::dnsIP(uint8_t dns_no)
{
    const ip_addr_t * dns_ip = dns_getserver(dns_no);
    return IPAddress(dns_ip->u_addr.ip4.addr);
}

IPAddress ETHClass::broadcastIP(){ return WiFiGenericClass::calculateBroadcast(IPAddress(ip_info->gw.addr), IPAddress(ip_info->netmask.addr)); }

IPAddress ETHClass::networkID(){ return WiFiGenericClass::calculateNetworkID(IPAddress(ip_info->gw.addr), IPAddress(ip_info->netmask.addr)); }

uint8_t ETHClass::subnetCIDR(){ return WiFiGenericClass::calculateSubnetCIDR(IPAddress(ip_info->netmask.addr)); }

const char * ETHClass::getHostname()
{
    const char * hostname;
    if(tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_ETH, &hostname)){
        return NULL;
    }
    return hostname;
}

bool ETHClass::setHostname(const char * hostname){ return tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_ETH, hostname) == 0; }

bool ETHClass::fullDuplex()
{
    eth_duplex_t link_duplex;
    esp_eth_ioctl(eth_handle, ETH_CMD_G_DUPLEX_MODE, &link_duplex);
    return (link_duplex == ETH_DUPLEX_FULL);
}

bool ETHClass::linkUp(){ return WiFiGenericClass::getStatusBits() & ETH_CONNECTED_BIT; }

uint8_t ETHClass::linkSpeed()
{
    eth_speed_t link_speed;
    esp_eth_ioctl(eth_handle, ETH_CMD_G_SPEED, &link_speed);
    return (link_speed == ETH_SPEED_10M)?10:100;
}

uint8_t * ETHClass::macAddress(uint8_t* mac)
{
    if(!mac){
        return NULL;
    }
    esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac);
    return mac;
}

String ETHClass::macAddress(void)
{
    uint8_t mac[6] = {0,0,0,0,0,0};
    char macStr[18] = { 0 };
    macAddress(mac);
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

ETHClass ETH;
