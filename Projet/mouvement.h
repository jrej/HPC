#pragma once
#include "nrdef.h"

uint8 ** routine_FrameDifference(uint8 **It, uint8 **Itm1, uint8 **Et, long nrl,long nrh,long ncl,long nch, int seuil);


//void routine_FrameDifference(char* nomFichier1, char* nomFichier2, int seuil);

void routine_SigmaDelta_step0(uint8** I, uint8 **M, uint8 **V, long nrl, long nrh, long ncl, long nch);

void routine_SigmaDelta_1stepO(  uint8 ** restrict It, uint8 ** restrict Itm1, uint8** restrict Vt, uint8 ** restrict Vtm1, uint8** restrict Mt, uint8 ** restrict Mtm1, uint8 ** restrict Et,  long nrl, long nrh, long ncl, long nch );

void routine_SigmaDelta_1step(uint8 **It_1, uint8 **It, uint8 **Vt_1, uint8 **Vt, uint8 **Mt_1, uint8 **Mt, uint8 **Ot, uint8 **Et, long nrl, long nrh, long ncl, long nch);
