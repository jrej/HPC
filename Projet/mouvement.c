#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "nrdef.h"
#include "nrutil.h"

#define N 3
#define VMIN 2
#define VMAX 255 //V est entre 2 et 2^m-1 avec m le nombre de bits des donnees ici 8 => https://hal.inria.fr/hal-01130889/document

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
            V[i][j] = 10; //On le met a VMIN
        }
    }
}

void routine_SigmaDelta_1step(uint8 **I0, uint8 **I1, uint8** E0, uint8 **V1, uint8 **M1,  long nrl, long nrh, long ncl, long nch )
{
    uint8 **O0 = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **M0 = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **V0 = ui8matrix(nrl, nrh, ncl, nch);

    for(int i = nrl; i < nrh; i++ ) //Step1 Mt Estimation
    {
        for(int j = ncl; j < nch; j++)
        {
            if(M1[i][j] < I0[i][j])
                M0[i][j]  = M1[i][j] + 1;

            else if(M1[i][j] < I0[i][j])
                M0[i][j] = M1[i][j] - 1;

            else
                M0[i][j] = M1[i][j];


        }
    }


    for(int i = nrl; i < nrh; i++)//Step 2 difference Computation
    {
        for(int j = ncl; j < nch; j++)
        {
            O0[i][j] = abs(M0[i][j] - I0[i][j]);
        }
    }


    for(int i = nrl; i < nrh; i++)//Step 3 Update and clamping
    {
        for(int j = ncl; j < nch; j++)
        {
            if(V1[i][j] < N * O0[i][j])
                V0[i][j]  = V1[i][j] + 1;

            else if(V1[i][j] < N * O0[i][j])
                V0[i][j] = V1[i][j] - 1;

            else
                V0[i][j] = V1[i][j];

            V0[i][j] = max( min(V0[i][j], VMAX), VMIN);


        }
    }

    for(int i = nrl; i < nrh; i++)//Step 4 Et estimation
    {
        for(int j = ncl; j < nch; j++)
        {
            if(O0[i][j] < V0[i][j])
                E0[i][j] = 0;
            else
                E0[i][j] = 255;
        }
    }



}
