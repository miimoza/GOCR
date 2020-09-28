/*
 * matrix.h: standard matrix operations
 *
 * 2017 Marwan Burelle / EPITA
 */

#ifndef _MATRIXSTDOPERATIONS_MATRIX_H_
#define _MATRIXSTDOPERATIONS_MATRIX_H_

/*
 * transpose(mat, lines, cols, res)
 * transpose the lines X cols matrix mat into a cols X lines matrix
 * result will be stored in res
 */
void transpose(double mat[], size_t lines, size_t cols, double res[]);

/*
 * add(mat1, mat2, lines, cols, res)
 * computes res = mat1 + mat2
 * All 3 matrices have the form lines X cols
 */
void add(double mat1[], double mat2[], size_t lines, size_t cols, double res[]);

/*
compute res = mat1 - mat2
*/
void substract(double mat1[], double mat2[], size_t lines, size_t cols, double res[]);

/*
 * mul(m1, m2, n, m, p, res)
 * computes res = m1 * m2
 * Where:
 *        m1 is a n X m matrix
 *        m2 is a m X p matrix
 *        res is a n X p matrix
 */
void mul(double m1[], double m2[], size_t n, size_t m, size_t p, double res[]);

/*
 v1(a,b,c,d) v2(e,f,g,h) --> res(a*e,b*f,c*g,d*h)
*/
void vector_horizontal_mul(double v1[], double v2[], size_t size, double res[]);


/*
 * print_matrix(mat, lines, cols)
 * prints the lines X cols matrix mat
 * All coeficient will be printed using the format " %4g ", line by line
 */
void print_matrix(double mat[], size_t lines, size_t cols);

/*
 * vector_apply(f, vect, size, res)
 * applies a function to all element on a vector of size n
 * for i in 0..size, res[i] = f[vect[i]]
 */

typedef double (*func_t)(double);

void vector_apply(func_t f, double vect[], size_t size, double res[]);


#endif /* _MATRIXSTDOPERATIONS_MATRIX_H_ */
