#pragma once


void routine_FrameDifference(char* nomFichier1, char* nomFichier2, int seuil);
void routine_SigmaDelta_1step(uint8 **I0, uint8 **I1, uint8** E0, uint8** V1, uint8 **M1,  long nrl, long nrh, long ncl, long nch );
void routine_SigmaDelta_step0(uint8** I, uint8 **M, uint8 **V, long nrl, long nrh, long ncl, long nch);
