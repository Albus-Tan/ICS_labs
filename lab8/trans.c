// 520021910607 Tanziming

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k;
    int a0, a1, a2, a3, a4, a5, a6, a7;
    if(M == 32 && N == 32){
        // split as 8*8 size block
        for (i = 0; i < 32; i += 8) {
            for (j = 0; j < 32; j += 8) {
                for(k = i; k < i + 8; ++k){
                    // use local variables as temp cache
                    // to avoid diagonal conflict
                    a0 = A[k][j];
                    a1 = A[k][j+1];
                    a2 = A[k][j+2];
                    a3 = A[k][j+3];
                    a4 = A[k][j+4];
                    a5 = A[k][j+5];
                    a6 = A[k][j+6];
                    a7 = A[k][j+7];
                    B[j][k] = a0;
                    B[j+1][k] = a1;
                    B[j+2][k] = a2;
                    B[j+3][k] = a3;
                    B[j+4][k] = a4;
                    B[j+5][k] = a5;
                    B[j+6][k] = a6;
                    B[j+7][k] = a7;
                }
            }
        }
        return;
    }

    
    if(M == 64 && N == 64){
        // I first try split as 4*4 blocks, but not getting full mark
        // So I split as 8*8 size big block
        // futher, split each big block to 4 4*4 small blocks
        for (i = 0; i < 64; i += 8) {
            for (j = 0; j < 64; j += 8) {
                for(k = i; k < i + 4; ++k){

                    // use local variables as temp cache
                    // copy left up and right up block of A
                    a0 = A[k][j];
                    a1 = A[k][j+1];
                    a2 = A[k][j+2];
                    a3 = A[k][j+3];
                    a4 = A[k][j+4];
                    a5 = A[k][j+5];
                    a6 = A[k][j+6];
                    a7 = A[k][j+7];

                    // move left up part to B left up
                    B[j][k] = a0;
                    B[j+1][k] = a1;
                    B[j+2][k] = a2;
                    B[j+3][k] = a3;

                    // move right up part to B right up, which
                    // is incorrect temporarily, but make full use
                    // of its cache, comment says where it should be
                    B[j][k+4] = a4;  // B[j+4][k]
                    B[j+1][k+4] = a5;  // B[j+5][k]
                    B[j+2][k+4] = a6;  // B[j+6][k]
                    B[j+3][k+4] = a7;  // B[j+7][k]
                }

                for(k = j; k < j + 4; ++k){

                    // get right up part of B and store in local var (by row)
                    a0 = B[k][i+4];
                    a1 = B[k][i+5];
                    a2 = B[k][i+6];
                    a3 = B[k][i+7];

                    // get the left bottom part of A (by col)
                    a4 = A[i+4][k];
                    a5 = A[i+5][k];
                    a6 = A[i+6][k];
                    a7 = A[i+7][k];

                    // since upper part of B still in cache,
                    // put left bottom part of A to right up part of B
                    B[k][i+4] = a4;
                    B[k][i+5] = a5;
                    B[k][i+6] = a6;
                    B[k][i+7] = a7;

                    // move stored right up part of B to left bottom
                    B[k+4][i] = a0;
                    B[k+4][i+1] = a1;
                    B[k+4][i+2] = a2;
                    B[k+4][i+3] = a3;
                }

                for(k = i + 4; k < i + 8; ++k){
                    // move the right bottom part
                    a0 = A[k][j+4];
                    a1 = A[k][j+5];
                    a2 = A[k][j+6];
                    a3 = A[k][j+7];
                    B[j+4][k] = a0;
                    B[j+5][k] = a1;
                    B[j+6][k] = a2;
                    B[j+7][k] = a3;
                }
            }
        }
        return;
    }

    int s;
    if(M == 61 && N == 67){
        // split as 16*16 block
        for (i = 0; i < M; i += 16) {
            for (j = 0; j < N; j += 16) {
                // be aware to check the rest cases not enough a block
                for(k = j; k < j + 16 && k < N; ++k){
                    for(s = i; s < i + 16 && s < M; ++s){
                        B[s][k] = A[k][s];
                    }
                }
            }
        }
        return;
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

