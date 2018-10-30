/* --------------------- */
/* --- test_matrix.c --- */
/* --------------------- */

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "nrdef.h"
#include "nrutil.h"

#include "mymacro.h"

//static inline double rdtsc(void)
/*double rdtsc(void)
{
    unsigned int eax, edx;
    unsigned long cycles;

    asm volatile("rdtsc\n\t", "=a" (eax), "=d" (edx));
    cycles = (uint64) eax | (uint64)edx << 32;
    return (double) cycles;
}*/

#include "matrix.h"
#include <ia32intrin.h> // icc
//#include <x86intrin.h> // gcc

/* ---------------------------------------- */
void disp(char *name, double cycles, double n)
/* ---------------------------------------- */
{
    printf("%s: %10f.2\n", name, cycles/(n*n*n));
}
// -------------------------
void main_test_matrix(int n)
// -------------------------
{
    float32 **T, **X, **Y, **Z, *V;

    
    char *format_f32 = "%4.0f ";
    char *format     = "%6.2f ";
    
    int iter, niter = 2;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //
    
    printf("========================================\n");
    printf("n = %d\n", n);
    
    // ------------------- //
    // -- allocation 2D -- //
    // ------------------- //
    
    
    X = f32matrix(0, n-1, 0, n-1);
    Y = f32matrix(0, n-1, 0, n-1);
    Z = f32matrix(0, n-1, 0, n-1);
    
    T = f32matrix(0-1, n-1+1, 0-1, n-1+1); // matrice avec bords
    
    V = f32vector(0, n-1);
    // ---------- //
    // -- init -- //
    // ---------- //
    
    zero_f32matrix(T, 0-1, n-1+1, 0-1, n-1+1);
    zero_f32matrix(Z, 0, n-1, 0, n-1);
    zero_f32vector(V, 0, n-1);
    
    set_f32matrix_param(X, 0, n-1, 0, n-1,  1, 1,  1);
    set_f32matrix_param(Y, 0, n-1, 0, n-1,  1, 1,  1);
    set_f32matrix_param(T, 0, n-1, 0, n-1,  1, 1,  1);
    
    // -------------------- //
    // -- affichage init -- //
    // -------------------- //
 
    DEBUG(display_f32matrix(X, 0, n-1, 0, n-1, format_f32, "X"));
    DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y"));
    DEBUG(display_f32matrix(T, 0, n-1, 0, n-1, format_f32, "T"));
    
    puts("----------------");
    puts("-- dup: Z = X --");
    puts("----------------");
    
    CHRONO(dup_f32matrix_ij(X, Z, n),cycles); printf("dup  ij: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(dup_f32matrix_ji(X, Z, n),cycles); printf("dup  ji: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    
    puts("--------------------");
    puts("-- add: Z = X + Y --");
    puts("--------------------");
    
    CHRONO(add_f32matrix_ij(X, Y, Z, n),cycles); printf("add  ij: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(add_f32matrix_ji(X, Y, Z, n),cycles); printf("add  ji: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));

accumulate:
    puts("----------------------");
    puts("-- acc: V = proj(X) --");
    puts("----------------------");
    
    CHRONO(accumulate_f32matrix_h (X, V, n),cycles); printf("acc  h : "); DEBUG(display_f32vector(V, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(accumulate_f32matrix_v (X, V, n),cycles); printf("acc  v : "); DEBUG(display_f32vector(V, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(accumulate_f32matrix_vh(X, V, n),cycles); printf("acc  vh: "); DEBUG(display_f32vector(V, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    //goto end;
    
    puts("--------------------");
    puts("-- mul: Z = X * Y --");
    puts("--------------------");
    
    CHRONO(mul_f32matrix_ijk(X, Y, Z, n),cycles); printf("ijk: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n*n))); puts(""); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(mul_f32matrix_ikj(X, Y, Z, n),cycles); printf("ikj: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n*n))); puts(""); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(mul_f32matrix_jik(X, Y, Z, n),cycles); printf("jik: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n*n))); puts(""); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(mul_f32matrix_jki(X, Y, Z, n),cycles); printf("jki: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n*n))); puts(""); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(mul_f32matrix_kij(X, Y, Z, n),cycles); printf("kij: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n*n))); puts(""); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(mul_f32matrix_kji(X, Y, Z, n),cycles); printf("kji: "); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n*n))); puts(""); zero_f32matrix(Z, 0, n-1, 0, n-1);
    
end:
    // ---------- //
    // -- free -- //
    // ---------- //
    
    free_f32matrix(X, 0, n-1, 0, n-1);
    free_f32matrix(Y, 0, n-1, 0, n-1);
    free_f32matrix(Z, 0, n-1, 0, n-1);
    
    free_f32matrix(T, 0-1, n-1+1, 0-1, n-1+1);
}
// -------------------------
void main_test_stencil(int n)
// -------------------------
{
    float32 **X, **Y, **Z;
    
    
    char *format_f32 = "%4.0f ";
    char *format     = "%6.2f ";
    
    int b = 1; // bord
    int iter, niter = 2;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;
    
    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //
    
    printf("========================================\n");
    printf("n = %d\n", n);
    
    // ------------------- //
    // -- allocation 2D -- //
    // ------------------- //
    
    X = f32matrix(0-1, n-1+1, 0-1, n-1+1); // matrice avec bords
    Y = f32matrix(0-1, n-1+1, 0-1, n-1+1); // matrice avec bords
    Z = f32matrix(0,   n-1,   0,   n-1);   // matrice sans bord
    
    // ---------- //
    // -- init -- //
    // ---------- //
    
    zero_f32matrix(X, 0-b, n-1+b, 0-b, n-1+b);
    zero_f32matrix(Y, 0-b, n-1+b, 0-b, n-1+b);
    zero_f32matrix(Z, 0, n-1, 0, n-1);
    
    set_f32matrix_param(X, 0, n-1, 0, n-1,  1, 1,  1);
    set_f32matrix_param(Y, 0, n-1, 0, n-1,  1, 1,  1);
    
    // -------------------- //
    // -- affichage init -- //
    // -------------------- //
    
    DEBUG(display_f32matrix(X, 0, n-1, 0, n-1, format_f32, "X"));
    DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y"));
    DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Z"));
    
    puts("-------------------------");
    puts("-- sum: Y = sum 3x3(X) --");
    puts("-------------------------");
    
    CHRONO(sum3_f32matrix_loop (X, Y, n),cycles); printf("sum3  Loop : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1);
    CHRONO(sum3_f32matrix_array(X, Y, n),cycles); printf("sum3  Array: "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1);
    CHRONO(sum3_f32matrix_reg  (X, Y, n),cycles); printf("sum3  Reg  : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1);
    CHRONO(sum3_f32matrix_rot  (X, Y, n),cycles); printf("sum3  Rot  : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1);
    CHRONO(sum3_f32matrix_red  (X, Y, n),cycles); printf("sum3  Red  : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1);
    CHRONO(sum3_f32matrix_lu   (X, Y, n),cycles); printf("sum3  LU   : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1);
    
    puts("---------------------------------------");
    puts("-- sum: Y = sum 3x3(X), Z=sum 3x3(Y) --");
    puts("---------------------------------------");
    
    CHRONO(sum3x2_f32matrix_array(X, Y, Z, n),cycles);           printf("sum3x2  Array    : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Z ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(sum3x2_f32matrix_red  (X, Y, Z, n),cycles);           printf("sum3x2  Red      : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Z ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(sum3x2_f32matrix_reg_pipe(X, Y, Z, n),cycles);        printf("sum3x2  Reg+Pipe : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Z ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(sum3x2_f32matrix_red_pipe(X, Y, Z, n),cycles);        printf("sum3x2  Red+Pipe : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Z ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1); zero_f32matrix(Z, 0, n-1, 0, n-1);
    CHRONO(sum3x2_f32matrix_red_pipe_inline(X, Y, Z, n),cycles); printf("sum3x2  Red+Pipe+Inline : "); DEBUG(display_f32matrix(Y, 0, n-1, 0, n-1, format_f32, "Y ")); DEBUG(display_f32matrix(Z, 0, n-1, 0, n-1, format_f32, "Z ")); BENCH(printf(format, cycles/(n*n))); puts(""); zero_f32matrix(Y, 0, n-1, 0, n-1); zero_f32matrix(Z, 0, n-1, 0, n-1);

end:
    // ---------- //
    // -- free -- //
    // ---------- //
    puts("free ...");
    
    free_f32matrix(X, 0-b, n-1+b, 0-b, n-1+b);
    free_f32matrix(Y, 0-b, n-1+b, 0-b, n-1+b);
    free_f32matrix(Z, 0,   n-1,   0,   n-1);
}
/* ================================== */
int main_matrix(int argc, char * argv[])
/* ================================== */
{
    puts("main_matrix");

    DEBUG(main_test_matrix(4));
    //DEBUG(main_test_matrix(5));
    //DEBUG(main_test_matrix(6));
    //DEBUG(main_test_matrix(7));
    DEBUG(main_test_matrix(10));
    
    //BENCH(main_test_matrix(128));
    //BENCH(main_test_matrix(256));
    BENCH(main_test_matrix(512));
    //BENCH(main_test_matrix(1024));
    //BENCH(main_test_matrix(2048));
    //BENCH(main_test_matrix(4096));
    //BENCH(main_test_matrix(8000));
    
    return 0;
}
/* =================================== */
int main_stencil(int argc, char * argv[])
/* =================================== */
{
    puts("main_stencil");
    
    DEBUG(main_test_stencil(4));
    //DEBUG(main_test_stencil(5));
    //DEBUG(main_test_stencil(6));
    //DEBUG(main_test_stencil(7));
    DEBUG(main_test_stencil(10));
    
    //BENCH(main_test_stencil(128));
    //BENCH(main_test_stencil(256));
    //BENCH(main_test_stencil(512));
    BENCH(main_test_stencil(1024));
    //BENCH(main_test_stencil(2048));
    //BENCH(main_test_stencil(4096));
    //BENCH(main_test_stencil(8000));
    
    return 0;
}
