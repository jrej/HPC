#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"

#define N 3
#define VMIN 1
#define VMAX 254 //V est entre 2 et 2^m-1 avec m le nombre de bits des donnees ici 8 => https://hal.inria.fr/hal-01130889/document

int min(int a, int b){
    return a<b ? a:b;
}

int max(int a, int b){
    return a>b ? a:b;
}

void routine_FrameDifference(char* nomFichier1, char* nomFichier2, int seuil)
{
    printf("Frame Different\n");
    //m[nrl..nrh][ncl..nch]
    long nrl, nrh, ncl, nch;
    uint8 **I0 =  LoadPGM_ui8matrix(nomFichier1, &nrl, &nrh, &ncl, &nch);
    uint8 **I1 =  LoadPGM_ui8matrix(nomFichier2, &nrl, &nrh, &ncl, &nch);


    uint8 **E0 =  ui8matrix(nrl, nrh, ncl, nch);
    uint8 **O0 = ui8matrix(nrl, nrh, ncl, nch);
    for(int i = nrl; i < nrh; i++ )
    {
        for(int j = ncl; j < nch; j++)
        {
            O0[i][j] = abs(I1[i][j] - I0[i][j]);

        }
    }
    for(int i = nrl; i < nrh; i++ )
    {
        for(int j = ncl; j < nch; j++)
        {
            if(O0[i][j] < seuil)
                E0[i][j] = 0;
            else
                E0[i][j] = 255;

        }
    }

    SavePGM_ui8matrix(E0, nrl, nrh, ncl, nch, "test.pgm");
    free_ui8matrix(I0, nrl, nrh, ncl, nch);
    free_ui8matrix(I1, nrl, nrh, ncl, nch);
    free_ui8matrix(E0, nrl, nrh, ncl, nch);
    free_ui8matrix(O0, nrl, nrh, ncl, nch);
}

void routine_SigmaDelta_step0(uint8** I, uint8 **M, uint8 **V, long nrl, long nrh, long ncl, long nch)
{
    for(int i = nrl; i < nrh; i++ )
    {
        for(int j = ncl; j < nch; j++)
        {
            M[i][j] = I[i][j];
            V[i][j] = (uint8) VMIN;
        }
    }
}

void routine_SigmaDelta_1step(uint8 **It_1, uint8 **It, uint8 **Vt_1, uint8 **Vt, uint8 **Mt_1, uint8 **Mt, uint8 **Ot, uint8 **Et, long nrl, long nrh, long ncl, long nch)
{

	uint8 n = (uint8) N;
	uint8 vmin = (uint8) VMIN;
	uint8 vmax = (uint8) VMAX;

    for(int i = nrl; i < nrh; i++ ) //Step1 Mt Estimation
    {
        for(int j = ncl; j < nch; j++)
        {
            if(Mt_1[i][j] < It[i][j])
                Mt[i][j]  = Mt_1[i][j] + 1;

            else if(Mt_1[i][j] < It[i][j])
                Mt[i][j] = Mt_1[i][j] - 1;

            else
                Mt[i][j] = Mt_1[i][j];
        }
    }


    for(int i = nrl; i < nrh; i++)//Step 2 difference Computation
    {
        for(int j = ncl; j < nch; j++)
        {
            Ot[i][j] = abs(Mt[i][j] - It[i][j]);
        }
    }


    for(int i = nrl; i < nrh; i++)//Step 3 Update and clamping
    {
        for(int j = ncl; j < nch; j++)
        {
            if(Vt_1[i][j] < n * Ot[i][j])
                Vt[i][j]  = Vt_1[i][j] + 1;

            else if(Vt_1[i][j] < n * Ot[i][j])
                Vt[i][j] = Vt_1[i][j] - 1;

            else
                Vt[i][j] = Vt_1[i][j];

            Vt[i][j] = max( min(Vt[i][j], vmax), vmin);


        }
    }

    for(int i = nrl; i < nrh; i++)//Step 4 Et estimation
    {
        for(int j = ncl; j < nch; j++)
        {
            if(Ot[i][j] < Vt[i][j])
                Et[i][j] = 0;
            else
                Et[i][j] = 255;
        }
    }



}
