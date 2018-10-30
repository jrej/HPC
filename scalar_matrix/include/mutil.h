/* --------------- */
/* --- mutil.h --- */
/* --------------- */

/*
 * Misc util
 */


#ifndef __MUTIL_H__
#define __MUTIL_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif  

#define RUN 5
#define FREQ 2.5e9

double chrono(void);
    
uint8 ui8rand(void);
uint32 ui32rand(void);
uint32 f32rand(void);

void rand_ui8vector(uint8 *X, int i0, int i1);
void rand_ui32vector(uint32 *X, int i0, int i1);
void rand_f32vector(float32 *X, int i0, int i1);

int getIter(int size);
int getIterAV(int size);

float32 gauss(float32 sigma, float32 x);

double cpp1(double cycles, double n);
double cpp2(double cycles, double n);
double cpp3(double cycles, double n);
double opp(void);
double MFlops(double dt, double n);
double GFlops(double dt, double n);

#ifdef __cplusplus
}
#endif

#endif /* __MUTIL_H__ */
