//**************************************************************************************************
#include "DatosMapas.h"
//**************************************************************************************************
#define MIRROR_HORIZONTAL 1
#define MIRROR_VERTICAL   2 

/**
 * draw a bitmap with an optional mask, optional mirroring and optional inverting
 *
 * This code is a combination of Arduboy2's Sprite::drawExternalMask() and Ardbitmap's drawBitmap()
 *
 * This method can accomplish the same effect as most of Sprite's methods:
 * drawOverwrite: pass in a mask of NULL
 * drawExternalMask: pass in a separate mask
 * drawSelfMasked: pass in the same pointer for bitmap and mask
 *
 * To mirror the sprite, pass in MIRROR_HORIZONTAL or MIRROR_VERTICAL as the mirror parameter.
 * To mirrow both ways at once, pass in MIRROR_HORIZONTAL | MIRROR_VERTICAL as the parameter
 *
 * setting invert to true causes pixels that would have been black to be white, and white pixels to be black
 */
void SpriteMirror(int16_t x, int16_t y, const uint8_t* bitmap, const uint8_t* mask, uint8_t frame, 
					  uint8_t maskFrame, uint8_t mirror, bool invert) {
//    if (bitmap == NULL)
//        return;

    uint8_t w = pgm_read_byte(bitmap++);
    uint8_t h = pgm_read_byte(bitmap++);

    // no need to draw at all if we're offscreen
    if (x + w <= 0 || x > WIDTH - 1 || y + h <= 0 || y > HEIGHT - 1)
        return;

    const boolean hasMask = mask != NULL;

    uint16_t frame_offset = (w * ( h / 8 + ( h % 8 == 0 ? 0 : 1)));

    if (frame >= 0) {
        mask += maskFrame * frame_offset;
        bitmap += frame * frame_offset;
        
    // xOffset technically doesn't need to be 16 bit but the math operations
    // are measurably faster if it is
    uint16_t xOffset, ofs;
    int8_t yOffset = abs(y) % 8;
    int8_t sRow = y / 8;
    uint8_t loop_h, start_h, rendered_width;

    if (y < 0 && yOffset > 0) {
        sRow--;
        yOffset = 8 - yOffset;
    }

    // if the left side of the render is offscreen skip those loops
    if (x < 0) {
        xOffset = abs(x);
    } else {
        xOffset = 0;
    }

    // if the right side of the render is offscreen skip those loops
    if (x + w > WIDTH - 1) {
        rendered_width = ((WIDTH - x) - xOffset);
    } else {
        rendered_width = (w - xOffset);
    }

    // if the top side of the render is offscreen skip those loops
    if (sRow < -1) {
        start_h = abs(sRow) - 1;
    } else {
        start_h = 0;
    }

    loop_h = h / 8 + (h % 8 > 0 ? 1 : 0); // divide, then round up

    // if (sRow + loop_h - 1 > (HEIGHT/8)-1)
    if (sRow + loop_h > (HEIGHT / 8)) {
        loop_h = (HEIGHT / 8) - sRow;
    }

    // prepare variables for loops later so we can compare with 0
    // instead of comparing two variables
    loop_h -= start_h;

    sRow += start_h;
    ofs = (sRow * WIDTH) + x + xOffset;

    const uint8_t *bofs = bitmap + (start_h * w) + xOffset;
    const uint8_t *mask_ofs = mask + (start_h * w) + xOffset;

    if (mirror & MIRROR_HORIZONTAL)  {
        bofs += rendered_width - 1;
        mask_ofs += rendered_width - 1;
        if (x < 0){
            bofs -= w - rendered_width;
            mask_ofs -= w - rendered_width;
        } else{
            bofs += w - rendered_width;
            mask_ofs += w - rendered_width;
        }
    }
    uint8_t data;
    uint8_t mul_amt = 1 << yOffset;
    uint16_t bitmap_data;
    uint16_t mask_data;

    // really if yOffset = 0 you have a faster case here that could be
    // optimized
    for (uint8_t a = 0; a < loop_h; a++) {
        for (uint8_t iCol = 0; iCol < rendered_width; iCol++) {
            data = pgm_read_byte(bofs);
            mask_data = hasMask ? pgm_read_byte(mask_ofs) : 0xFF;

            if (invert) {
                data = ~data & mask_data;
            }
            bitmap_data = data * mul_amt;
            mask_data = ~(mask_data * mul_amt);

            if (sRow >= 0) {
                data = Arduboy2Base::sBuffer[ofs];
                data &= (uint8_t)(mask_data);
                data |= (uint8_t)(bitmap_data);
                Arduboy2Base::sBuffer[ofs] = data;
            }

            if (yOffset != 0 && sRow < 7) {
                data = Arduboy2Base::sBuffer[ofs + WIDTH];
                data &= (*((unsigned char *) (&mask_data) + 1));
                data |= (*((unsigned char *) (&bitmap_data) + 1));
                Arduboy2Base::sBuffer[ofs + WIDTH] = data;
            }

            ofs++;

            if (mirror & MIRROR_HORIZONTAL) {
                bofs--;
                mask_ofs--;
            } else{
                bofs++;
                mask_ofs++;
            }
        }
        sRow++;

        if (mirror & MIRROR_HORIZONTAL) {
            bofs += w + rendered_width;
            mask_ofs += w + rendered_width;
        } else{
            bofs += w - rendered_width;
            mask_ofs += w - rendered_width;
        }
        ofs += WIDTH - rendered_width;
    }
}
}
//**************************************************************************************************
//**************************************************************************************************
CalculaTile(int iTileX, int iTileY)
//Devuelve el numero de tile al que pertenece las coordenadas.
//iX e iY son coordenadas tiles.
//Actualiza iDesplazamiento.
{
	int iDesplazamiento=0;
	int iMetaTileX=0;
	int iMetaTileY=0;
	byte bMetaTile=0;
	
	//Calcula Desplazamiento.
	iDesplazamiento=pgm_read_byte(&aDesplazamientos[pgm_read_byte(&aDesplazamientosInit[bMapaEnCurso-1])+(iTileY/4)]);
	iTileX=(iTileX-iDesplazamiento);
	if(iTileX<0)iTileX=iTileX+iAnchoMapaEnCurso/8; //Si se sale por la izquierda.
	if(iTileX>=iAnchoMapaEnCurso/8)iTileX=iTileX-(iAnchoMapaEnCurso/8); //Si se sale por la derecha.
	//Compruebo si esta en la mitad original o en la espejada.
	if(iTileX<(iAnchoMapaEnCurso/8)/2) 
		iMetaTileX=iTileX/4;
	else{ 
		iTileX=(((iAnchoMapaEnCurso/8)/2)-(iTileX-((iAnchoMapaEnCurso/8)/2)))-1;
		iMetaTileX=iTileX/4;
	}
	iMetaTileY=iTileY/4;
	//Calculo Metatile al que pertenece.
	bMetaTile=pgm_read_byte(&Mapas[iMetaTileX+iMetaTileY*iAnchoMapaEnCurso/8/2/4+pgm_read_byte(&MapComienzo[bMapaEnCurso-1])]);
	if(bMetaTile>127) bMetaTile-=128; //Tiene desplazamiento. Se lo quito.
	if(bMetaTile==0) return 0; //Tile vacio.
	else
		//Calculo Tile exacto dentro del metatile. Aprobecho la variable.
		bMetaTile=pgm_read_byte(&MetaTiles[(bMetaTile-1)*16+(iTileX%4+(iTileY%4)*4)]);
	return bMetaTile;
}
//**************************************************************************************************
boolean EsTileSolido(int iX, int iY)
//Devuelve True si el tile en la coordenada x,y absoluta del mapa es solido y no se puede atravesar.
//iX e iY son coordenadas pixeles.
{
	int bTile=0;

	//Convierto las coordenadas pixel en coordenadas tile.
	iX=(iX)/8;
	iY=(iY)/8;
	//Calculo Tile.
	bTile=CalculaTile(iX,iY);
	if(bTile>=cFirstSolidTile and bTile<=cLastSolidTile) return true;
	if(bMapaEnCurso==2){ //Compruebo Arboles
		if((iX==cArbol1X or iX==cArbol1X+1) and (iY==cArbol1Y or iY==cArbol1Y+1)) return true;
		if((iX==cArbol2X or iX==cArbol2X+1) and (iY==cArbol2Y or iY==cArbol2Y+1)) return true;
		if((iX==cArbol3X or iX==cArbol3X+1) and (iY==cArbol3Y or iY==cArbol3Y+1)) return true;
	}
	return false;
}
//**************************************************************************************************
//**************************************************************************************************
PlayerColision()
//Calcula si el prota choca con alguna de sus esquinas. iPerritaX, iPerritaY.
{
	byte bContador=0;

//	return false;
	//Colision con tiles.
	if((EsTileSolido(iPerritaX+4,iPerritaY+4))or
		(EsTileSolido(iPerritaX+12,iPerritaY+4))or
		(EsTileSolido(iPerritaX+4,iPerritaY+12))or
		(EsTileSolido(iPerritaX+12,iPerritaY+12)))return true;
	//Compruebo piedras
	if(bMapaEnCurso==2){//Segundo mapa
//		if((iPerritaX==cArbol1X or iPerritaX==cArbol1X+1) and (iPerritaY==cArbol1Y or iPerritaY==cArbol1Y+1))return true;
//		if((iPerritaX==cArbol2X or iPerritaX==cArbol2X+1) and (iPerritaY==cArbol2Y or iPerritaY==cArbol2Y+1))return true;
//		if((iPerritaX==cArbol3X or iPerritaX==cArbol3X+1) and (iPerritaY==cArbol3Y or iPerritaY==cArbol3Y+1))return true;
	
		//Compruebo puertas.
		for(bContador=2;bContador<=7;bContador++)
			if(arduboy.collide((Rect){pgm_read_byte(&aPuertas[(bContador-1)*2])*8+3,pgm_read_byte(&aPuertas[(bContador-1)*2+1])*8+3,12,12},
				(Rect){iPerritaX+2,iPerritaY+2,14,14})and(!bitRead(PuertasBit,bContador-2)))return true;			
	}
	return false;
}
//**************************************************************************************************
Colision8x8(int iX, int iY)
//Calcula si un objeto de 8x8 choca con algun tile solido.
{
  if((EsTileSolido(iX+2,iY+2))or
    (EsTileSolido(iX+4,iY+2))or
    (EsTileSolido(iX+2,iY+4))or
    (EsTileSolido(iX+4,iY+4))) return true;
    return false;
}
//**************************************************************************************************
ColisionGeneral8x8(int iX, int iY)
//Devuelve si en el tile iX, iY hay algun obstaculo.
{
	byte bContador=0;
	iX-=4;
	iY-=2;

	//Choque con un tile solido.
	if(Colision8x8(iX,iY)) return true;
	if(bMapaEnCurso==2){
		//Choque con piedras.
		if(arduboy.collide((Rect){cArbol1X*8+1,cArbol1Y*8+1,14,14},
			(Rect){iX,iY,8,8})) return true;
		if(arduboy.collide((Rect){cArbol2X*8+1,cArbol2Y*8+1,14,14},
			(Rect){iX,iY,8,8})) return true;
		if(arduboy.collide((Rect){cArbol3X*8+1,cArbol3Y*8+1,14,14},
			(Rect){iX,iY,8,8})) return true;
		//Choque con piedra y puerta especiales.
		if(bEnemigos and arduboy.collide((Rect){cArbol3X*8+1,cArbol3Y*8+1,14,14},
				(Rect){iX,iY,8,8})) return true;
		
		//Choque con puerta cerrada
		for(bContador=2;bContador<=7;bContador++)
			if(arduboy.collide((Rect){pgm_read_byte(&aPuertas[(bContador-1)*2])*8+1,pgm_read_byte(&aPuertas[(bContador-1)*2+1])*8+1,13,13},
				(Rect){iX,iY,4,4})and(!bitRead(PuertasBit,bContador-2)))return true;
	}
	return false;
	
}
//**************************************************************************************************
//Colisiones.
//**************************************************************************************************
ColisionAvispas(int iX, int iY, byte bX, byte bY)
//Comprueba si se ha colisionado con alguna avispa.
//iX, iY: Coordenadas de inicio.
//bX, bY: Tamaño del cuadrado.
{
	byte bContador=0;
	
	for(bContador=0;bContador<cCantidadAvispas;bContador++){
		if((aiAvispas[bContador*2]>0) and  //Avispa viva, compruebo
		  (arduboy.collide((Rect){aiAvispas[bContador*2],aiAvispas[bContador*2+1],8,8},
		  (Rect){iX,iY,bX,bY}))) return true;  //Colisiona con avispa.
	}
	return false;
}
//**************************************************************************************************
ColisionAvisperos(int iX, int iY, byte bX, byte bY)
//Comprueba si se ha colisionado con algun avispero.
//iX, iY: Coordenadas de inicio.
//bX, bY: Tamaño del cuadrado.
{
	byte bContador=0;

	for(bContador=(bMapaEnCurso-1)*cCantidadAvisperos;bContador<cCantidadAvisperos+(bMapaEnCurso-1)*cCantidadAvisperos;bContador++){
		if(abAvisperos[bContador] and //Compruebo que este vivo.
		   arduboy.collide((Rect){pgm_read_byte(&aAvisperos[bContador*2+(bMapaEnCurso-1)*cCantidadAvisperos*2])*8,
		   pgm_read_byte(&aAvisperos[bContador*2+1+(bMapaEnCurso-1)*cCantidadAvisperos*2])*8,8,8},
		   (Rect){iX,iY,bX,bY})) return true;  //Colisiona con avispero.
		
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

