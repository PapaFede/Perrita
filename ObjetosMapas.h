//Mapa Jardin.
//------------
//Puerta Tile 28,0 .Se abre con la llave.
//**************************************************************************************************
ColisionPajaro(byte bContador)
//Comprueba que el pajaro no choca con nada.
//bContador indica el numero de pajaro.
{

	int iDestinoX=0; //Variables para optimizar
	int iDestinoY=0;
	
	iDestinoX=abPajaros[bContador*2+cPajaroDestinoX]*8;
	iDestinoY=abPajaros[bContador*2+cPajaroDestinoY]*8;

	//Choque con un tile solido.
	if(Colision8x8(iDestinoX,iDestinoY)) return true;
	// Choque con avispas.
	if(ColisionAvispas(iDestinoX,iDestinoY,8,8)) return true;
	//Choque con avisperos
	if(ColisionAvisperos(iDestinoX,iDestinoY,8,8)) return true;
	//Choque con perrita.
	if(arduboy.collide((Rect){iPerritaX,iPerritaY,16,16},
		(Rect){iDestinoX,iDestinoY,8,8})) return true;
	//Choque con Arboles.
	if(arduboy.collide((Rect){cArbol1X*8,cArbol1Y*8,16,16},
		(Rect){iDestinoX,iDestinoY,8,8})) return true;
	if(arduboy.collide((Rect){cArbol2X*8,cArbol2Y*8,16,16},
		(Rect){iDestinoX,iDestinoY,8,8})) return true;
	if(arduboy.collide((Rect){cArbol3X*8,cArbol3Y*8,16,16},
		(Rect){iDestinoX,iDestinoY,8,8})) return true;
	return false;
	
}
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
PintaTile16x16(byte bTile16X16, int iX, int iY)
//Pinta un Tile de 16x16 formado por 4 tiles de 8x8.
//bTile - Numero de tile.
//iX    - Coordenada absoluta del mapa.
//iY    - Coordenada absoluta del mapa.
{
	byte bContador=0;
	byte bEspejo=0;
	byte bTile=0;
	
	for(bContador=bTile16X16*4;bContador<(bTile16X16+1)*4;bContador++){
		bTile=pgm_read_byte(&aTiles16x16[bContador]);
		if(bTile>128){
			bTile-=128;
			bEspejo=1;}
		else bEspejo=0;
			SpriteMirror(iX*8-iCamX+((bContador-bTile16X16*4)/2)*8,iY*8-iCamY+((bContador-bTile16X16*4)%2)*8
				,Tiles,NULL,bTile,NULL,bEspejo,0);
	}
}
//**************************************************************************************************
PintaCabezaLlave()
//Dibuja la cabeza donde se esconde la llave.
{
	if(bEnemigos or bLlaves)
		PintaTile16x16(cCabezaCerrada,50,3);
	else if(!bEnemigos){
		PintaTile16x16(cCabezaAbierta,50,3);
		if(arduboy.collide((Rect){50*8-1,3*8+1,15,15},
			(Rect){iPerritaX+2,iPerritaY+2,14,14})and arduboy.pressed(A_BUTTON)){
			arduboy.waitNoButtons();
			bLlaves++;
			bitSet(bPerritaBits,cAnimacionLlave);
			bAnimacionObjetoEncontrado=0;
		}
	}
}
//**************************************************************************************************
ProcesoGenerico()
{
//*******PINTA PANTALLA
//Pinta la pantalla correspondiente al inicio de coordenadas iCamX, iCamY.
	int iContadorX;
	int iContadorY;
	int iTileX=0; //Coordenadas del primer tile que hay que dibujar.
	int iTileY=0;
	int iDesplazamiento=0;
	byte bTile=0; //Tile a dibujar.
	byte bEspejo=0; //0-Sin Espejo. 1-Con Espejo.
	byte bDatoX=0;
	byte bDatoY=0;
	byte b1=0; //Puertas inicio y fin.
	byte b2=0;
	byte bContador=0;
	byte bPosicion=0; //Abierta o cerrada.
	int iDatoX=0;
	int iDatoY=0;
	byte bFrame=0;
	byte bMascara=0;
	int iDireccionX=0;
	int iDireccionY=0;

	



	
	iTileX=iCamX/8;
	iTileY=iCamY/8;
	//Redondemamos al tile anterior.
	if(iTileX*8<iCamX) iTileX++;
	if(iTileY*8<iCamY) iTileY++;
	for(iContadorY=iCamY/8;iContadorY<=iCamY/8+cAltoPantalla/8;iContadorY++){
		//Calcula Desplazamiento
		iDesplazamiento=pgm_read_byte(&aDesplazamientos[pgm_read_byte(&aDesplazamientosInit[bMapaEnCurso-1])+(iContadorY/4)]);
		for(iContadorX=iCamX/8;iContadorX<=iCamX/8+cAnchoPantalla/8;iContadorX++){
			//Compruembo en que lado del espejo esta.
			bTile=CalculaTile(iContadorX,iContadorY);
			if(bTile==0 or
				((iContadorX>=iDesplazamiento )
				and(iContadorX<((iAnchoMapaEnCurso/8)/2)+iDesplazamiento))
				or
				(iDesplazamiento>((iAnchoMapaEnCurso/8)/2) and 
				iContadorX<iDesplazamiento-((iAnchoMapaEnCurso/8)/2))
				)
				bEspejo=0;
			else bEspejo=1;
			SpriteMirror(iContadorX*8-iCamX,iContadorY*8-iCamY,
			Tiles,NULL,bTile,NULL,bEspejo,0);
			}
		}
//*******PINTA CARTEL
//Dibuja una señal y gestiona su lectura.	
	switch (bMapaEnCurso){
		case 1:
			bDatoX=cCartelJardinX;
			bDatoY=cCartelJardinY;
			break;
		case 2:
			bDatoX=cCartelBosqueX;
			bDatoY=cCartelBosqueY;
			break;
	} //End switch
	PintaTile16x16(cCartel,bDatoX,bDatoY);

	if(arduboy.collide((Rect){bDatoX*8-1,bDatoY*8-1,15,15},
		(Rect){iPerritaX+2,iPerritaY+2,14,14})and arduboy.pressed(A_BUTTON)){
		MuestraTexto2Lineas(bMapaEnCurso);
	}
//*******PINTA PUERTA
//Pinta y gestiona la puerta de salida.
	switch (bMapaEnCurso){
		case 1: //1 puerta.
			b1=1;
			b2=1;
			break;
		case 2: //5 puertas+1 especial.
			b1=2;
			b2=6;
			break;
	} //End switch

	for(bContador=b1;bContador<=b2;bContador++){
		if(bitRead(PuertasBit,bContador-b1)){
		bPosicion=cPuertaAbierta;
		}
		else {
		bPosicion=cPuertaCerrada;
		}
		PintaTile16x16(bPosicion,pgm_read_byte(&aPuertas[(bContador-1)*2]),pgm_read_byte(&aPuertas[(bContador-1)*2+1]));
		if(arduboy.collide((Rect){pgm_read_byte(&aPuertas[(bContador-1)*2])*8+1,pgm_read_byte(&aPuertas[(bContador-1)*2+1])*8+1,17,17},
			(Rect){iPerritaX+2,iPerritaY+2,14,14})and arduboy.pressed(A_BUTTON)and(!bitRead(PuertasBit,bContador-b1))){
			if(bLlaves==0){
				MuestraTexto2Lineas(3);
			}else
				if((bContador<b2)or(bContador==b2 and bEnemigos==0)){
					bLlaves--;
					bitSet(PuertasBit,bContador-b1);
			}
		}
	}
//*******PINTA MONTONES
//Comprueba si hay que pintar algun objeto entre las coordenas iCamX,iCamY - iCamX+anchoPantalla,iCamY+altoPantalla,
//o sea, en la pantalla que estamos dibujando..

  //Comprobamos montoncitos.
  
  for(bContador=((bMapaEnCurso-1)*16);bContador<cCantidadMontones+((bMapaEnCurso-1)*16);bContador++){
    bDatoX=pgm_read_byte(&aMonton[bContador*3]);
    bDatoY=pgm_read_byte(&aMonton[bContador*3+1]);
    if (bitRead(MontonesBit,bContador-((bMapaEnCurso-1)*16))and
      (((iCamX-8<bDatoX*8) and (iCamX+8+cAnchoPantalla>bDatoX*8)) or ((iCamY-8<bDatoY*8) and (iCamY+8+cAltoPantalla>bDatoY*8)))){
        SpriteMirror(bDatoX*8-iCamX,bDatoY*8-iCamY,Objetos8x8,NULL,cMonton,NULL,0,0);
		//Compruebo si estan escarvando
      if(bitRead(MontonesBit,bContador-((bMapaEnCurso-1)*16))and((arduboy.collide((Rect){iPerritaX-iCamX,iPerritaY-iCamY,16,16},
											(Rect){bDatoX*8-iCamX,bDatoY*8-iCamY,8,8}))and
        (bEstadoPerrita==cCavandoDerecha or bEstadoPerrita==cCavandoIzquierda)and(!bitRead(bPerritaBits,cAnimacionHuesito)))){
          bitSet(bPerritaBits,cAnimacionHuesito); //Acivo animacion hueso.
		  bAnimacionObjetoEncontrado=0;
		  if(bHuesos+pgm_read_byte(&aMonton[bContador*3+2])>255)bHuesos=255;  //Limite de huesos 255.
			else
			bHuesos+=pgm_read_byte(&aMonton[bContador*3+2]);
          bitClear(MontonesBit,bContador-((bMapaEnCurso-1)*16));
        }
    }
    }
//*******PINTA MONTONES LLAVE
//Comprueba si hay que pintar algun objeto entre las coordenas iCamX,iCamY - iCamX+anchoPantalla,iCamY+altoPantalla,
//o sea, en la pantalla que estamos dibujando..

  //Comprobamos montoncitos.
  if(bMapaEnCurso==2)
  for(bContador=0;bContador<5;bContador++){
    bDatoX=pgm_read_byte(&aMontonesLlaves[bContador*2]);
    bDatoY=pgm_read_byte(&aMontonesLlaves[bContador*2+1]);
    if (bitRead(MontonesLlaveBit,bContador)and
      (((iCamX-8<bDatoX*8) and (iCamX+8+cAnchoPantalla>bDatoX*8)) or ((iCamY-8<bDatoY*8) and (iCamY+8+cAltoPantalla>bDatoY*8)))){
        SpriteMirror(bDatoX*8-iCamX,bDatoY*8-iCamY,Objetos8x8,NULL,cMonton,NULL,0,0);
		//Compruebo si estan escarvando
      if(bitRead(MontonesLlaveBit,bContador)and((arduboy.collide((Rect){iPerritaX-iCamX,iPerritaY-iCamY,16,16},
											(Rect){bDatoX*8-iCamX,bDatoY*8-iCamY,8,8}))and
        (bEstadoPerrita==cCavandoDerecha or bEstadoPerrita==cCavandoIzquierda)and(!bitRead(bPerritaBits,cAnimacionLlave)))){
          bitSet(bPerritaBits,cAnimacionLlave); //Acivo animacion llave.
		  bAnimacionObjetoEncontrado=0;
		  bLlaves++;
          bitClear(MontonesLlaveBit,bContador);
        }
    }
    }

	
	
	
//*******PINTA AVISPEROS

//Pinta los avisperos.

	for(bContador=0;bContador<cCantidadAvisperos;bContador++){
		bDatoX=pgm_read_byte(&aAvisperos[bContador*2+(bMapaEnCurso-1)*cCantidadAvisperos*2]);
		bDatoY=pgm_read_byte(&aAvisperos[bContador*2+1+(bMapaEnCurso-1)*cCantidadAvisperos*2]);
		if ((abAvisperos[bContador])and
		  (((iCamX-8<bDatoX*8) and (iCamX+8+cAnchoPantalla>bDatoX*8)) or 
		  ((iCamY-8<bDatoY*8) and (iCamY+8+cAltoPantalla>bDatoY*8)))){
		  if(bContador<cCantidadAvisperos){
			if(bMapaEnCurso==1)
				bFrame=cAvispero;
			else
				bFrame=cTelarana;
			SpriteMirror(bDatoX*8-iCamX,bDatoY*8-iCamY,Objetos8x8,NULL,bFrame,NULL,0,0);
			}
		}
	}
//*******MUEVE AVISPAS
//Hacen que las avispas vuelen teniendo en cuenta las colisiones.

	
	while(bInicializaAvispas>0){
		for(bContador=0;bContador<cCantidadAvispas;bContador++){
			if(aiAvispas[bContador*2]){ //Avispa viva, a pintarla.
				iDatoX=aiAvispas[bContador*2];
				iDatoY=aiAvispas[bContador*2+1];
				//comprueba que esta en la zona de visualizacion
				if((((iCamX-8<iDatoX) and (iCamX+8+cAnchoPantalla>iDatoX)) or
					((iCamY-8<iDatoY) and (iCamY+8+cAltoPantalla>iDatoY)))){
					if(random(0,5)==3){ //Una probavilidad de 1/5 de que se mueva.
						if(bVidaArana==0){//Si la arana gigante esta muerta se mueven aleatoriamente
							iDireccionX=iDatoX+random(-2,3);
							iDireccionY=iDatoY+random(-2,3);
						}else{ //Si esta viva se mueven hasta que llegan por debajo de cierta coordenada.
							if(iDatoY<iAranaGrandeY+28){
								iDireccionY=iDatoY+random(1,2);
								iDireccionX=iDatoX;
							}else{
								iDireccionX=iDatoX+random(-2,3);
								iDireccionY=iDatoY+random(-2,3);
								}
						}
//						if(iDireccionX<1) iDireccionX=1; //Para que no se salgan del mapa.
//						if(iDireccionY<1) iDireccionY=1;
						if(!ColisionGeneral8x8(iDireccionX,iDatoY))
							aiAvispas[bContador*2]=iDireccionX;
						if(!ColisionGeneral8x8(iDatoX,iDireccionY))
							aiAvispas[bContador*2+1]=iDireccionY;
					}
				}
			}
		}
		bInicializaAvispas--;
	}
	bInicializaAvispas++;
//*******PINTA AVISPAS
//Pinta las avispas de la pantalla. Si no estan muertas.

	for(bContador=0;bContador<cCantidadAvispas;bContador++){
		if(aiAvispas[bContador*2]){ //Avispa viva, a pintarla.
			iDatoX=aiAvispas[bContador*2];
			iDatoY=aiAvispas[bContador*2+1];
			//comprueba que esta en la zona de visualizacion
			if((((iCamX-8<iDatoX) and (iCamX+8+cAnchoPantalla>iDatoX)) or
				((iCamY-8<iDatoY) and (iCamY+8+cAltoPantalla>iDatoY)))){
				if(bMapaEnCurso==1){
					if(arduboy.everyXFrames(3)){
						bFrame=cAvispa1;
						bMascara=6;
					}else{
						bFrame=cAvispa2;
						bMascara=7;
					}
					SpriteMirror(iDatoX-iCamX, iDatoY-iCamY, Objetos8x8, Mascaras8x8,
											bFrame, bMascara,0,0);
					}
				else
				if(bMapaEnCurso==2)
					SpriteMirror(iDatoX-iCamX, iDatoY-iCamY, Objetos8x8,NULL,cArana1+PerritaFrame,NULL,0,0);
			}
        }
    }
//*******COLISION AVISPAS PROTA
//Comprueba si la avispa ha colisionado con el prota.
//Lo ignora si el prota esta inmune.

	if(bitRead(bPerritaBits,cBitInmune)==0 and bVidas>0) //No es inmune, compruebo colisiones.
		if(ColisionAvispas(iPerritaX+2,iPerritaY+2,14,14)){
			bitSet(bPerritaBits,cBitInmune);
			if(bVidas>0) bVidas--;
		}
//*******CHEQUEA AVISPA MUERTA
//Comprueba si hay que revivir alguna avispa. Y la revive.

	for(bContador=0;bContador<cCantidadAvispas;bContador++){
		if(!aiAvispas[bContador*2] and abAvisperos[bContador/3]){ 
		     //Avispa muerta, compruebo si el avispero vivo
			aiAvispas[bContador*2+1]++;
			if((aiAvispas[bContador*2+1]>=cReviveAvispa)or
			  (bMapaEnCurso==2 and aiAvispas[bContador*2+1]>=cReviveArana)){
				aiAvispas[bContador*2]=(pgm_read_byte(&aAvisperos[bContador/3*2+((bMapaEnCurso-1)*cCantidadAvisperos*2)])*8);
				aiAvispas[bContador*2+1]=(pgm_read_byte(&aAvisperos[bContador/3*2+1+((bMapaEnCurso-1)*cCantidadAvisperos*2)])*8);
				bEnemigos++;
			}
		}
	}
//*******PINTA PAJARO
//Dibuja un pajaro de adorno.
//El pajaro no puede estar en ningun tile solido, ni avispero, ni estatua, ni puerta, ni avispa.
//Ni perrita.
//Cambia de frame al ritmo de la cola de la perrita.
	
	for(bContador=0;bContador<cCantidadPajaros;bContador++){
		if(aiPajaros[bContador*2+cPajaroX]==30000){ //El pajaro esta sin inicializar
			//Inicializa coordenadas pajaro
			while(ColisionPajaro(bContador)){ //Busco nuevas coordenadas.
				abPajaros[bContador*2+cPajaroDestinoX]=random(1,iAnchoMapaEnCurso/8-1);
				abPajaros[bContador*2+cPajaroDestinoY]=random(1,iAltoMapaEnCurso/8-1);
			}
			aiPajaros[bContador*2+cPajaroX]=0;
		}
		if(aiPajaros[bContador*2+cPajaroX]/8==abPajaros[bContador*2+cPajaroDestinoX] and 
		   aiPajaros[bContador*2+cPajaroY]/8==abPajaros[bContador*2+cPajaroDestinoY]){
			while(ColisionPajaro(bContador)){
				abPajaros[bContador*2+cPajaroDestinoX]+=random(-6,6);
				abPajaros[bContador*2+cPajaroDestinoY]+=random(-3,3);
				if(abPajaros[bContador*2+cPajaroDestinoX]*8>iAnchoMapaEnCurso-8 or abPajaros[bContador*2+cPajaroDestinoY]*8>iAltoMapaEnCurso-8
				   or abPajaros[bContador*2+cPajaroDestinoX]*8<8 or abPajaros[bContador*2+cPajaroDestinoY]*8<8){ //Busco nuevas coordenadas.
				abPajaros[bContador*2+cPajaroDestinoX]=aiPajaros[bContador*2+cPajaroX]/8;
				abPajaros[bContador*2+cPajaroDestinoY]=aiPajaros[bContador*2+cPajaroY]/8;
				}
			}
			SpriteMirror( aiPajaros[bContador*2+cPajaroX]-iCamX, aiPajaros[bContador*2+cPajaroY]-iCamY,Objetos8x8,Mascaras8x8,
									  cPajaro1+PerritaFrame,cMascaraPajaro1+PerritaFrame,bitRead(PajarosBit,bContador),0);
			bFrame=random(1,40);  //Unas veces pica para un lado, otras para el otro. Si el random no coincide, no cambio de posicion.
			switch(bFrame){
				case 2:
					bitSet(PajarosBit,bContador);
					break;
				case 3:
					bitClear(PajarosBit,bContador);
					break;
			}
		}
		else SpriteMirror( aiPajaros[bContador*2+cPajaroX]-iCamX, aiPajaros[bContador*2+cPajaroY]-iCamY,Objetos8x8,Mascaras8x8,
									   cPajaroVolando1+PerritaFrame,cMascaraPajaroVolando1+PerritaFrame,0,0);

			if(aiPajaros[bContador*2+cPajaroX]<abPajaros[bContador*2+cPajaroDestinoX]*8)aiPajaros[bContador*2+cPajaroX]++;
			if(aiPajaros[bContador*2+cPajaroX]>abPajaros[bContador*2+cPajaroDestinoX]*8)aiPajaros[bContador*2+cPajaroX]--;
			if(aiPajaros[bContador*2+cPajaroY]<abPajaros[bContador*2+cPajaroDestinoY]*8)aiPajaros[bContador*2+cPajaroY]++;
			if(aiPajaros[bContador*2+cPajaroY]>abPajaros[bContador*2+cPajaroDestinoY]*8)aiPajaros[bContador*2+cPajaroY]--;
	}
//*******PINTA ARBOLES SUELTOS

	if(bMapaEnCurso==2){
		PintaTile16x16(cArbolRedondoOscuro,cArbol1X,cArbol1Y);
		PintaTile16x16(cArbolRedondoOscuro,cArbol2X,cArbol2Y);
		PintaTile16x16(cArbolRedondoOscuro,cArbol3X,cArbol3Y);
	}else{
		PintaTile16x16(cCasitaPerro,cCasetaPerroX,cCasetaPerroY);
		PintaCabezaLlave();
	}

	

		
}
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
InicializaAvispas()
//Crea todas las avispas.
//Cada 3 avispas pertenecen a un avispero. Hay que tenerlo en cuenta a la hora de crearlas.
{
	byte bContador;
	int  iDatoX=0; //Tile x del avispero.
	int  iDatoY=0; //Tile y del avispero.
	byte bContador2=0;

	for(bContador=0;bContador<cCantidadAvisperos;bContador++){ //Bucle para los avisperos.
		//Leo coordenadas en tiles del bucle.
		iDatoX=pgm_read_byte(&aAvisperos[bContador*2+((bMapaEnCurso-1)*cCantidadAvisperos*2)])*8;
		iDatoY=pgm_read_byte(&aAvisperos[bContador*2+1+((bMapaEnCurso-1)*cCantidadAvisperos*2)])*8;
		//Pongo las avispas en los avisperos y las agito un poco.
		for(bContador2=0;bContador2<3;bContador2++){
			aiAvispas[bContador*6+bContador2*2]=(iDatoX);
			aiAvispas[bContador*6+1+bContador2*2]=(iDatoY);
		}
	}
	//Muevo un poco las avispas.
	bInicializaAvispas=20;

}
//**************************************************************************************************
//**************************************************************************************************
ColisionAvispaBala(int iX, int iY)
//Comprueba si alguna avispa ha sido alcanzada por una bala en las coordenadas iX, iY.
//Mata la avispa y devuelve true.
{
	byte bContador=0;
	int iDatoX=0;
	int iDatoY=0;

	for(bContador=0;bContador<cCantidadAvispas;bContador++){
		if(aiAvispas[bContador*2]){ //Avispa viva, compruebo
			iDatoX=aiAvispas[bContador*2];
			iDatoY=aiAvispas[bContador*2+1];
			//comprueba que esta en la zona de visualizacion
			if((((iCamX-8<iDatoX) and (iCamX+8+cAnchoPantalla>iDatoX)) or
				((iCamY-8<iDatoY) and (iCamY+8+cAltoPantalla>iDatoY)))){
				if(arduboy.collide((Rect){iDatoX+2,iDatoY+2,6,6},
					(Rect){iX-2,iY-2,6,6})){
					aiAvispas[bContador*2]=0; //x==0, avispa muerta.
					aiAvispas[bContador*2+1]=0; //y==cReviveAvispa, se revive.
					bEnemigos--;
					//Explosion de avispa. Usamos ultimo slot.
					aiExplosionDisparo[(cCantidadDisparos-1)*3]=iDatoX; //Coordenada X de la explosion.
					aiExplosionDisparo[(cCantidadDisparos-1)*3+1]=iDatoY; //Coordenada Y de la explosion.
					aiExplosionDisparo[(cCantidadDisparos-1)*3+2]=0; //Frame de la explosión. Si es 4==explosion terminada.
				return true;
				}
			}
		}
	}
	return false;
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
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************

