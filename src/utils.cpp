#include <main.h>

//######## IO ########
void initIO(){
    DEBUGLN("initIO");
    //Input
    pinMode(PIN_Bouton1, INPUT);
    pinMode(PIN_Bouton2, INPUT);
    pinMode(PIN_Bouton3, INPUT);
    pinMode(PIN_Bouton4, INPUT);
    pinMode(PIN_PresenceSecteur, INPUT);
    pinMode(PIN_Cycle, INPUT);

    //Output
    pinMode(PIN_LED_BUILDIN, OUTPUT);
    pinMode(PIN_Force, OUTPUT);
    pinMode(PIN_Ouvre, OUTPUT);
    pinMode(PIN_DEBUG, OUTPUT);

    // ADS1115
    ADS.begin();         // Initialisation du module ADS1115
    ADS.setGain(0);      // On prend le gain le plus bas (index 0), pour avoir la plus grande plage de mesure (6.144 volt)
    ADS.setMode(0);      // On indique à l'ADC qu'on fera des mesures à la demande, et non en continu (0 = CONTINUOUS, 1 = SINGLE)
    ADS.setDataRate(4);  // On spécifie la vitesse de mesure de tension qu'on souhaite, allant de 0 à 7 (7 étant le plus rapide, soit 860 échantillons par seconde)
    ADS.readADC(0);      // Et on fait une lecture à vide, pour envoyer tous ces paramètres
}

uint32_t previousDebounce[4] = {0};
bool oneTimeDebounce[4] = {true};
void debounceBtn(){
  //Btn1
  if(state_pin_btn1 == true && oneTimeDebounce[0] == true){
    oneTimeDebounce[0] = false;
    last_time_btn_pressed = millis();
    LCD_oneTime = true;
  }
  else if(state_pin_btn1 == false && oneTimeDebounce[0] == false){
    oneTimeDebounce[0] = true;
  }

  //Btn2
  if(state_pin_btn2 == true && oneTimeDebounce[1] == true){
    oneTimeDebounce[1] = false;
    if(LCD_menu_pos == 0){
      LCD_menu_pos = LCD_MENU_TAILLE;
    }
    else{
      LCD_menu_pos--;
    }
    last_time_btn_pressed = millis();
  }
  else if(state_pin_btn2 == false && oneTimeDebounce[1] == false){
    oneTimeDebounce[1] = true;
  }

  //Btn3
  if(state_pin_btn3 == true && oneTimeDebounce[2] == true){
    oneTimeDebounce[2] = false;
    if(LCD_menu_pos == LCD_MENU_TAILLE){
      LCD_menu_pos = 0;
    }
    else{
      LCD_menu_pos++;
    }
    LCD_oneTime = true;
    last_time_btn_pressed = millis();
  }
  else if(state_pin_btn3 == false && oneTimeDebounce[2] == false){
    oneTimeDebounce[2] = true;
  }

  //Btn4
  if(state_pin_btn4 == true && oneTimeDebounce[3] == true){
    oneTimeDebounce[3] = false;
    last_time_btn_pressed = millis();
  }
  else if(state_pin_btn4 == false && oneTimeDebounce[3] == false){
    oneTimeDebounce[3] = true;
  }
}

void readInputs(){
  state_pin_btn1 = !digitalRead(PIN_Bouton1);
  state_pin_btn2 = !digitalRead(PIN_Bouton2);
  state_pin_btn3 = !digitalRead(PIN_Bouton3);
  state_pin_btn4 = !digitalRead(PIN_Bouton4);

  secteur = !digitalRead(PIN_PresenceSecteur);
  cycle_en_cours = !digitalRead(PIN_Cycle);

  debounceBtn();
}

void readADC(){
  nBat = ADS.readADC(0);           // Mesure de tension de la broche A0, par rapport à la masse
  //vBat = ADS.toVoltage(nBat);
  vBat = float(nBat*1.0988)/1000;
}

void writeOutputs(){
  digitalWrite(PIN_Force, ouvre_force);
  //digitalWrite(PIN_Ouvre, ouvre_normal);
}

void blink(int pin, int time){
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
  delay(time);
  digitalWrite(pin, LOW);
}

//######## LCD ########
#if LCD_ENABLED
void initLCD(){
  PRINTLN("init LCD");
  lcd.init();
  lcd.backlight();
  
  lcdClear(2);
}

void lcdPrint(uint8_t line, uint8_t pos, String str){
    if(pos < LCD_WIDTH && line < LCD_HEIGHT){
        lcd.setCursor(pos, line);
        lcd.print(str);
    }
}

void lcdPrint(uint8_t line, String str){
    if(line < LCD_HEIGHT){
        lcd.setCursor(0, line);
        lcd.print(str);
    }
}

void lcdClear(uint8_t line){
    if(line == 2){
        lcdPrint(0,"                ");
        lcdPrint(1,"                ");
    }
    else{
        lcdPrint(line,"                ");
    }
}

