//**************************************************************************************************
InicializaNivel()
{
	byte bContador=0;

    //Inicializando mapa
//	bMapaEnCurso=2; //Mapa que usamos actualemente.
//		bVidas=3;   //Vidas que tenemos. 0-9.
//		bHuesos=00;  //Huesos. 0-255.
//		bLlaves=0;  //llaves. 0-9.
    //Actualizo dimensiones en tiles.
    iAnchoMapaEnCurso=pgm_read_byte(&TileMapsDimensions[(bMapaEnCurso-1)*2])*8;
    iAltoMapaEnCurso=pgm_read_byte(&TileMapsDimensions[(bMapaEnCurso-1)*2+1])*8;
	//Inicializando variables perrita y camara.
	bEstadoPerrita=cParadoDerecha;
	bEstadoPerritaCuandoPare=cParadoDerecha;
	
	bPerritaBits=0;
	bContadorInmunidad=0; //Numero de frames que permanezco inmune.
	
	if(bMapaEnCurso==1){
		bVidas=3;   //Vidas que tenemos. 0-9.
		bHuesos=00;  //Huesos. 0-255.
		bLlaves=0;  //llaves. 0-9.
	}

	MontonesBit=65535; //Indica a nivel de bits si los montones se han excarvado.
	MontonesLlaveBit=255;
	bEnemigos=cCantidadAvisperos+cCantidadAvispas;
	PajarosBit=0;
	PuertasBit=0;

	//Inicializa los pajaros.
	for(bContador=0;bContador<cCantidadPajaros;bContador++){
		aiPajaros[bContador*2]=30000;
		aiPajaros[bContador*2+1]=0;
		abPajaros[bContador*2]=0;
		abPajaros[bContador*2+1]=0;
	}
	
	if(bMapaEnCurso==1){
		iPerritaX=220; //Variables de perrita.
		iPerritaY=60;
		iCamX=iPerritaX-56;
		iCamY=iPerritaY-24;
	}else{
	iPerritaX=6*8; //Variables de perrita.
	iPerritaY=44*8;
	iCamX=iPerritaX-56+8;
	iCamY=iPerritaY-24-8;

//	iPerritaX=45*8; //Variables de perrita.
//	iPerritaY=6*8;
//	iCamX=iPerritaX;
//	iCamY=iPerritaY;

	bVidaArana=100;
	iAranaGrandeX=45*8;
	iAranaGrandeY=3*8;
	}
	//Inicializo avisperos **********************************
    for(bContador=0;bContador<cCantidadAvisperos;bContador++) //Inicializo vida de los avisperos.
      abAvisperos[bContador]=3;
	//Inicializa avispas ************************************
	InicializaAvispas();
	//Inicializamos explosiones
    for(bContador=0;bContador<cCantidadDisparos;bContador++){
        aiExplosionDisparo[bContador*3+2]=4;
    }
}
//**************************************************************************************************
Nivel()
//Ejecuta el nivel 1.
{
	static byte bFallaste=1;
	
	int iContador=0;
	ProcesoGenerico();
	//Miro si estoy en el cubil de la araña.
	if(bMapaEnCurso==2 and iPerritaX>33*8 and iPerritaX<62*8 and iPerritaY>1*8 and iPerritaY<12*8+4){
		//Cierro puertas.
		PuertasBit=0;
		PintaAranaGrande();
	}
	Player();
//	bEnemigos=0;
//	bLlaves=10;
	PintaVida();
	//Compruebo si he sido vencida.
	if(bVidas==0){
		if(bFallaste){
			//MUSICA FALLASTE
			for(iContador=4000;iContador>30;iContador--){
				Pita(iContador, 10, iContador, 20);
			}
			bSonido=0; //Para sonido.
			while(arduboy.pressed(A_BUTTON));
		}
		TextoPresionaA();
		bFallaste=0;
		if(arduboy.pressed(A_BUTTON)) {
			PintaPerritaTriste();
			bMapaEnCurso=0;
			bPerritaBits=0;
		}
	}else bFallaste=1;
		
	
	
	//Compruebo si he ganado.
	if((bMapaEnCurso==1 and bitRead(PuertasBit,0))){
		bMapaEnCurso=2;
		InicializaNivel();
	}
	if((bMapaEnCurso==2 and bVidaArana==0 and bEnemigos==0)){
		TextoPresionaA();
		EsperarBotonA();
		MuestraTexto2Lineas(4);//Has ganado.
		bMapaEnCurso=0;
		bPerritaBits=0;
	}
	
}
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************

