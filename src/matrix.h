#ifndef MATRIX_H
#define MATRIX_H
typedef struct matrix{
    int rows, cols;
    double **data;
} matrix;

typedef struct LUP{
    matrix *L;
    matrix *U;
    int *P;
    int n;
} LUP;

matrix make_identity_homography();
matrix make_translation_homography(float dx, float dy);

void free_matrix(matrix m);
matrix make_matrix(int rows, int cols);
double *sle_solve(matrix A, double *b);
matrix matrix_mult_matrix(matrix a, matrix b);
void print_matrix(matrix m);
double **n_principal_components(matrix m, int n);
void test_matrix();
matrix solve_system(matrix M, matrix b);
matrix matrix_invert(matrix m);
#endif