void lcdMenu(){

  if(LCD_menu_pos <= LCD_MENU_TAILLE){

    if(LCD_oneTime){
      LCD_oneTime = false;
      lcdClear(2);
    }

    switch(LCD_menu_pos){
        case 0:
            lcdPrint(0, getRTCDateStr() + "         ");
            lcdPrint(1, getRTCTimeStr() + "         ");
            break;
        case 1:
            lcdPrint(0,"Compteur:       ");
            lcdPrint(1,"Jour:"+ String(cmp_journalier_jour) + " Nuit:"+ String(cmp_journalier_nuit) + "     ");
            break;
        case 2:
            lcdPrint(0,"Tension Batterie");
            lcdPrint(1,String(vBat) + "V              ");
            break;
        case 3:
            lcdPrint(0,"Uptime:       ");
            lcdPrint(1,String(millis()/1000) + "             ");
            break;
        case 4:
            lcdPrint(0,"Ethernet:     ");
            lcdPrint(1, ethernet_status  + "             ");
            break;

        default:
            lcdPrint(0,"Menu:"+String(LCD_menu_pos));
            break;
    }
  }


  if(millis() >= last_time_btn_pressed + (LCD_BACKLIGHT_TIMEOUT*1000)){
    lcd.noBacklight();
  }
  else{
    lcd.backlight();
  }
}

#endif

