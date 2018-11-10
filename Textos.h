//**************************************************************************************************
/* TEXTOS

1-LA SOLEDAD TE HARA LIBRE
2-NECESITAS UNA LLAVE

*/
//**************************************************************************************************
//LONGITUD MAXIMA DE LAS CADENAS 20 CARACTERES.
//Cadenas de texto de 2 lineas.
//1-LA SOLEDAD TE HARA LIBRE
const char PROGMEM Texto2Lineas00[]="LONELINESS";
const char PROGMEM Texto2Lineas01[]="WILL MAKE YOU FREE";

const char PROGMEM Texto2Lineas10[]="LA SOLEDAD";
const char PROGMEM Texto2Lineas11[]="TE HARA LIBRE";

//2-Necesitaras dos llaves
const char PROGMEM Texto2Lineas20[]="LONELINESS AND";
const char PROGMEM Texto2Lineas21[]="ONE KEY YOU NEED";

const char PROGMEM Texto2Lineas30[]="LA SOLEDAD Y";
const char PROGMEM Texto2Lineas31[]="UNA LLAVE NECESITARAS";

//3-NECESITAS UNA LLAVE
const char PROGMEM Texto2Lineas40[]="YOU NEED";
const char PROGMEM Texto2Lineas41[]="A KEY";

const char PROGMEM Texto2Lineas50[]="NECESITAS";
const char PROGMEM Texto2Lineas51[]="UNA LLAVE";

//4-GANASTE
const char PROGMEM Texto2Lineas60[]="...AND THE FOREST WAS RELEASED...";
const char PROGMEM Texto2Lineas61[]="...AND THERE WAS PEACE";

const char PROGMEM Texto2Lineas70[]="...Y EL BOSQUE FUE LIBERADO...";
const char PROGMEM Texto2Lineas71[]="...Y HUBO PAZ";




const char* const PROGMEM Texto2LineasTabla[]={
Texto2Lineas00,Texto2Lineas01,
Texto2Lineas10,Texto2Lineas11,
Texto2Lineas20,Texto2Lineas21,
Texto2Lineas30,Texto2Lineas31,
Texto2Lineas40,Texto2Lineas41,
Texto2Lineas50,Texto2Lineas51,
Texto2Lineas60,Texto2Lineas61,
Texto2Lineas70,Texto2Lineas71,
};

//Cadenas de texto "PRESIONA A. *********************************
const char PROGMEM TextoPresionaA0[]="  PRESS A   ";
const char PROGMEM TextoPresionaA1[]=" PRESIONA A ";

const char* const PROGMEM TextoPresionaATabla[]={
TextoPresionaA0,TextoPresionaA1};


