#include <stdio.h>
#include <stdlib.h>
#include "nrdef.h"
#include "nrutil.h"
#include "mouvement.h"
#include"mymacro.h"
#include <dirent.h>
#define NBIMAGES 199



void copieNB(uint8 **src, uint8 **dst, long nrl, long nrh, long ncl, long nch);


//fonction non optimisé
void test_routine_frameDiff_loop(int seuil)
{
  /////////////// Pour le cycle par point////////////
      double cycles;

      char *format = "%6.2f \n";
      double cycleTotal = 0;
      int iter, niter = 2;
      int run, nrun = 5;
      double t0, t1, dt, tmin, t;
      ///////////////////////////////////////////////
    char nomImage[50];
    char nomImage2[50];
    char nomImageSave[50];

    long nrl, nrh, ncl, nch;

    uint8 **I0 = LoadPGM_ui8matrix("../hall/hall000000.pgm", &nrl, &nrh, &ncl, &nch);
    uint8 **I1;

    uint8 **E0 =  ui8matrix(nrl, nrh, ncl, nch);
    uint8 **O0 = ui8matrix(nrl, nrh, ncl, nch);


    printf("Frame Different\n");

    for(int i= 1; i <= 299; i++)
    {
        sprintf(nomImage, "../hall/hall%06d.pgm", i-1);
        sprintf(nomImage2, "../hall/hall%06d.pgm", i);
      //  printf("img: %s\n", nomImage2);


        //m[nrl..nrh][ncl..nch]

        I0 =  LoadPGM_ui8matrix(nomImage, &nrl, &nrh, &ncl, &nch);
        I1 =  LoadPGM_ui8matrix(nomImage2, &nrl, &nrh, &ncl, &nch);



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

        sprintf(nomImageSave, "../Save/fd_hall%06d.pgm", i);
        SavePGM_ui8matrix(E0, nrl, nrh, ncl, nch, nomImageSave);

    }



    free_ui8matrix(I0, nrl, nrh, ncl, nch);
    free_ui8matrix(I1, nrl, nrh, ncl, nch);
    free_ui8matrix(E0, nrl, nrh, ncl, nch);
    free_ui8matrix(O0, nrl, nrh, ncl, nch);
}



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
  /////////////// Pour le cycle par point////////////
      double cycles;

      char *format = "%6.2f \n";
      double cycleTotal = 0;
      int iter, niter = 2;
      int run, nrun = 5;
      double t0, t1, dt, tmin, t;
      ///////////////////////////////////////////////

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


    CHRONO(routine_SigmaDelta_step0(I0, M0, V0, nrl, nrh, ncl, nch), cycles);
    cycleTotal+=cycles;

  //  printf("Sigma Delta\n");

    for(i= 1; i <= 299; i++)
    {
        sprintf(nomImage, "../hall/hall%06d.pgm", i);
      //  printf("img: %s\n", nomImage);
        It =  LoadPGM_ui8matrix(nomImage, &nrl, &nrh, &ncl, &nch);


        CHRONO(routine_SigmaDelta_1step(I0, It, V0, Vt, M0, Mt, Ot, Et, nrl, nrh, ncl, nch), cycles);
        cycleTotal+=cycles;
        sprintf(nomImage, "../Save/hall%06d.pgm", i);
        SavePGM_ui8matrix(Et, nrl, nrh, ncl, nch, nomImage);

        copieNB(It, I0, nrl, nrh, ncl, nch);
        copieNB(Vt, V0, nrl, nrh, ncl, nch);
        copieNB(Mt, M0, nrl, nrh, ncl, nch);
    }
    cycleTotal/=NBIMAGES;
    cycleTotal/=((nch+1)*(nrh+1));
  //  printf("cycle totaal %d  \n", cycleTotal);
    BENCH(printf("Cycles SigmaDelta = "));
    BENCH(printf(format, cycleTotal));


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
//  	printf("Creation PPM\n");
	char nomImageLoad[50];// = "car3/car_3";
    char nomImageSave[50];// = "car3Sigma/car_3"
    long nrl, nrh, ncl, nch;
    uint8 **I0 = LoadPGM_ui8matrix("../hall/hall000000.pgm", &nrl, &nrh, &ncl, &nch);
  //  printf("I0 Loaded\n");

    rgb8 **res = rgb8matrix(nrl, nrh, ncl, nch);
    int nrow=nrh-nrl+1,ncol=nch-ncl+1;

    for(int i = 0; i <= NBIMAGES; i++)
    {
        //printf("LOOOP \n");
        sprintf(nomImageLoad, "../hall/hall000%03d.pgm", i);//Image a t
      //  printf("IMAGE Loaded %s \n",nomImageLoad);
        MLoadPGM_ui8matrix(nomImageLoad, nrl, nrh, ncl, nch, I0);
      //  printf("CHargement de l'image %s dans nrl,nrh,ncl,nch\n", nomImageLoad);
        copieTab(I0, res, nrl, nrh, ncl, nch);
    //    printf("copie dan le tableau \n");
        sprintf(nomImageSave, "../Save/hall000%03d.ppm", i);
    //    printf("IMAGE Loaded %s \n",nomImageSave);

        SavePPM_rgb8matrix(res, nrl, nrh, ncl, nch, nomImageSave);
      //  printf("CHargement de l'image %s dans nrl,nrh,ncl,nch\n", nomImageSave);

    }

}
/*

int main(int argc, char* argv[])
{

    //for(int i =0 ; i < 299;i++)
    //routine_FrameDifference("../hall/hall000063.pgm", "../hall/hall000064.pgm", atoi(argv[1]));
    //test_routine_sigmaDelta("../hall/hall000063.pgm", "../hall/hall000064.pgm" );
	//creerPPM();

    test_routine_frameDiff_loop(atoi(argv[1]));
    test_routine_sigmaDelta_loop("../hall");

    return 0;
}

*/
