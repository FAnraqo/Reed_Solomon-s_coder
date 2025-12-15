#ifndef RS_ENCODER_H
#define RS_ENCODER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

typedef struct {
    int q;              // Размер поля (15 для GF(2^4))
    int n;              // Длина кодового слова
    int k;              // Число информационных символов
    int t;              // Число исправляемых ошибок
    int r;              // Число проверочных символов (n-k)
    int m;              // Степень поля (4 для GF(2^4))
    
    // Таблицы поля
    int *log_table;     // Таблица логарифмов (размер q+1)
    int *alog_table;    // Таблица антилогарифмов (размер 2*q)
    int *inverse;       // Таблица обратных элементов (размер q+1)
    
    // Генератор полинома
    int *g;             // Коэффициенты g(x)
    int g_deg;          // Степень g(x)
} RS_Coder;

void build_gf_field(int *log_table, int *alog_table, int m, int prim_poly);
void build_inverse_table(int *inverse, int *log_table, int *alog_table, int q);

int gf_add(int a, int b);
int gf_multiply(int a, int b, int *log_table, int *alog_table, int q);
int gf_divide(int a, int b, int *log_table, int *alog_table, int q);
int gf_inverse(int a, int *log_table, int *alog_table, int q);

void build_generator_polynomial(RS_Coder *rs, int *log_table, int *alog_table);
void polynomial_divide(int *dividend, int dividend_deg,
                      int *divisor, int divisor_deg,
                      int *quotient, int *remainder,
                      int *log_table, int *alog_table, int q);
void polynomial_multiply(int *poly1, int poly1_deg,
                        int *poly2, int poly2_deg,
                        int *result,
                        int *log_table, int *alog_table, int q);

RS_Coder* rs_init(int n, int k, int m, int prim_poly);
void rs_free(RS_Coder *rs);
void rs_encode(RS_Coder *rs, int *message, int *codeword);

int verify_codeword(RS_Coder *rs, int *codeword);
int check_cyclicity(RS_Coder *rs, int *codeword);


void print_gf_field(int *log_table, int *alog_table, int q);
void print_generator_polynomial(RS_Coder *rs);
void print_codeword(int *codeword, int n);

#endif
