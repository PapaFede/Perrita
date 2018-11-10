//Federico Bustamante (FBustamante)
//4 Diciembre 2017
//Programa 'Perrito'

#include <Arduboy2.h>
#include <EEPROM.h>
Arduboy2 arduboy;
BeepPin1 beep1; // class instance for speaker pin 1
BeepPin2 beep2; // class instance for speaker pin 1
//#include "Sprites.h"
//Sprites sprites;
#include "Font3x5.h" //Version altamente recortada
Font3x5 font3x5 = Font3x5();
#include "global.h"
#include "Graficos.h"
#include "Varios.h";
#include "Maps.h"
#include "Textos.h"
#include "ObjetosMapas.h"
#include "AranaGrande.h"
#include "Player.h";
#include "Niveles.h";
#include "Menu.h";

//**************************************************************************************************
void setup() {
    arduboy.begin();
//	Serial.begin(9600);	
	beep1.begin(); // set up the hardware for playing tones
	beep2.begin(); // set up the hardware for playing tones
	if (EEPROM.read(EEPROM_AUDIO_ON_OFF))bSoundOnOff=1;
	else bSoundOnOff=0;
	bIdioma=EEPROM.read(EEPROM_STORAGE_SPACE_START);
	if(bIdioma>1)bIdioma=0;
}
//**************************************************************************************************
void loop() {
	
	if (!(arduboy.nextFrame())) return;
    arduboy.clear();
	beep1.timer(); // handle tone duration
	beep2.timer(); // handle tone duration
	TocaSonido();
	if(bMapaEnCurso==0)Menu();else Nivel();
//	Serial.write(arduboy.getBuffer(), 128 * 64 / 8);	
    arduboy.display();
} 
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************