//**************************************************************************************************
MuestraTexto2Lineas(byte bLinea)
//Muestra un texto de 2 lineas con un recuadro y espera a que se pulse A.
//bLinea==Contiene la linea a mostrar.
{
	char  cTextBuffer[35];
	byte bContador=0;
	byte bContador2=8;
	byte bMariposaY=15;
	byte bMariposaSentido=0; //1-Sube. 2-Baja.
	byte bFrame=0;
	byte bPajaroVolando=0;  //1-Volando. 0-Parado.
	
	
	
	String sBuffer=""; //Variable para poder medir la longitud.

	arduboy.waitNoButtons();

	//Dibujo marco de la ventana.
	arduboy.fillRect(5,10,118,44,BLACK);
	arduboy.drawRect(7,12,114,40,WHITE); 
	//Copio cadena a cTextBuffer.
	//bIdioma==0 a 3
	//1a linea.
	if(bLinea==4){ //Animacion final
		arduboy.clear();
		bMariposaSentido=10;
	}
	
	for(bContador=0;bContador<2;bContador++){
		strcpy_P(cTextBuffer,(char*)pgm_read_word(&(Texto2LineasTabla[((bLinea-1)*4)+bContador+(bIdioma*2)])));
		sBuffer=cTextBuffer;
		
		font3x5.setCursor((128-sBuffer.length()*4)/2,20-bMariposaSentido+bContador*14);
		font3x5.println(sBuffer);
	}

	if(bLinea==4){ //Animacion final
		//Inicializa el pajaro.
		aiPajaros[0]=-10;
		
		while(!arduboy.pressed(A_BUTTON)){
			//Cuadrado blanco.
			arduboy.fillRect(0,36,128,28,WHITE); 
			//Hierba
			for(bContador=0;bContador<19;bContador++){
				SpriteMirror(bContador*8+bContador2-24,64-8,Tiles,NULL,26+bContador%3,NULL,0,0);
			}
			if(bContador2<1)bContador2=24;
			if(arduboy.everyXFrames(2)){
				bContador2--;
			}
			//Perrita
			SpriteMirror(4,64-24+8, Perrita,PerritaMascaras,PerritaFrame,PerritaFrame,1,0);
			if(arduboy.everyXFrames(15)){
			  if(PerritaFrame==1){
				PerritaFrame=0;
			  }
			  else{
				PerritaFrame=1;
			  }
			}
			//Mariposa.
			SpriteMirror(4+25,64-38+bMariposaY+8, Objetos8x8,NULL,19+PerritaFrame,NULL,0,0);
			if(arduboy.everyXFrames(10))
			if(bMariposaSentido)bMariposaY++;
			else bMariposaY--;
			if(bMariposaY==10)bMariposaSentido=1;
			if(bMariposaY==15)bMariposaSentido=0;
			//Pajarito
			
			if(bPajaroVolando==0){
				if(aiPajaros[cPajaroX]<40){
					abPajaros[cPajaroDestinoX]=random(30,128-8);
					abPajaros[cPajaroDestinoY]=random(36,64-10);
					bPajaroVolando=1;
				}
				if(arduboy.everyXFrames(2))aiPajaros[cPajaroX]--;
				
				SpriteMirror( aiPajaros[cPajaroX], aiPajaros[cPajaroY],Objetos8x8,Mascaras8x8,
										  cPajaro1+PerritaFrame,cMascaraPajaro1+PerritaFrame,bitRead(PajarosBit,0),0);
				bFrame=random(1,40);  //Unas veces pica para un lado, otras para el otro. Si el random no coincide, no cambio de posicion.
				switch(bFrame){
					case 2:
						bitSet(PajarosBit,0);
						break;
					case 3:
						bitClear(PajarosBit,0);
						break;
				}
			}
			else{
				SpriteMirror( aiPajaros[cPajaroX], aiPajaros[cPajaroY],Objetos8x8,Mascaras8x8,
										   cPajaroVolando1+PerritaFrame,cMascaraPajaroVolando1+PerritaFrame,0,0);

				if(aiPajaros[cPajaroX]<abPajaros[cPajaroDestinoX])aiPajaros[cPajaroX]++;
				if(aiPajaros[cPajaroX]>abPajaros[cPajaroDestinoX])aiPajaros[cPajaroX]--;
				if(aiPajaros[cPajaroY]<abPajaros[cPajaroDestinoY])aiPajaros[cPajaroY]++;
				if(aiPajaros[cPajaroY]>abPajaros[cPajaroDestinoY])aiPajaros[cPajaroY]--;
				if(aiPajaros[cPajaroX]==abPajaros[cPajaroDestinoX] and aiPajaros[cPajaroX]==abPajaros[cPajaroDestinoX])
				bPajaroVolando=0;
			}	

			arduboy.display();
			while(!(arduboy.nextFrame()));
		}
	}


	
	EsperarBotonA();

}
//**************************************************************************************************
TextoPresionaA()
//Muestra un mensaje enpantalla pidiendo que se presione A
{
	char cTextBuffer[20];
	
	strcpy_P(cTextBuffer,(char*)pgm_read_word(&(TextoPresionaATabla[bIdioma])));
	arduboy.fillRect((128-12*4)/2,29-10,12*4,9,BLACK);
	font3x5.setCursor((128-12*4)/2,30-10);
	font3x5.print(cTextBuffer);
}
//**************************************************************************************************
