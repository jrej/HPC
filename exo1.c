#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>  
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 

char buffer0[128];
char buffer1[128];
int barriere0 = 0 ;
int barriere1 = 0 ;

void f0(){
	barriere0 = 1;
	strcpy(buffer0,"fonction 0");
	barriere0 = 0; 
	while(barriere1);
	printf("buffer 1  :: %s \n",buffer1);
		
	 
	

}
void f1(){
	barriere1 = 1;
	strcpy(buffer1,"fonction 1");
	barriere1 = 0; 
	while(barriere0);
	printf("buffer 0  :: %s \n",buffer0);
		
	 
	

}


int main(){

	pthread_t thread1;  /* thread variables */
	pthread_create (&thread1, NULL, (void *)&f1, NULL);
	f0();
	pthread_join(thread1, NULL);
	return 0;
	
	

	



}
