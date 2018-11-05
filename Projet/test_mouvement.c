#include <stdio.h>
#include <stdlib.h>
#include "nrdef.h"
#include "nrutil.h"
#include "mouvement.h"
#define NBIMAGES 199

void test_routine_sigmaDelta(char* nomFichier1, char* nomFichier2)
{
    //m[nrl..nrh][ncl..nch]
    long nrl, nrh, ncl, nch;
    uint8 **I0 =  LoadPGM_ui8matrix(nomFichier1, &nrl, &nrh, &ncl, &nch);
    uint8 **I1 =  LoadPGM_ui8matrix(nomFichier2, &nrl, &nrh, &ncl, &nch);
    uint8 **E0 =  ui8matrix(nrl, nrh, ncl, nch);
    uint8 **V1 =  ui8matrix(nrl, nrh, ncl, nch);
    uint8 **M1 =  ui8matrix(nrl, nrh, ncl, nch);

    routine_SigmaDelta_step0(I1, M1, V1, nrl, nrh, ncl, nch);
    routine_SigmaDelta_1step(I0, I1, E0, V1, M1, nrl, nrh, ncl, nch);

    SavePGM_ui8matrix(E0, nrl, nrh, ncl, nch, "test2.pgm");




}

void copieTab(uint8 **src, rgb8 **dst, long nrl, long nrh, long ncl, long nch){
	for(int i = nrl; i < nrh; i++){
		for(int j = ncl; j < nch; j++){
			dst[i][j].r = src[i][j];
			dst[i][j].b = src[i][j];
			dst[i][j].g = src[i][j];
		}
	}
}

void creerPPM(){
  printf("Creation PPM\n");
	char nomImageLoad[50];// = "car3/car_3";
    char nomImageSave[50];// = "car3Sigma/car_3"
    long nrl, nrh, ncl, nch;
    uint8 **I0 = LoadPGM_ui8matrix("/home/gregory/HPC/hall/hall000000.pgm", &nrl, &nrh, &ncl, &nch);
    printf("I0 Loaded\n");

    rgb8 **res = rgb8matrix(nrl, nrh, ncl, nch);
    int nrow=nrh-nrl+1,ncol=nch-ncl+1;

    for(int i = 0; i <= NBIMAGES; i++)
    {
        printf("LOOOP \n");
        sprintf(nomImageLoad, "/home/gregory/HPC/hall/hall000%03d.pgm", i);//Image a t
        printf("IMAGE Loaded %s \n",nomImageLoad);
        MLoadPGM_ui8matrix(nomImageLoad, nrl, nrh, ncl, nch, I0);
        printf("CHargement de l'image %s dans nrl,nrh,ncl,nch\n", nomImageLoad);
        copieTab(I0, res, nrl, nrh, ncl, nch);
        printf("copie dan le tableau \n");
        sprintf(nomImageSave, "/home/gregory/HPC/Save/hall000%03d.ppm", i);
        printf("IMAGE Loaded %s \n",nomImageSave);

        SavePPM_rgb8matrix(res, nrl, nrh, ncl, nch, nomImageSave);
        printf("CHargement de l'image %s dans nrl,nrh,ncl,nch\n", nomImageSave);

    }

}


int main(int argc, char* argv[])
{

    //for(int i =0 ; i < 299;i++)
    routine_FrameDifference("/home/gregory/HPC/hall/hall000063.pgm", "/home/gregory/HPC/hall/hall000064.pgm", atoi(argv[1]));
    test_routine_sigmaDelta("/home/gregory/HPC/hall/hall000063.pgm", "/home/gregory/HPC/hall/hall000064.pgm" );
	creerPPM();


    return 0;
}
