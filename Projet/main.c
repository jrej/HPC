#include <stdio.h>
#include <stdlib.h>
#include "mouvement.h"
#include "test_mouvement.h"
#include "test_mouvement_SIMD.h"
#define NBIMAGES 299
#include "nrdef.h"
#include "nrutil.h"
#include "matrice_roc.h"
#include "morpho.h"
#include "test_morpho.h"



void difference2Images()
{

    long nrl, nrh, ncl, nch;
    uint8 **ItImage1 = LoadPGM_ui8matrix("../hall/hall000000.pgm", &nrl, &nrh, &ncl, &nch);
    uint8 **ItImage2 =  ui8matrix(nrl, nrh, ncl, nch);;
    uint8 **ItNormal = ui8matrix(nrl, nrh, ncl, nch);
    char nomImage1[100];//Pour initialiser nrl, etc
    char nonImage2[100];
    char nomImageLoadNormal[100];


    for(int i = 1; i <= NBIMAGES; i++)
    {
       sprintf(nomImage1,"../hallSigma/hall%06d.pgm",i);//Image a t
        sprintf(nonImage2,"../hallSigmaSIMD/hall%06d.pgm",i);//Image a t
        sprintf(nomImageLoadNormal,"../hall/hall%06d.pgm",i);//Image a t
        sprintf(nomImage1,"../hallFrame3x3O/hall%06d.pgm",i);//Image a t
        sprintf(nonImage2,"../hallFrame3x3O_pipe/hall%06d.pgm",i);//Image a t
        sprintf(nomImageLoadNormal,"../hall/hall%06d.pgm",i);//Image a t

        MLoadPGM_ui8matrix(nomImage1, nrl, nrh, ncl, nch, ItImage1);
        MLoadPGM_ui8matrix(nonImage2, nrl, nrh, ncl, nch, ItImage2);
        MLoadPGM_ui8matrix(nomImageLoadNormal, nrl, nrh, ncl, nch, ItNormal);

        for(int k = nrl; k <= nrh; k++)
        {
            for(int l = ncl; l <= nch; l++)
            {
                if(ItImage1[k][l] != ItImage2[k][l])
                {
                    printf("Probleme ! i = %d, j = %d, ItImage2 = %d, image = %d\n",k, l, ItImage2[k][l], i);
                }
            }
        }

    }

}

#define OPTI 3 //1 pour optimisation 2 sans optimisation 3 pour tout 0 pour rien


int main(int argc, char* argv[])
{
#if OPTI & 0x1
    printf("OPTI 1  test_routine_FrameDifference_SIMD\n");
    test_routine_FrameDifference_SSE2(20);
    printf("OPTI 1  test_routine_sigmaDelta_SIMD\n");
    test_routine_sigmaDelta_SSE2();
    printf("OPTI 1  test_routine_FrameDifference_SIMDM\n");

    test_routine_FrameDifference_SSE2M(20);

#endif
#if OPTI & 0x2

    //test_routine_FrameDifference(20);
    printf("OPTI 2  test_routine_sigmaDelta\n");

    //test_routine_sigmaDelta();
    test_routine_sigmaDelta_loop("../hall");
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3ouverture\n");
    test_routine_FrameDifferenceMorpho3x3ouverture(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3fermeture\n");
    test_routine_FrameDifferenceMorpho3x3fermeture(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3ouvertureFermeture\n");
    test_routine_FrameDifferenceMorpho3x3ouvertureFermeture(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3fermetureouverture\n");

    test_routine_FrameDifferenceMorpho3x3fermetureOuverture(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3fermetureFermeture\n");
    test_routine_FrameDifferenceMorpho3x3fermeturefermeture(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3ouverture_pipe\n");
    test_routine_FrameDifferenceMorpho3x3ouverture_pipe(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3fermeture_pipe\n");
    test_routine_FrameDifferenceMorpho3x3fermeture_pipe(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3ouverture_bin\n");
    test_routine_FrameDifferenceMorpho3x3ouverture_bin(20);
    printf("OPTI 2  test_routine_FrameDifferenceMorpho3x3ouverture\n");

    test_routine_FrameDifferenceMorpho3x3fermeture_bin(20);


#endif

    //creation_matrices_ROC("../hall");
  //difference2Images();

    return 0;
}