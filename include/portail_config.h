#ifndef _PORTAIL_CONFIG_H_
#define _PORTAIL_CONFIG_H_

//######## IO ########
//input
#define PIN_Bouton1 36          //VP  pull-up
#define PIN_Bouton2 39          //VN  pull-up
#define PIN_Bouton3 34          //D34 pull-up
#define PIN_Bouton4 35          //D35 pull-up
#define PIN_PresenceSecteur 33  //D33 pull-up
#define PIN_Cycle 25            //D32 pull-up

//Output
#define PIN_Force 26
#define PIN_Ouvre 27
#define PIN_LED_BUILDIN 5
#define PIN_DEBUG 27

//######## LCD ########
#define LCD_ENABLED true
#define LCD_ADDR 0x27               //Adresse I2C #SDA pin 21  #SCL pin 22
#define LCD_WIDTH 16                //Nombre de pixels en largeur
#define LCD_HEIGHT 2                //Nombre de pixels en hauteur
#define LCD_REFRESH_RATE 500        //Delais entre chaque mise a jour de l'ecran
#define LCD_BACKLIGHT_TIMEOUT 60    //Delais de mise en veille de l'ecran LCD (en secondes) si aucune interaction
#define LCD_MENU_TAILLE 4           //Nombre de Menu
#define LCD_START_DELAY 3000        //Temps d'affichage des infos sur LCD au demarage (en ms)

//######## NETWORK ########
//Ethernet
#define ETH_SPI_HOST HSPI_HOST
#define SPI_CLOCK_MHZ 12

#define SPI_MISO_GPIO 19
#define SPI_MOSI_GPIO 23
#define SPI_SCLK_GPIO 18
#define SPI_CS0_GPIO 5

#define SPI_INT0_GPIO 4
#define SPI_PHY_RST0_GPIO -1
#define SPI_PHY_ADDR0 1

//WiFi
#define ENABLE_SOFT_AP true         //Point d'acces WiFi
#define AP_SSID "ESP_PORTAIL"
#define AP_PASSWORD "admin#esp32"
#define AP_HIDDEN false

//Website
#define HTTP_PORT 80                //Port HTTP
#define WEB_ENABLE_LOGIN false
#define WEB_USERNAME "user"         //Nom d'utilisateur SITE
#define WEB_PASSWORD "1234"         //Mot de passe SITE
#define WEB_ADMIN_USERNAME "admin"  //Nom d'utilisateur ADMINISTRATEUR SITE
#define WEB_ADMIN_PASSWORD "admin"  //Mot de passe ADMINISTRATEUR SITE

//######## MISC ########
#define SHOW_DEBUG true             //Affichage debug
#define UART_BAUDRATE 115200        //Vitesse de connexion Serie

//######## DEFAULT ########
#define HEURE_JOUR_DEBUT 6          //Heure debut de journée
#define HEURE_JOUR_FIN 21           //Heure fin de journée

#define DEFAULT_SEUIL_AVANT_FORCE 10 //Nombre d'ouvertures sur batterie avant ouverture forcée 
#define DEFAULT_SEUIL_BATTERIE 24.8  //Valeur en Volt
#define DEFAULT_SEUIL_OFFSET 0.5     //Pour eviter les clignotements

#endif