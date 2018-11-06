#include <stdio.h>
#include <stdlib.h>
#include "nrdef.h"
#include "nrutil.h"
#include "mouvement.h"
#include <dirent.h>
#define NBIMAGES 199



void copieNB(uint8 **src, uint8 **dst, long nrl, long nrh, long ncl, long nch);

void test_routine_sigmaDelta(char* nomFichier1, char* nomFichier2)
{
	/*DIR *d;
	struct dirent *dir;
	d = opendir("../hall");
	if(d)
		while ((dir = readdir(d)) != NULL)
			printf("%s\n", dir->d_name);

	closedir(d);*/
	

    //m[nrl..nrh][ncl..nch]
    long nrl, nrh, ncl, nch;
    uint8 **I0 =  LoadPGM_ui8matrix(nomFichier1, &nrl, &nrh, &ncl, &nch);
    uint8 **I1 =  LoadPGM_ui8matrix(nomFichier2, &nrl, &nrh, &ncl, &nch);
    uint8 **V0 =  ui8matrix(nrl, nrh, ncl, nch);
    uint8 **M0 =  ui8matrix(nrl, nrh, ncl, nch);
	uint8 **Ot = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **Mt = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **Vt = ui8matrix(nrl, nrh, ncl, nch);
	uint8 **Et = ui8matrix(nrl, nrh, ncl, nch);

    routine_SigmaDelta_step0(I0, M0, V0, nrl, nrh, ncl, nch);

    routine_SigmaDelta_1step(I0, I1, V0, Vt, M0, Mt, Ot, Et, nrl, nrh, ncl, nch);

    SavePGM_ui8matrix(Et, nrl, nrh, ncl, nch, "test2.pgm");
	
}




void test_routine_sigmaDelta_loop(char* nomDir)
{
    /*DIR *d;
    struct dirent *dir;
    d = opendir(nomDir);
    if(d)
        while ((dir = readdir(d)) != NULL)
            printf("%s\n", dir->d_name);

    closedir(d);*/
    
    //m[nrl..nrh][ncl..nch]
    long nrl, nrh, ncl, nch;

    char nomImage[30] = "";
    int i= 0;

    uint8 **It = NULL;
    uint8 **I0 = NULL;

    sprintf(nomImage, "../hall/hall%06d.pgm", i);
    I0 =  LoadPGM_ui8matrix(nomImage, &nrl, &nrh, &ncl, &nch);

    uint8 **V0 = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **M0 = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **Ot = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **Mt = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **Vt = ui8matrix(nrl, nrh, ncl, nch);
    uint8 **Et = ui8matrix(nrl, nrh, ncl, nch);



    routine_SigmaDelta_step0(I0, M0, V0, nrl, nrh, ncl, nch);

    for(i= 1; i <= 299; i++)
    {
        sprintf(nomImage, "../hall/hall%06d.pgm", i);
        printf("img: %s\n", nomImage);
        It =  LoadPGM_ui8matrix(nomImage, &nrl, &nrh, &ncl, &nch);

        routine_SigmaDelta_1step(I0, It, V0, Vt, M0, Mt, Ot, Et, nrl, nrh, ncl, nch);
    
        
        sprintf(nomImage, "../Save/hall%06d.pgm", i);
        SavePGM_ui8matrix(Et, nrl, nrh, ncl, nch, nomImage);

        copieNB(It, I0, nrl, nrh, ncl, nch);
        copieNB(Vt, V0, nrl, nrh, ncl, nch);
        copieNB(Mt, M0, nrl, nrh, ncl, nch);
    }

    
}



void copieNB(uint8 **src, uint8 **dst, long nrl, long nrh, long ncl, long nch){
    for(int i = nrl; i < nrh; i++){
        for(int j = ncl; j < nch; j++){
            dst[i][j] = src[i][j];
        }
    }
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
    uint8 **I0 = LoadPGM_ui8matrix("../hall/hall000000.pgm", &nrl, &nrh, &ncl, &nch);
    printf("I0 Loaded\n");

    rgb8 **res = rgb8matrix(nrl, nrh, ncl, nch);
    int nrow=nrh-nrl+1,ncol=nch-ncl+1;

    for(int i = 0; i <= NBIMAGES; i++)
    {
        printf("LOOOP \n");
        sprintf(nomImageLoad, "../hall/hall000%03d.pgm", i);//Image a t
        printf("IMAGE Loaded %s \n",nomImageLoad);
        MLoadPGM_ui8matrix(nomImageLoad, nrl, nrh, ncl, nch, I0);
        printf("CHargement de l'image %s dans nrl,nrh,ncl,nch\n", nomImageLoad);
        copieTab(I0, res, nrl, nrh, ncl, nch);
        printf("copie dan le tableau \n");
        sprintf(nomImageSave, "../Save/hall000%03d.ppm", i);
        printf("IMAGE Loaded %s \n",nomImageSave);

        SavePPM_rgb8matrix(res, nrl, nrh, ncl, nch, nomImageSave);
        printf("CHargement de l'image %s dans nrl,nrh,ncl,nch\n", nomImageSave);

    }

}


int main(int argc, char* argv[])
{

    //for(int i =0 ; i < 299;i++)
    //routine_FrameDifference("../hall/hall000063.pgm", "../hall/hall000064.pgm", atoi(argv[1]));
    //test_routine_sigmaDelta("../hall/hall000063.pgm", "../hall/hall000064.pgm" );
	//creerPPM();
    test_routine_sigmaDelta_loop("../hall");

    return 0;
}

