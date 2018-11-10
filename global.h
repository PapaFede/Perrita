//Estados de la perrita.
#define cDerecha     1
#define cIzquierda   2
#define cArriba      3
#define cAbajo       4
#define cParadoDerecha      5
#define cParadoIzquierda    6
#define cParadoArriba       7
#define cParadoAbajo        8
#define cCavandoDerecha     9
#define cCavandoIzquierda   10
#define cVencida            11
#define cTamanoPantallaEnCurso 130  //En tiles.
#define cAnchoPantalla 128
#define cAltoPantalla 64
#define cAnchoMapa 0
#define cAltoMapa 1
#define cFirstSolidTile 6 //Primer tile solido.
#define cLastSolidTile 250 //Ultimo tile solido.

//Ojbetos 16x16
#define cCasitaPerro   0
#define cCartel        1
#define cPuertaCerrada 2
#define cPuertaAbierta 3
#define cCabezaCerrada 4
#define cCabezaAbierta 5
#define cArbolRedondoOscuro 6
//Coordenadas Objetos 16x16
#define cCasetaPerroX 25
#define cCasetaPerroY 6
#define cCartelJardinX 45
#define cCartelJardinY 2
#define cCartelBosqueX 14
#define cCartelBosqueY 38
#define cCabezaLlaveX 50
#define cCabezaLlaveY 3
#define cArbol1X 28
#define cArbol1Y 12
#define cArbol2X 44
#define cArbol2Y 20
#define cArbol3X 44
#define cArbol3Y 14

//Ojbetos 8x8
#define cMonton     1
#define cCantidadMontones 16
#define cHuesito        2
#define cMascaraHuesito 0
#define cBalaHueso        3
#define cMascaraBalaHueso 1
#define cDistanciaDisparo 70
#define cVelocidadDisparo 2
#define cExplosionBalaHueso 4
#define cCantidadDisparos 5 //Numero maximo de disparos en pantalla.
#define cAnimacionExplosionBalaHueso 4 //Numero de frames en la explosion de BalaHueso
#define cLlave       0
#define cMascaraLlave 8
#define cAvispa1  8
#define cAvispa2  9
#define cAvispero 10 //Frame del avispero.
#define cTelarana 18
#define cArana1 16
#define cArana2 17

#define cPajaro1 12
#define cPajaro2 13
#define cMascaraPajaro1 9
#define cMascaraPajaro2 10
#define cPajaroVolando1 14
#define cPajaroVolando2 15
#define cMascaraPajaroVolando1 11
#define cMascaraPajaroVolando2 12
#define cFlechaMenu 11
#define cAltavoz 0

//SONIDOS
#define cSonidoA 1
#define cSonidoB 2
#define cSonidoIzq 3
#define cSonidoDer 4
#define cSonidoExplosion 5
#define cSonidoDisparo   6
#define cSonidoCavando   7
#define cSonidoObjetoEncontrado 8
//#define cSonido
byte  bSoundOnOff=1;  //0-no suena, 1-suena.

byte  bSonido=255; //0-Parar, 1-254 Toca un sonido, 255-No hace nada.
byte  bIdioma=0;  //0-Ingles, 1-Espanol, 2-Frances, 3-Aleman.
byte  bMapaEnCurso=0; //0-Menu, 255-Ganaste. Otro-Mapa que usamos actualemente.
int   iAnchoMapaEnCurso=0; //Tamaño del mapa en pixeles.
int   iAltoMapaEnCurso=0;
int   iCamX=0;
int   iCamY=0;
int   iPerritaX=0; //Variables de perrita.
int   iPerritaY=0;
int   PerritaFrame=0;
byte  bEstadoPerrita=cParadoDerecha;
byte  bEstadoPerritaCuandoPare=cParadoDerecha;
byte  bPerritaBits=0; //0-Inmunidad, 1-Parpadeo, 2-No Volver a Disparar, 3-AnimacionHuesito, 4-AnimacionLlave.
#define cBitInmune   0
#define cBitParpadeo 1
#define cNoVolverADisparar 2
#define cAnimacionHuesito  3
#define cAnimacionLlave    4
byte  bContadorInmunidad=0; //Numero de frames que permanezco inmune.
//DISPARO
int   aiDisparo[cCantidadDisparos*4]; //x,y,direccion,distancia que le queda, de los disparos.
int   aiExplosionDisparo[cCantidadDisparos*3]; //x,y,frame de la explosion.
byte  bVelocidadExplosion=10;
//PANEL
byte  bVidas=1;   //Vidas que tenemos. 0-9. //Tambien variable menu. Tambien sirve en el menu.
byte  bHuesos=0;  //Huesos. 0-255.
byte  bLlaves=0;  //llaves. 0-9.

byte bAnimacionObjetoEncontrado=30; //Si 0, esta parada, si es mayor, animar. Activa la nimacion del hueso o de la llave cuando se encuentra.
unsigned int MontonesBit=0; //Indica a nivel de bits si los montones se han excarvado.
byte MontonesLlaveBit=0; //Montones con llaves.
byte bEnemigos=0;  //Indica el numero de Enemgos que hay en la pantalla. Avisperos + avispas en el nivel.
//ENEMIGOS
//===========
//AVISPEROS
#define cCantidadAvisperos 14 //Numero de avisperos en el nivel. 
#define cReviveAvispa 140 //Frames para revivir avispa muerta.
#define cReviveArana 50
#define cCantidadAvispas cCantidadAvisperos*3  //Por cada avispero hay 3 avispas
byte  abAvisperos[cCantidadAvisperos]; //0-5 Vida del avispero. Las coordenadas están en una constante.
int   aiAvispas[cCantidadAvispas*2]; //x,y,frame de animacion. 3 avispas por avispero. 
									   //Si x==0 la avispa esta muerta, e y es el contador para revivirla.
byte bInicializaAvispas=0;  //Agitamos un poco las avispas.
//Pajaro
#define cCantidadPajaros 5
#define cPajaroX 0  //Para coordenadas absolutas.
#define cPajaroY 1
#define cPajaroDestinoX 0  //Para los tiles
#define cPajaroDestinoY 1
byte abPajaros[cCantidadPajaros*2]; //Tile en que se encuentra el pajaro.
int aiPajaros[cCantidadPajaros*2];  //Coordenadas absolutas a la que debe de ir el pajaro.
byte PajarosBit = 0; //Indica si el pajaro se dibuja en espejo o no.
byte PuertasBit = 0; //En el nivel 2 hay 5 puertas.
//Arana Grande
int iAranaGrandeX=0;
int iAranaGrandeY=0;
byte bVidaArana=0;
