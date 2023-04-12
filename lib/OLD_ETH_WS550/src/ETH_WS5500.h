/*
 ETH.h - espre ETH PHY support.
 Based on WiFi.h from Ardiono WiFi shield library.
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

#ifndef _ETH_WS5500_H_
#define _ETH_WS5500_H_

#include "WiFi.h"
#include "esp_system.h"
#include "esp_eth.h"

#include "esp_netif.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/spi_common_internal.h"
#include "driver/spi_master.h"

typedef struct {
    uint8_t spi_cs_gpio;
    uint8_t int_gpio;
    int8_t phy_reset_gpio;
    uint8_t phy_addr;
}spi_eth_module_config_t;

#define SPI_HOST_DEFINED HSPI_HOST
#define SPI_CLOCK_MHZ 12

#define SPI_MISO_GPIO 19
#define SPI_MOSI_GPIO 23
#define SPI_SCLK_GPIO 18

#define SPI_CS0_GPIO 5
#define SPI_INT0_GPIO 4
#define SPI_PHY_RST0_GPIO -1
#define SPI_PHY_ADDR0 1

class ETHClass {
    private:
        esp_eth_handle_t eth_handle = NULL;
        static esp_netif_ip_info_t *ip_info;
        static IPAddress ip, gateway, netmask;

        static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
        {
            uint8_t mac_addr[6] = {0};
            esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;
            switch (event_id) {
            case ETHERNET_EVENT_CONNECTED:
                esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
                log_d("Ethernet Link Up");
                log_d("Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
                break;
            case ETHERNET_EVENT_DISCONNECTED:
                log_d("Ethernet Link Down");
                break;
            case ETHERNET_EVENT_START:
                log_d("Ethernet Started");
                break;
            case ETHERNET_EVENT_STOP:
                log_d("Ethernet Stopped");
                break;
            default:
                break;
            }
        }
        static void got_ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
        {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
            ip_info = &event->ip_info;
            ip = IPAddress(ip_info->ip.addr);
            gateway = IPAddress(ip_info->gw.addr);
            netmask = IPAddress(ip_info->netmask.addr);

            log_d("netif_ETH_IP:" IPSTR, IP2STR(&ip_info->ip));
            log_d("netif_ETH_GATEWAY:" IPSTR, IP2STR(&ip_info->gw));
            log_d("netif_ETH_NETMASK:" IPSTR, IP2STR(&ip_info->netmask));
        }

    public:
        ETHClass();
        ~ETHClass();

        void begin_ws5500(uint8_t *mac);
        
        const char * getHostname();
        bool setHostname(const char * hostname);

        bool fullDuplex();
        bool linkUp();
        uint8_t linkSpeed();

        IPAddress localIP(){ return ip; };
        IPAddress subnetMask(){ return netmask; };
        IPAddress gatewayIP(){ return gateway; };

        IPAddress dnsIP(uint8_t dns_no = 0);

        IPAddress broadcastIP();
        IPAddress networkID();
        uint8_t subnetCIDR();

        uint8_t * macAddress(uint8_t* mac);
        String macAddress();

        friend class WiFiClient;
        friend class WiFiServer;
};

extern ETHClass ETH;

#endif /* _ETH_WS5500_H_ */
