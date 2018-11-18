#include <stdio.h>
#include <stdlib.h>

#include "nrdef.h"
#include "nrutil.h"
#include "matrice_roc.h"

#define IMAGEMIN 10
#define IMAGEMAX 290

void matrice_ROC(char *dirPATH)
{
	long nrl, nrh, ncl, nch;
	char nomImageV[100];
    char nomImageT[100];

	uint8 **ImageVerite =  LoadPGM_ui8matrix("../hall/hall000000.pgm", &nrl, &nrh, &ncl, &nch);
	uint8 **ImageTest = ui8matrix(nrl, nrh, ncl, nch);
	int matriceRoc[2][2] = {0};

	for(int k = IMAGEMIN; k <= IMAGEMAX; k+= 10)
	{
        sprintf(nomImageV, "../verite/hall000%03d.pgm", k);
        sprintf(nomImageT, "%s/hall000%03d.pgm", dirPATH, k);

        ImageVerite= LoadPGM_ui8matrix(nomImageV, &nrl, &nrh, &ncl, &nch);
        ImageTest= LoadPGM_ui8matrix(nomImageT, &nrl, &nrh, &ncl, &nch);

        //MLoadPGM_ui8matrix(nomImageV, nrl, nrh, ncl, nch, ImageVerite);
        //MLoadPGM_ui8matrix(nomImageT, nrl, nrh, ncl, nch, ImageTest);


        for(int i = nrl; i <= nrh; i++)
        {
        	for(int j = ncl; j <= nch; j++)
        	{
        		if(ImageTest[i][j] == 255 && ImageVerite[i][j] == 255)
                    matriceRoc[0][0]+= 1; //VP
                else if(ImageTest[i][j] == 0 && ImageVerite[i][j] == 255)
                    matriceRoc[0][1]+= 1; //FN
                else if(ImageTest[i][j] == 255 && ImageVerite[i][j] == 0)
                    matriceRoc[1][0]+= 1; //FP
                else if(ImageTest[i][j] == 0 && ImageVerite[i][j] == 0)
                    matriceRoc[1][1]+= 1; //VN
            }
        }
    }


    double rapport = 1.0*(matriceRoc[0][0] + matriceRoc[1][1])  /  (matriceRoc[0][1] + matriceRoc[1][0]);
    
    printf("Matrice ROC = \n%d %d\n%d %d\n", matriceRoc[0][0], matriceRoc[0][1], matriceRoc[1][0], matriceRoc[1][1]);
    printf("Rapport = %d/%d = %f\n", matriceRoc[1][1]+matriceRoc[0][0], matriceRoc[0][1]+matriceRoc[1][0], rapport);
    

    free_ui8matrix(ImageVerite, nrl, nrh, ncl, nch);
    free_ui8matrix(ImageTest, nrl, nrh, ncl, nch);
}
