/* ---------------- */
/* --- matrix.h --- */
/* ---------------- */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

// --------------------------------
// calcul matriciel: BLAS2 et BLAS3
// --------------------------------

void dup_f32matrix_ij(float32 **A, float32 **B, int n);
void dup_f32matrix_ji(float32 **A, float32 **B, int n);

void trn_f32matrix_ij(float32 **A, float32 **B, int n);
void trn_f32matrix_ji(float32 **A, float32 **B, int n);

void add_f32matrix_ij(float32 **A, float32 **B, float32 **C, int n);
void add_f32matrix_ji(float32 **A, float32 **B, float32 **C, int n);

void mul_f32matrix_ijk(float32 **A, float32 **B, float32 **C, int n);
void mul_f32matrix_ikj(float32 **A, float32 **B, float32 **C, int n);
void mul_f32matrix_jik(float32 **A, float32 **B, float32 **C, int n);
void mul_f32matrix_jki(float32 **A, float32 **B, float32 **C, int n);
void mul_f32matrix_kij(float32 **A, float32 **B, float32 **C, int n);
void mul_f32matrix_kji(float32 **A, float32 **B, float32 **C, int n);

void accumulate_f32matrix_h (float32 **A, float32 *B, int n);
void accumulate_f32matrix_v (float32 **A, float32 *B, int n);
void accumulate_f32matrix_vh(float32 **A, float32 *B, int n);

// ----------
// stencil 2D
// ----------

void sum3_f32matrix_loop (float32 **X, float32 **Y, int n);
void sum3_f32matrix_array(float32 **X, float32 **Y, int n);
void sum3_f32matrix_reg  (float32 **X, float32 **Y, int n);
void sum3_f32matrix_rot  (float32 **X, float32 **Y, int n);
void sum3_f32matrix_red  (float32 **X, float32 **Y, int n);
void sum3_f32matrix_lu   (float32 **X, float32 **Y, int n);

void sum3x2_f32matrix_array   (float32 **X, float32 **Y, float32 **Z, int n);
void sum3x2_f32matrix_red     (float32 **X, float32 **Y, float32 **Z, int n);
void sum3x2_f32matrix_reg_pipe(float32 **X, float32 **Y, float32 **Z, int n);
void sum3x2_f32matrix_red_pipe(float32 **X, float32 **Y, float32 **Z, int n);

void sum3x2_f32matrix_red_pipe_inline(float32 **X, float32 **Y, float32 **Z, int n);
    
#ifdef __cplusplus
}
#endif

#endif // __MATRIX_H__