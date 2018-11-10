//**************************************************************************************************
//Los caracteres originales de la ArduBoy son de 5x7.
const char PROGMEM TextoLenguaje00[]="  START  ";
const char PROGMEM TextoLenguaje01[]="LANGUAGE ";
const char PROGMEM TextoLenguaje02[]=" SOUND   ";
const char PROGMEM TextoLenguaje10[]=" INICIO  ";
const char PROGMEM TextoLenguaje11[]=" IDIOMA  ";
const char PROGMEM TextoLenguaje12[]=" SONIDO  ";

const char* const PROGMEM TextoLenguajeTabla[]={
TextoLenguaje00,
TextoLenguaje01,
TextoLenguaje02,
TextoLenguaje10,
TextoLenguaje11,
TextoLenguaje12,
};

//**************************************************************************************************
Menu()
//Presenta un menu y gestiona las opciones.
{
	static byte bFlecha=1; //Indica meneo y parpadeo de flecha.
	static byte bContador=1;
	static byte bMenu=1; //1-INICIO, 2-LENGAJE, 3-SONIDO.
	static byte bSiguienteMenu=1;
	byte bContador2=0;    //Local
	char cTextBuffer[20]; //Local

//		arduboy.audio.off();
		bSonido=0; //Detiene el sonido.
		if(arduboy.pressed(A_BUTTON) and bMenu==1 and bSiguienteMenu==1 and bContador>34){
			//Empieza el juego.
			bSonido=cSonidoA;
			bMapaEnCurso=1;
			InicializaNivel();
			arduboy.waitNoButtons();
			randomSeed(arduboy.frameCount); //Inicializo variable cuando pulsen A.
			bSonido=0; //Para sonido.
			return;
		}
		if(arduboy.pressed(RIGHT_BUTTON) and bContador>34)
			if(bMenu<3 and bMenu==bSiguienteMenu){
				bSiguienteMenu=bMenu+1;
				//bSonido=cSonidoDer;
				Pita(900,30,900,30);
				while(arduboy.pressed(RIGHT_BUTTON));
			}
		if(arduboy.pressed(LEFT_BUTTON))
			if(bMenu>1 and bMenu==bSiguienteMenu){
				bSiguienteMenu=bMenu-1;
				//bSonido=cSonidoIzq;
				Pita(850,30,850,30);
				while(arduboy.pressed(LEFT_BUTTON));
			}
		//Movimiento del letrero.
			arduboy.clear();
			//Caja de menu
			arduboy.fillRect(82-10,53,37,11,WHITE);
			arduboy.fillRect(83-10,54,35,9,BLACK);
			font3x5.setCursor(-10+90+30-bContador,55);
			//Copia cadena que voy a imprimir
			for(bContador2=0;bContador2<3;bContador2++){
				strcpy_P(cTextBuffer,(char*)pgm_read_word(&(TextoLenguajeTabla[bIdioma*3+bContador2])));
				font3x5.print(cTextBuffer);
			}
			//Borro area.
			arduboy.fillRect(119-10,50,127,64,BLACK);
			arduboy.fillRect(0,50-10,82-10,64,BLACK);
		//Pinto Pantalla.
			arduboy.drawCompressed(0,0,PantallaMenu);
			//Dibujo flechas.
			if(arduboy.everyXFrames(5))bFlecha++;
			if(bMenu==2 or bMenu==3){
				SpriteMirror(-10+74-bFlecha%4,55,Objetos8x8,Objetos8x8,cFlechaMenu,cFlechaMenu,1,0); //Flecha izquierda.
			}
			if(bMenu==1 or bMenu==2){
				SpriteMirror(-10+119+bFlecha%4,55,Objetos8x8,NULL,cFlechaMenu,NULL,0,0); //Flecha derecha.
			}
			//Muevo los menus
			if(bContador<35)bContador+=2;  //Deslizo el primer cartel.
			
			if(bMenu<bSiguienteMenu)
				bContador+=2;
			else if(bMenu>bSiguienteMenu)
				bContador-=2;

			if((bContador+1)%36==0)bMenu=bSiguienteMenu; //Paro cuando llego a la siguiente opcion.
			
			if(arduboy.pressed(A_BUTTON) and bMenu==2 and bSiguienteMenu==2){
				bSonido=cSonidoA;
				arduboy.waitNoButtons();
				bIdioma++;
				if(bIdioma==2)bIdioma=0;
				EEPROM.write(EEPROM_STORAGE_SPACE_START, bIdioma); 
			}
			if(arduboy.pressed(A_BUTTON) and bMenu==3 and bSiguienteMenu==3){
				bSonido=cSonidoA;
				arduboy.waitNoButtons();
				if(bSoundOnOff){
					bSoundOnOff=0;
					arduboy.audio.off();
				}
				else{
					bSoundOnOff=1;
					arduboy.audio.on();
					bSonido=cSonidoA;
				}
				EEPROM.write(EEPROM_AUDIO_ON_OFF, bSoundOnOff); 
			}
			if(bSoundOnOff==0)  //Borro altavoz.
				arduboy.fillRect(119,39,3,8,BLACK);
}

//**************************************************************************************************
