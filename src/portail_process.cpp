#include <main.h>

//######## PROGRAM ########
void portail_process(){

  //cycle_en_cours = state_pin_cycle; //Status du cycle definie par etat de la broche (amelioration ?)
  //secteur = state_pin_secteur; //

  if(rtc_now.day() != previous_day){  //Si nouvelle journée reset des compteurs journaliers
    previous_day = rtc_now.day();
    cmp_journalier_jour = 0;
    cmp_journalier_nuit = 0;
  }

  if(cycle_en_cours == true && cycleOneTime == true){ //1 Fois par cycle
    cycleOneTime = false;
    cycle_process();
  }
  else if(cycle_en_cours == false && cycleOneTime == false){ //Attente d'un front descendant
    cycleOneTime = true;
  }


  if(secteur == false && secteurOneTime == true){         //1 FOIS PAR COUPURE SECTEUR
      secteurOneTime = false;

      //Enregistrement de la date de coupure
      if(date_derniere_coupure_increment == 10){
        date_derniere_coupure_increment = 0;
      }
      date_derniere_coupure[date_derniere_coupure_increment++] = getRTCDateStr() + " - " + getRTCTimeStr();

      //Ajout au compteur de coupures
      cmp_coupures++;
  }
  else if(secteur == false && secteurOneTime == false){   //TANT QUE LE SECTEUR N'EST PAS REVENU
    //Gestion de la batterie
    if( (vBat < DEFAULT_SEUIL_BATTERIE) || (cmp_avant_force >= seuil_avant_force) ){  //Si N batterie en dessous du seuil OU seuil atteint -> Force ouverture
      ouvre_force = true;
    }
    else if(vBat > DEFAULT_SEUIL_BATTERIE + DEFAULT_SEUIL_OFFSET){  //Si N batterie au dessus du (seuil + Offset), Desactiver force ouverture
      ouvre_force = false;
    }
  }
  else if(secteur == true && secteurOneTime == false){    //SI RETOUR SECTEUR, Desactiver force ouverture

    //Enregistrer date du retour secteur
    if(date_derniere_presence_increment == 10){
      date_derniere_presence_increment = 0;
    }
    date_derniere_presence[date_derniere_presence_increment++] = getRTCDateStr() + " - " + getRTCTimeStr();

    cmp_avant_force = 0;    // Reset si retour du secteur
    blink(PIN_Ouvre, 200);  // Cycle pour relancer le portail
    ouvre_force = false;    //Desactiver l'ouverture forcée car retour secteur = fonctionnement normal

    PRINTLN("RETOUR SECTEUR");
    secteurOneTime = true;
  }
  
  //Si ouverture forcée, enregistrement de la date
  if(ouvre_force == true && forceOneTime == false){
    forceOneTime = true;
    if(date_dernier_force_increment == 10){
      date_dernier_force_increment = 0;
    }
    date_dernier_force[date_dernier_force_increment++] = getRTCDateStr() + " - " + getRTCTimeStr();  //Enregistrer date
  }
  else if(ouvre_force == false && forceOneTime == true){
    forceOneTime = false;
  }

}

void cycle_process(){

  //Enregistrement de la date du cycle
  if(date_cycle_increment == 20){
    date_cycle_increment = 0;
  }
  date_cycle[date_cycle_increment++] = getRTCDateStr() + " - " + getRTCTimeStr();


  bool jour = false;
  bool hMinPassed = false;

  //Heure dans la plage horaire ?
  if(rtc_now.hour() > horaireMatin_h){
    hMinPassed = true;
  }
  else if(rtc_now.hour() == horaireMatin_h){
    if(rtc_now.minute() >= horaireMatin_m){
      hMinPassed = true;
    }
  }

  //Jour ou nuit
  if(hMinPassed){
    if(rtc_now.hour() < horaireNuit_h){
      jour = true;
    }
    else if(rtc_now.hour() == horaireNuit_h){
      if(rtc_now.minute() < horaireNuit_m){
        jour = true;
      }
    }
  }

  //Mise a jour des compteurs
  if(jour){
      cmp_journalier_jour++;
      cmp_ouvertures_jour++;
      cmp_aux_ouvertures_jour++;
      date_dernier_cycle_jour = getRTCDateStr() + " - " + getRTCTimeStr();
  }
  else{
      cmp_journalier_nuit++;
      cmp_ouvertures_nuit++;
      cmp_aux_ouvertures_nuit++;
      date_dernier_cycle_nuit = getRTCDateStr() + " - " + getRTCTimeStr();
  }

  //S'il n'y a pas de secteur
  if(secteur == false){
    cmp_avant_force++;
  }
}
