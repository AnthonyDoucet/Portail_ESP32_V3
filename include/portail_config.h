#ifndef _PORTAIL_CONFIG_H_
#define _PORTAIL_CONFIG_H_

//######## IO ########              //Broches entrees / sorties
#define PIN_Bouton1 36         
#define PIN_Bouton2 39         
#define PIN_Bouton3 34  
#define PIN_Bouton4 35
#define PIN_vBat 32
#define PIN_PresenceSecteur 33
#define PIN_Cycle  25

#define PIN_Force 26
#define PIN_Ouvre 27
#define PIN_LED_BUILDIN 5
#define PIN_DEBUG 16

//######## LCD ########
#define LCD_ENABLED true
#define LCD_ADDR 0x27               //Adresse I2C #SDA pin 21  #SCL pin 22
#define LCD_WIDTH 16                //Nombre de pixels en largeur
#define LCD_HEIGHT 2                //Nombre de pixels en hauteur
#define LCD_REFRESH_RATE 500        //Delais entre chaque mise a jour de l'ecran
#define LCD_BACKLIGHT_TIMEOUT 60    //Delais de mise en veille de l'ecran LCD (en secondes) si aucune interaction
#define LCD_MENU_TAILLE 8           //Nombre de Menu
#define LCD_START_DELAY 3000        //Temps d'affichage des infos sur LCD au demarage (en ms)

//######## NETWORK ########
#define ETH_SPI_HOST HSPI_HOST
#define SPI_CLOCK_MHZ 12

#define SPI_MISO_GPIO 19
#define SPI_MOSI_GPIO 23
#define SPI_SCLK_GPIO 18
#define SPI_CS0_GPIO 5

#define SPI_INT0_GPIO 4
#define SPI_PHY_RST0_GPIO -1
#define SPI_PHY_ADDR0 1

#define MAC_ADDRESS { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }  //Adresse MAC ETHERNET
#define HTTP_PORT 80                //Port HTTP

#define WEB_ENABLE_LOGIN false
#define WEB_USERNAME "user"         //Nom d'utilisateur SITE
#define WEB_PASSWORD "1234"         //Mot de passe SITE
#define WEB_ADMIN_USERNAME "admin"  //Nom d'utilisateur ADMINISTRATEUR SITE
#define WEB_ADMIN_PASSWORD "admin"  //Mot de passe ADMINISTRATEUR SITE

//######## MISC ########
#define SHOW_DEBUG true             //Affichage debug
#define UART_BAUDRATE 115200        //Vitesse de connexion Serie

#define HEURE_HIVER false           //Heure actuelle = heure d'hiver ?
#define HEURE_JOUR_DEBUT 6          //Heure debut de journée
#define HEURE_JOUR_FIN 21           //Heure fin de journée

#define DEFAULT_SEUIL_AVANT_FORCE 10 //Nombre d'ouvertures sur batterie avant ouverture forcée 
#define DEFAULT_SEUIL_BATTERIE 3000  //Valeur en N,  N = V * (Vmax/1023)
#define DEFAULT_SEUIL_OFFSET 750     //Pour eviter les clignotements

/*#######################
    Bat / Pin     /  N
    28    3.08    = 1009
    26    2.94    = 960
    25    2.86    = 934    
    24    2.77    = 907
    23    2.68    = 880
    22    2.59    = 846
  #######################*/

#endif