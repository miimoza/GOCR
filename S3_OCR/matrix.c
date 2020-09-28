#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

void print_matrix(double mat[], size_t lines, size_t cols)
{
   for(size_t h=0; h<lines;h++)
    {
        for(size_t w=0;w<cols;w++)
        {
            printf("%4g",mat[w + h * cols]);
        }
        printf("\n");
    }
}


void transpose(double mat[], size_t lines, size_t cols, double res[])
{
    for(size_t h=0; h<lines;h++)
    {
        for(size_t w=0;w<cols;w++)
        {
            res[h + w * lines] = mat[w + h * cols];
        }
    }

}


void add(double mat1[], double mat2[], size_t lines, size_t cols, double res[])
{
    for(size_t h=0; h<lines;h++)
    {
        for(size_t w=0;w<cols;w++)
        {
            res[w + h * cols] = mat1[w + h * cols] + mat2[w + h * cols];
        }
    }
}

void substract(double mat1[], double mat2[], size_t lines, size_t cols, double res[])
{
    for(size_t h=0; h<lines;h++)
    {
        for(size_t w=0;w<cols;w++)
        {
            res[w + h * cols] = mat1[w + h * cols] - mat2[w + h * cols];
        }
    }
}

void mul(double m1[], double m2[], size_t n, size_t m, size_t p, double res[])
{
    // on peut pas simplement renvoyer un tableau parce que la taille doit
    // être définie avant
//    printf("n :%zu m:%zu p:%zu \n",n,m,p );
    for(size_t h=0; h<n;h++)
    {
        for(size_t w=0;w<p;w++)
        {
            res[w + h * p] = 0;
            for(size_t i=0; i<m; i++)
            {
                     res[w + h * p] += m1[i + h * m] * m2[w + i * p];
            }
        }
    }
}

void vector_horizontal_mul(double v1[], double v2[], size_t size, double res[])
{
  for(size_t i = 0; i<size;i++)
  {
    res[i] = v1[i] * v2[i];
  }
}

void vector_apply(func_t f, double vect[], size_t size, double res[])
{
    for(size_t i = 0; i<size;i++)
    {
        res[i] = f(vect[i]);
    }
}
