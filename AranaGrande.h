//**************************************************************************************************
AranaCreaAranitas()
//La arana gitante ataca creando aranitas.
{
	byte bContador=0;
	//Cada X frames suelta una arana.
    if(arduboy.everyXFrames(60)){
		for(bContador=0;bContador<cCantidadAvispas;bContador++){
			if(aiAvispas[bContador*2]==0){ //Avispa muerta, la uso.
				aiAvispas[bContador*2]=iAranaGrandeX+16;
				aiAvispas[bContador*2+1]=iAranaGrandeY+16;
				bEnemigos++;
				return;
			}
		}
	}
}
//**************************************************************************************************
PintaAranaGrande()
//Pinta la arana grande
{
	static byte bContador1=32;
	static byte bContador2=16;
	byte bAranaFrame;
	static byte bMovimiento=1; //1-Derecha/Izquierda.
	static byte bSentido=0; //0-Izquierda, 1-Derecha.
	
	bAranaFrame=PerritaFrame;
	
	if(bVidaArana==0)bAranaFrame=0; //Arana muerta.
	else{
		if(arduboy.collide((Rect){iAranaGrandeX+2,iAranaGrandeY+2,30,14},
		(Rect){iPerritaX+2,iPerritaY+2,14,14})and bitRead(bPerritaBits,cBitInmune)==0){
			bitSet(bPerritaBits,cBitInmune);
			if(bVidas>0) bVidas--;
		}
		AranaCreaAranitas();
	}

	
	SpriteMirror(iAranaGrandeX-iCamX, iAranaGrandeY-iCamY, AranaGrande, AranaGrandeMascara,
							bAranaFrame, bAranaFrame,0,0);
	SpriteMirror(iAranaGrandeX-iCamX+16, iAranaGrandeY-iCamY, AranaGrande, AranaGrandeMascara,
							bAranaFrame, bAranaFrame,1,0);
							
	//Muere la arana.
	if(bVidaArana==0 and aiExplosionDisparo[(cCantidadDisparos-1)*3+2]==4 and bContador1>=0 and bContador1<33){
		bVelocidadExplosion=1;
		//Explosion Esplosion.
		aiExplosionDisparo[(cCantidadDisparos-1)*3]=iAranaGrandeX+bContador1; //Coordenada X de la explosion.
		aiExplosionDisparo[(cCantidadDisparos-1)*3+1]=iAranaGrandeY+bContador2; //Coordenada Y de la explosion.
		aiExplosionDisparo[(cCantidadDisparos-1)*3+2]=0; //Frame de la explosión. Si es 4==explosion terminada.
		bContador2-=4;
		if(bContador2==0){
			bContador2=16;
			bContador1-=4;
		}
	}

	if(bContador1>33) bVelocidadExplosion=10;
	

	if(bVidaArana>0)
		//Dibuja una barra con la vida que le queda a la arana
		arduboy.fillRect(75,2,52,5,BLACK);
		arduboy.fillRect(76+50-bVidaArana/2,3,bVidaArana/2,3,WHITE);
		//Mueve la arana
		if(bSentido==0){
			iAranaGrandeX--;
			if(EsTileSolido(iAranaGrandeX,iAranaGrandeY))bSentido=1;
		}else{
			iAranaGrandeX++;
			if(EsTileSolido(iAranaGrandeX+32,iAranaGrandeY))bSentido=0;
		}


}
//**************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
ColisionAranaBala(int iX, int iY)
//Comprueba si ha chocado con una bala.
{
	if(arduboy.collide((Rect){iAranaGrandeX+2,iAranaGrandeY+2,30,14},
		(Rect){iX-2,iY-2,6,6}))if(bVidaArana>0){
		bVidaArana--;
		return true;
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
