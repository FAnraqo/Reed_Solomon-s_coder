#ifndef GENERATOR_POLY_H
#define GENERATOR_POLY_H

#include <stdint.h>

typedef uint64_t Polynomial;

typedef struct {
    int n;              // Длина кода
    int k;              // Размерность (информационные биты)
    Polynomial g;       // Порождающий многочлен
} CyclicCodeParams;

/* Получить порождающий многочлен для заданных параметров
 * n - длина кода
 * poly_index - номер многочлена
 * Возвращает структуру с параметрами или {0,0,0} при ошибке */
CyclicCodeParams get_generator_polynomial(int n, int poly_index);

void print_polynomial_info(CyclicCodeParams params);

int poly_degree(Polynomial p);

#endif
