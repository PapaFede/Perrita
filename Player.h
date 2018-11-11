//**************************************************************************************************
//**************************************************************************************************
void Player(){
	//Gestiona la perrita.
	byte bContador=0;  //Tambien la usamos para indicar espejo.
	byte bContador2=0;
	byte bSpritePerrita1=255; //Sprite de la perrita que toca pintar. 255=ninguno.
	byte bSpritePerrita2=255;
	byte bSpriteMirrorPerrita1=255; //SpriteMirror de la perrita que toca pintar.
	byte bSpriteMirrorPerrita2=255;
	byte bDireccionDisparo=0;
	int iDireccionDisparoX=0;

	//Actualizando camara (iCamX,iCamY) con respecto a las coordenadas del prota.
    if(iPerritaX-iCamX>cAnchoPantalla/2-8 and iCamX<iAnchoMapaEnCurso-cAnchoPantalla){
      iCamX++;
    }
    if(iPerritaX-iCamX<cAnchoPantalla/2-8 and iCamX>0){
      iCamX--;
    }
    if(iPerritaY-iCamY>cAltoPantalla/2-8 and iCamY<iAltoMapaEnCurso-cAltoPantalla){
      iCamY++;
    }
    if(iPerritaY-iCamY<cAltoPantalla/2-8 and iCamY>0){
      iCamY--;
    }

	//Compruebo si le han vencido.
  	if(bVidas==0) bEstadoPerritaCuandoPare=cVencida;


	//Manejo del prota, la perrita.
    
    //Si no pulsamos ningun boton la perrita esta parada.
    //Si no, mostramos el movimiento que toque.
    bEstadoPerrita=bEstadoPerritaCuandoPare;
	if(bVidas>0){
		if(arduboy.pressed(LEFT_BUTTON)and bEstadoPerrita!=cCavandoIzquierda and bEstadoPerrita!=cCavandoDerecha) {
		  iPerritaX--;
		  if(PlayerColision()){
			iPerritaX++;
		  }
		  bEstadoPerrita=cIzquierda;
		  bEstadoPerritaCuandoPare=cParadoIzquierda;
		}
		if(arduboy.pressed(RIGHT_BUTTON)and bEstadoPerrita!=cCavandoIzquierda and bEstadoPerrita!=cCavandoDerecha) {
		  iPerritaX++;
		  if(PlayerColision()){
			iPerritaX--;
		  }
		  bEstadoPerrita=cDerecha;
		  bEstadoPerritaCuandoPare=cParadoDerecha;
		}
		if(arduboy.pressed(UP_BUTTON)and bEstadoPerrita!=cCavandoIzquierda and bEstadoPerrita!=cCavandoDerecha) {
		   iPerritaY--;
		  if(PlayerColision()){
			iPerritaY++;
		  }
		  bEstadoPerrita=cArriba;
		  bEstadoPerritaCuandoPare=cParadoArriba;
		}
		if(arduboy.pressed(DOWN_BUTTON)and bEstadoPerrita!=cCavandoIzquierda and bEstadoPerrita!=cCavandoDerecha) {
		  iPerritaY++;
		  if(PlayerColision()){
			iPerritaY--;
		  }
		  bEstadoPerrita=cAbajo;
		  bEstadoPerritaCuandoPare=cParadoAbajo;
		}
		//A_BUTTON - EXCAVANDO. **********************************************
		if(arduboy.pressed(A_BUTTON)and (bEstadoPerrita==cParadoIzquierda or bEstadoPerrita==cParadoDerecha 
									or bEstadoPerrita==cParadoArriba or bEstadoPerrita==cParadoAbajo)) {
		  if(bEstadoPerritaCuandoPare==cParadoIzquierda or bEstadoPerritaCuandoPare==cParadoArriba){
			bEstadoPerrita=cCavandoIzquierda;
			bEstadoPerritaCuandoPare=cCavandoIzquierda;
		  }
		  else{
			bEstadoPerrita=cCavandoDerecha;
			bEstadoPerritaCuandoPare=cCavandoDerecha;
		  }
		}
		else{
		  if(!arduboy.pressed(A_BUTTON)and (bEstadoPerritaCuandoPare==cCavandoIzquierda or bEstadoPerritaCuandoPare==cCavandoDerecha)){
			if(bEstadoPerritaCuandoPare==cCavandoIzquierda){
			  bEstadoPerritaCuandoPare=cParadoIzquierda;
			}
			else{
			  bEstadoPerritaCuandoPare=cParadoDerecha;
			}
		  }
		}
		//B_BUTTON - DISPARANDO. **********************************************
		//¡OJO! Con una bala puede matar varios enemigos si están cerca.
		//Estudio coordenadas del disparo y direccion.
		if(arduboy.pressed(B_BUTTON) and bitRead(bPerritaBits,cNoVolverADisparar)==0 and bHuesos>0 and bDireccionDisparo==0) { //Dispara los huesos de uno en uno.
				//bSonido=cSonidoDisparo;
				Pita(100,10,400,10);
				if(bEstadoPerrita==cIzquierda or bEstadoPerrita==cParadoIzquierda){
				iDireccionDisparoX=iPerritaX;
				bDireccionDisparo=cIzquierda; //Unico tile que encaja el disparo del hueso.
				}
				else
				iDireccionDisparoX=iPerritaX+8; //Ajustar disparo del hueso a cabeza perrita.
				if(bEstadoPerrita==cDerecha or bEstadoPerrita==cParadoDerecha){
				bDireccionDisparo=cDerecha;
				}
				else
				if(bEstadoPerrita==cArriba or bEstadoPerrita==cParadoArriba){
				bDireccionDisparo=cArriba;
				}
				else
				if(bEstadoPerrita==cAbajo or bEstadoPerrita==cParadoAbajo){
				bDireccionDisparo=cAbajo;
				}
			bitSet(bPerritaBits,cNoVolverADisparar);
		}
		if (!arduboy.pressed(B_BUTTON) and bitRead(bPerritaBits,cNoVolverADisparar))
			bitClear(bPerritaBits,cNoVolverADisparar);
		//Espero a que suelte el boton para volver a disparar
	}
    
    if(bDireccionDisparo){
      //Han disparado, voy a ver si hay hueco para una nueva bala. Si no hay hueco, se ignora.
      bContador=0;
      while(bContador<cCantidadDisparos){
        if(aiDisparo[bContador*4+3]==0){ //Encontre un disparo libre {
        aiDisparo[bContador*4]=iDireccionDisparoX;
        aiDisparo[bContador*4+1]=iPerritaY+4;
        aiDisparo[bContador*4+2]=bDireccionDisparo;
        aiDisparo[bContador*4+3]=cDistanciaDisparo;
        bContador=cCantidadDisparos+1; //Lo he encontrado y salgo del bucle.
        bHuesos--;
        }else bContador++; //Sigo buscando.
      }
    }
     bDireccionDisparo=0; //Desactivo disparo ya procesado.

    //Comprobamos si hay balas por actualizar.
    for(bContador=0;bContador<cCantidadDisparos;bContador++){
      if(aiDisparo[bContador*4+3]>0){ //Hay una vala en movimiento, la actualizo.
        if(aiDisparo[bContador*4+2]==cDerecha)aiDisparo[bContador*4]+=cVelocidadDisparo;else
        if(aiDisparo[bContador*4+2]==cIzquierda)aiDisparo[bContador*4]-=cVelocidadDisparo;else
        if(aiDisparo[bContador*4+2]==cArriba)aiDisparo[bContador*4+1]-=cVelocidadDisparo;else
        if(aiDisparo[bContador*4+2]==cAbajo)aiDisparo[bContador*4+1]+=cVelocidadDisparo;
        aiDisparo[bContador*4+3]-=cVelocidadDisparo;
        SpriteMirror( aiDisparo[bContador*4]-4-iCamX, aiDisparo[bContador*4+1]-4-iCamY,Objetos8x8,Mascaras8x8,cBalaHueso,cMascaraBalaHueso,0,0);
        //Comprobamos colisiones con objetos no destruibles (Tiles, puertas, etc).
		if(ColisionGeneral8x8((aiDisparo[bContador*4]),(aiDisparo[bContador*4+1]))){
        aiExplosionDisparo[bContador*3]=aiDisparo[bContador*4]; //Coordenada X de la explosion.
        aiExplosionDisparo[bContador*3+1]=aiDisparo[bContador*4+1]; //Coordenada Y de la explosion.
        aiExplosionDisparo[bContador*3+2]=0; //Frame de la explosión. Si es 4==explosion terminada.
        aiDisparo[bContador*4+3]=0; //Desactivo la bala.
        }
        //Comprobamos colisiones con Avisperos.
        bContador2=0;
        while(bContador2<cCantidadAvisperos){
			if(abAvisperos[bContador2] and arduboy.collide((Rect){aiDisparo[bContador*4]-4,
					aiDisparo[bContador*4+1]-4,8,8},
					(Rect){pgm_read_byte(&aAvisperos[bContador2*2+(bMapaEnCurso-1)*cCantidadAvisperos*2])*8,
					pgm_read_byte(&aAvisperos[bContador2*2+1+(bMapaEnCurso-1)*cCantidadAvisperos*2])*8,8,8})){
				aiExplosionDisparo[bContador*3]=aiDisparo[bContador*4]; //Coordenada X de la explosion.
				aiExplosionDisparo[bContador*3+1]=aiDisparo[bContador*4+1]; //Coordenada Y de la explosion.
				aiExplosionDisparo[bContador*3+2]=0; //Frame de la explosión. Si es 4==explosion terminada.
				aiDisparo[bContador*4+3]=0; //Desactivo la bala.
				abAvisperos[bContador2]--; //Resto una vida.
				if(!abAvisperos[bContador2]){
					bEnemigos--;
					aiExplosionDisparo[(bContador+1)*3]=pgm_read_byte(&aAvisperos[bContador2*2+(bMapaEnCurso-1)*cCantidadAvisperos*2])*8+4; //Coordenada X de la explosion.
					aiExplosionDisparo[(bContador+1)*3+1]=pgm_read_byte(&aAvisperos[bContador2*2+1+(bMapaEnCurso-1)*cCantidadAvisperos*2])*8+4; //Coordenada Y de la explosion.
					aiExplosionDisparo[(bContador+1)*3+2]=0; //Frame de la explosión. Si es 4==explosion terminada.
				}
				bContador2=cCantidadAvisperos+1;//Salgo del bucle.
			}else bContador2++;
		  }
        //Comprobamos colisiones con avispas.
		if(ColisionAvispaBala(aiDisparo[bContador*4],aiDisparo[bContador*4+1]) or
		//Colision con arana gigante
		ColisionAranaBala(aiDisparo[bContador*4],aiDisparo[bContador*4+1])
		){
		
		//Explosionamos la bala.
        aiExplosionDisparo[bContador*3]=aiDisparo[bContador*4]; //Coordenada X de la explosion.
        aiExplosionDisparo[bContador*3+1]=aiDisparo[bContador*4+1]; //Coordenada Y de la explosion.
        aiExplosionDisparo[bContador*3+2]=0; //Frame de la explosión. Si es 4==explosion terminada.
        aiDisparo[bContador*4+3]=0; //Desactivo la bala.
        }
	}
    }
    //Pintamos explosiones de balas.
    for(bContador=0;bContador<cCantidadDisparos;bContador++){
      if(aiExplosionDisparo[bContador*3+2]<4){
        SpriteMirror( aiExplosionDisparo[bContador*3]-4-iCamX, aiExplosionDisparo[bContador*3+1]-4-iCamY,
        Objetos8x8,Mascaras8x8,cExplosionBalaHueso+aiExplosionDisparo[bContador*3+2],2+aiExplosionDisparo[bContador*3+2],0,0);
		beep1.tone(beep1.freq(40), 10);
        if(arduboy.everyXFrames(bVelocidadExplosion))
        aiExplosionDisparo[bContador*3+2]++;
      }
    }

	//Si hay inmunidad, parpadea.
	if(bitRead(bPerritaBits,cBitInmune) and arduboy.everyXFrames(3)){
		bContadorInmunidad++;
		if(bContadorInmunidad==30){
			bitClear(bPerritaBits,cBitInmune);
			bContadorInmunidad=0;
		}
		if(bitRead(bPerritaBits,cBitParpadeo))bitClear(bPerritaBits,cBitParpadeo);
			else bitSet(bPerritaBits,cBitParpadeo);
	}

	if(bitRead(bPerritaBits,cBitParpadeo)==0){
		switch (bEstadoPerrita){
			case cParadoDerecha:
				bSpriteMirrorPerrita1=4+PerritaFrame; //Frame
				bSpriteMirrorPerrita2=4; //Mascara
			break;
			case cParadoIzquierda:
				bSpritePerrita1=4+PerritaFrame;
				bSpritePerrita2=4;
			break;
			case cParadoArriba:
				bSpritePerrita1=6+PerritaFrame;
				bSpritePerrita2=5;
			break;
			case cParadoAbajo:
				bSpritePerrita1=8+PerritaFrame;
				bSpritePerrita2=5;
			break;
			case cArriba:
				if(PerritaFrame){
					bSpritePerrita1=3;
					bSpritePerrita2=bSpritePerrita1;
				}
				else{
					bSpriteMirrorPerrita1=3;
					bSpriteMirrorPerrita2=bSpriteMirrorPerrita1;
				}
			break;
			case cAbajo:
				if(PerritaFrame){
					bSpritePerrita1=2;
					bSpritePerrita2=bSpritePerrita1;
				}
				else{
					bSpriteMirrorPerrita1=2;
					bSpriteMirrorPerrita2=bSpriteMirrorPerrita1;
				}
			break;
			case cDerecha:
				bSpriteMirrorPerrita1=PerritaFrame;
				bSpriteMirrorPerrita2=bSpriteMirrorPerrita1;
			break;
			case cIzquierda:
				bSpritePerrita1=PerritaFrame;
				bSpritePerrita2=bSpritePerrita1;
			break;
			case cCavandoIzquierda:
				bSpritePerrita1=10+PerritaFrame;
				bSpritePerrita2=6;
				SpriteMirror( iPerritaX-iCamX+13, iPerritaY-iCamY-1, Perrita,PerritaMascaras, 12+PerritaFrame, 7,0,0);
				if(arduboy.everyXFrames(9)){
				Pita(40,10,40,10);
				}
			break;
			case cCavandoDerecha:
				bSpriteMirrorPerrita1=10+PerritaFrame;
				bSpriteMirrorPerrita2=6;
				SpriteMirror( iPerritaX-iCamX-13, iPerritaY-iCamY-1, Perrita,PerritaMascaras, 12+PerritaFrame, 7,1,0);
				if(arduboy.everyXFrames(9))	Pita(40,10,40,10);
			break;
			case cVencida:
				bSpritePerrita1=14;
				bSpritePerrita2=8;
			break;
		}//End switch
		if(bSpritePerrita1!=255){ //Pintamos
			bContador=0; //No hay espejo.
		}else{
			bContador=1;
			bSpritePerrita1=bSpriteMirrorPerrita1;
			bSpritePerrita2=bSpriteMirrorPerrita2;
		}
		SpriteMirror( iPerritaX-iCamX, iPerritaY-iCamY, Perrita,PerritaMascaras, bSpritePerrita1, bSpritePerrita2,bContador,0);
		bSpritePerrita1=255;
		bSpritePerrita2=255;
		bSpriteMirrorPerrita1=255;
		bSpriteMirrorPerrita2=255;

	} 

    if(arduboy.everyXFrames(10)){
      if(PerritaFrame==1){
        PerritaFrame=0;
      }
      else{
        PerritaFrame=1;
      }
    }
	
  //Gestion de animacion cuando encuentra balas hueso o llave. Aprobecho variables libres. ***************************************
	if(bitRead(bPerritaBits,cAnimacionHuesito)){
		bContador=cHuesito;
		bContador2=cMascaraHuesito;
	}else
	if(bitRead(bPerritaBits,cAnimacionLlave)){
		bContador=cLlave;
		bContador2=cMascaraLlave;
	}
	if(bAnimacionObjetoEncontrado<30){
		SpriteMirror( iPerritaX+8-iCamX,iPerritaY+8-iCamY-bAnimacionObjetoEncontrado,Objetos8x8,Mascaras8x8,
								bContador, bContador2,0,0);
		if(arduboy.everyXFrames(3)){
			Pita(80,10,500,10);
		}
		bAnimacionObjetoEncontrado+=2;
		}
	else{
		bitClear(bPerritaBits,cAnimacionHuesito);
		bitClear(bPerritaBits,cAnimacionLlave);
	}
}

//**************************************************************************************************