//######## RTC ########
void initRTC(){
  if (!DS1307_RTC.begin()) {
    PRINTLN("Couldn't find RTC");
    return;
  }
  
  if (! DS1307_RTC.isrunning()) {
    DEBUGLN("RTC is NOT running, let's set the time!");
    DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  PRINTLN("RTC Started");
}

void setRTCunixtime(uint32_t epoch){
  if(!DS1307_RTC.isrunning()){
    return;
  }
  DateTime dt(epoch);
  DS1307_RTC.adjust(dt);
}

String getRTCDateStr(){
  if(!DS1307_RTC.isrunning()){
    return "RTC Not running";
  }
  return String(rtc_now.year()) + "/" + to2digit(rtc_now.month()) + "/" + to2digit(rtc_now.day());
}

String getRTCTimeStr(){
  if(!DS1307_RTC.isrunning()){
    return "RTC Not running";
  }
  return to2digit(rtc_now.hour()) + ":" + to2digit(rtc_now.minute()) + ":" + to2digit(rtc_now.second());
}

void updateTime(){
  rtc_now = DS1307_RTC.now();
}

String to2digit(uint8_t nombre){
  String resultat;
  if(nombre < 10)
    resultat = "0";
  return resultat += String(nombre,DEC);  
}

//######## EEPROM ########
void initEEPROM(){
  PRINTLN("Init read EEPROM");
  eeprom.begin("data", false); //Read/Write
  reboot_counter = eeprom.getUShort("reboot", 0);
  reboot_counter++;
  eeprom.putUShort("reboot", reboot_counter);
  eeprom.end();

  //writeEEPROM_BRUTE();
  readEEPROM();
}

void readEEPROM(){
  PRINTLN("Read EEPROM");
  eeprom.begin("data", true); //Read only
  saved_uptime = eeprom.getUInt("saved_uptime");

  heure_hiver    = eeprom.getBool("heure_hiver");
  horaireMatin_h = eeprom.getUChar("Matin_h");
  horaireMatin_m = eeprom.getUChar("Matin_m");
  horaireNuit_h  = eeprom.getUChar("Nuit_h");
  horaireNuit_m  = eeprom.getUChar("Nuit_m");
  //COMPTEUR AVANT FORCE
  seuil_avant_force  = eeprom.getUShort("force");
  //COMPTEURS
  cmp_ouvertures_jour = eeprom.getUShort("cmp_o_j");
  cmp_ouvertures_nuit = eeprom.getUShort("cmp_o_n");
  //COMPTEURS JOURNALIER
  cmp_journalier_jour = eeprom.getUShort("cmp_j_j");
  cmp_journalier_nuit = eeprom.getUShort("cmp_j_n");
  //COMPTEURS AUX
  cmp_aux_ouvertures_jour = eeprom.getUShort("cmp_o_a_j");
  cmp_aux_ouvertures_nuit = eeprom.getUShort("cmp_o_a_n");
  //COMPTEUR COUPURES SECTEUR!
  cmp_coupures = eeprom.getUShort("cmp_coupures");

  for(int i=0 ; i < 20 ; i++){
    date_cycle[i] = eeprom.getString("d_cycle_" + static_cast<char>(i));
  }

  for(int i=0 ; i < 10 ; i++){
    date_derniere_coupure[i] = eeprom.getString("d_coupure_" + static_cast<char>(i));
  }

  for(int i=0 ; i < 10 ; i++){
    date_derniere_presence[i] = eeprom.getString("d_presence_" + static_cast<char>(i));
  }

  for(int i=0 ; i < 10 ; i++){
    date_dernier_force[i] = eeprom.getString("d_force_" + static_cast<char>(i));
  }

  eeprom.end();
}

void writeEEPROM_RESET(){
  DEBUGLN("Write EEPROM RESET");
  eeprom.begin("data", false); //Read/Write

  eeprom.putULong("saved_uptime",millis()/1000);
  eeprom.putBool("heure_hiver",false);
  eeprom.putUChar("Matin_h",HEURE_JOUR_DEBUT);
  eeprom.putUChar("Matin_m",0);
  eeprom.putUChar("Nuit_h",HEURE_JOUR_FIN);
  eeprom.putUChar("Nuit_m",0);

  eeprom.putUShort("force", DEFAULT_SEUIL_AVANT_FORCE);
  eeprom.putUShort("cmp_o_j",0);
  eeprom.putUShort("cmp_o_n",0);
  eeprom.putUShort("cmp_j_j",0);
  eeprom.putUShort("cmp_j_n",0);
  eeprom.putUShort("cmp_o_a_j",0);
  eeprom.putUShort("cmp_o_a_n",0);
  eeprom.putUShort("cmp_coupures",0);

  for(int i=0 ; i < 20 ; i++){
    eeprom.putString("d_cycle_"+static_cast<char>(i), "Aucune");
  }

  for(int i=0 ; i < 10 ; i++){
    eeprom.putString("d_coupure_"+static_cast<char>(i), "Aucune");
  }

  for(int i=0 ; i < 10 ; i++){
    eeprom.putString("d_presence_"+static_cast<char>(i), "Aucune");
  }

  for(int i=0 ; i < 10 ; i++){
    eeprom.putString("d_force_"+static_cast<char>(i), "Aucune");
  }

  eeprom.end();
}

void writeEEPROM(){
  DEBUGLN("Write EEPROM");
  eeprom.begin("data", false); //Read/Write
  eeprom.putULong("saved_uptime",millis()/1000);

  if(heure_hiver    != eeprom.getBool("heure_hiver"))   { eeprom.putBool("heure_hiver",heure_hiver); }
  if(horaireMatin_h != eeprom.getUChar("Matin_h"))     { eeprom.putUChar("Matin_h",horaireMatin_h); }
  if(horaireMatin_m != eeprom.getUChar("Matin_m"))     { eeprom.putUChar("Matin_m",horaireMatin_m); }
  if(horaireNuit_h  != eeprom.getUChar("Nuit_h"))      { eeprom.putUChar("Nuit_h",horaireNuit_h); }
  if(horaireNuit_m  != eeprom.getUChar("Nuit_m"))      { eeprom.putUChar("Nuit_m",horaireNuit_m); }
  //COMPTEUR AVANT FORCE
  if(seuil_avant_force  != eeprom.getUShort("force"))  { eeprom.putUShort("force",seuil_avant_force); }
  //COMPTEURS
  if(cmp_ouvertures_jour != eeprom.getUShort("cmp_o_j"))   { eeprom.putUShort("cmp_o_j",cmp_ouvertures_jour); }
  if(cmp_ouvertures_nuit != eeprom.getUShort("cmp_o_n"))   { eeprom.putUShort("cmp_o_n",cmp_ouvertures_nuit); }
  //COMPTEURS JOURNALIER
  if(cmp_journalier_jour != eeprom.getUShort("cmp_j_j"))   { eeprom.putUShort("cmp_j_j",cmp_journalier_jour); }
  if(cmp_journalier_nuit != eeprom.getUShort("cmp_j_n"))   { eeprom.putUShort("cmp_j_n",cmp_journalier_nuit); }
  //COMPTEURS AUX
  if(cmp_aux_ouvertures_jour != eeprom.getUShort("cmp_o_a_j"))   { eeprom.putUShort("cmp_o_a_j",cmp_aux_ouvertures_jour); }
  if(cmp_aux_ouvertures_nuit != eeprom.getUShort("cmp_o_a_n"))   { eeprom.putUShort("cmp_o_a_n",cmp_aux_ouvertures_nuit); }
  //COMPTEUR COUPURES SECTEUR!
  if(cmp_coupures != eeprom.getUShort("cmp_coupures"))   { eeprom.putUShort("cmp_coupures",cmp_coupures); }
  

  for(int i=0 ; i < 20 ; i++){
    if(date_cycle[i] != eeprom.getString("d_cycle_"  + static_cast<char>(i))){
      eeprom.putString("d_cycle_"+static_cast<char>(i), date_cycle[i]);
    };
  }
  
  for(int i=0 ; i < 10 ; i++){
    if(date_derniere_coupure[i] != eeprom.getString("d_coupure_"  + static_cast<char>(i))){
      eeprom.putString("d_coupure_"+static_cast<char>(i), date_derniere_coupure[i]);
    };
  }

  for(int i=0 ; i < 10 ; i++){
    if(date_derniere_presence[i] != eeprom.getString("d_presence_"  + static_cast<char>(i))){
      eeprom.putString("d_presence_"+static_cast<char>(i), date_derniere_presence[i]);
    };
  }

  for(int i=0 ; i < 10 ; i++){
    if(date_dernier_force[i] != eeprom.getString("d_force_"  + static_cast<char>(i))){
      eeprom.putString("d_force_"+static_cast<char>(i), date_dernier_force[i]);
    };
  }

  eeprom.end();
}