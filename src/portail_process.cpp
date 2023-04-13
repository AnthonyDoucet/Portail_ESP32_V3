#include <main.h>

//######## PROGRAM ########
void portail_process(){
/*
  gestionCycle();

  if(secteur == false && secteurOneTime == true){ //1 FOIS PAR COUPURE SECTEUR
      secteurOneTime = false;

      if(incrementCoupureSecteur == 10){
        incrementCoupureSecteur = 0;
      }
      dateDerniereCoupure[incrementCoupureSecteur++] = stringDate + " - " + stringHeure;   //Enregistrer date
      Serial.println("COUPURE SECTEUR");
      cmpCoupures++;
  }
  else if(secteur == false){  //TANT QUE LE SECTEUR N'EST PAS REVENU
    //GESTION SUR BATTERIE
    if( (nBat < SEUIL_BATTERIE) || (cmpAvantForce >= seuilAvantForce) ){    //Si N batterie en dessous du seuil, Force ouverture
      ouvreForce = true;
    }
    else if(nBat > SEUIL_BATTERIE + SEUIL_OFFSET){  //Si N batterie au dessus du seuil + Offset, Desactiver force ouverture
      ouvreForce = false;
    }

    if(cmpAvantForce >= SEUIL_AVANT_FORCE){
      ouvreForce = true;
    }
  }
  else if(secteur == true && secteurOneTime == false){  //SI RETOUR SECTEUR, Desactiver force ouverture
    
    if(incrementPresenceSecteur == 10){
      incrementPresenceSecteur = 0;
    }
    dateDernierePresence[incrementPresenceSecteur++] = stringDate + " - " + stringHeure;  //Enregistrer date
    cmpAvantForce = 0; // reset si retour du secteur
    cycle(); //Cycle pour relancer le portail
    ouvreForce = false;
    ouvreForceWEB = false;
    Serial.println("RETOUR SECTEUR");
    secteurOneTime = true;
  }
  else{ ouvreForce = false; }

  gestionWebInput();
  
  if(ouvreForce){
    if(incrementForce == 10){
      incrementForce = 0;
    }
    dateDernierForce[incrementForce++] = stringDate + " - " + stringHeure;  //Enregistrer date
  }

  digitalWrite(PIN_Force, ouvreForce);
*/
}

void cycle_process(){
/*
  if(cycleEnCours == true && cycleOneTime == true){
    cycleOneTime = false;

    if(incrementDateCycle == 10){
      incrementDateCycle = 0;
    }
    dateCycle[incrementDateCycle++] = stringDate + " - " + stringHeure;


    //Serial.println( "Cycle H:" + (String)hour() + " " + (String)horaireMatin[0] + " " + (String)horaireNuit[0] + "   M:" + minute() + " " + (String)horaireMatin[1] + " " + (String)horaireNuit[1]);
    
    bool jour = false;
    bool hMinPassed = false;
    if(hour() > horaireMatin[0]){
      //Serial.println("H > Matin");
      hMinPassed = true;
    }
    else if(hour() == horaireMatin[0]){
      //Serial.println("H == Matin");
      if(minute() >= horaireMatin[1]){
        //Serial.println("M Matin");
        hMinPassed = true;
      }
    }

    if(hMinPassed){
      if(hour() < horaireNuit[0]){
        //Serial.println("H < Nuit");
        jour = true;
      }
      else if(hour() == horaireNuit[0]){
        //Serial.println("H == Nuit");
        if(minute() < horaireNuit[1]){
          //Serial.println("M Nuit");
          jour = true;
        }
      }
    }

    //Serial.println("Jour:" + (String)jour);
    if(jour == true){
        cmpJournalierJour++;
        cmpOuverturesJour++;
        cmpAuxOuverturesJour++;
        dateDernierCycleJour = stringDate + " - " + stringHeure;
    }
    else{
        cmpJournalierNuit++;
        cmpOuverturesNuit++;
        cmpAuxOuverturesNuit++;
        dateDernierCycleNuit = stringDate + " - " + stringHeure;
    }

    if(secteur == false){
      cmpAvantForce++;
    }

  }
  else if(cycleEnCours == false && cycleOneTime == false){
    cycleOneTime = true;
  }
*/
}