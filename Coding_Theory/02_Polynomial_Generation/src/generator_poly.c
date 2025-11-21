#include "generator_poly.h"
#include <stdio.h>
#include <stdbool.h>

int poly_degree(Polynomial p) {
    if (p == 0) return -1;
    int degree = 0;
    while (p > 1) {
        p >>= 1;
        degree++;
    }
    return degree;
}

static void print_poly(Polynomial p) {
    if (p == 0) {
        printf("0");
        return;
    }
    bool first = true;
    for (int i = 0; i <= 32; i++) {
        if ((p >> i) & 1) {
            if (!first) printf(" + ");
            if (i == 0) printf("1");
            else if (i == 1) printf("x");
            else printf("x^%d", i);
            first = false;
        }
    }
}

CyclicCodeParams get_generator_polynomial(int n, int poly_index) {
    CyclicCodeParams params = {0, 0, 0};
    
    switch (n) {
        case 3:
            if (poly_index == 0) {
                params.n = 3;
                params.g = 0x3;
                params.k = 2;
            } else if (poly_index == 1) {
                params.n = 3;
                params.g = 0x7;
                params.k = 1;
            }
            break;
            
        case 5:
            if (poly_index == 0) {
                params.n = 5;
                params.g = 0x3;
                params.k = 4;
            } else if (poly_index == 1) {
                params.n = 5;
                params.g = 0x1F;
                params.k = 1;
            }
            break;
            
        case 7:
            if (poly_index == 0) {
                params.n = 7;
                params.g = 0x3;
                params.k = 6;
            } else if (poly_index == 1) {
                params.n = 7;
                params.g = 0x0B;
                params.k = 4;
            } else if (poly_index == 2) {
                params.n = 7;
                params.g = 0x0D;
                params.k = 4;
            }
            break;
            
        case 15:
            if (poly_index == 0) {
                params.n = 15;
                params.g = 0x3;
                params.k = 14;
            } else if (poly_index == 1) {
                params.n = 15;
                params.g = 0x13;
                params.k = 11;
            } else if (poly_index == 2) {
                params.n = 15;
                params.g = 0x19;
                params.k = 11;
            } else if (poly_index == 3) {
                params.n = 15;
                params.g = 0x1F;
                params.k = 11;
            }
            break;
            
        case 31:
            if (poly_index == 0) {
                params.n = 31;
                params.g = 0x3;
                params.k = 30;
            } else if (poly_index == 1) {
                params.n = 31;
                params.g = 0x25;
                params.k = 26;
            } else if (poly_index == 2) {
                params.n = 31;
                params.g = 0x29;
                params.k = 26;
            }
            break;
            
        default:
            fprintf(stderr, "Ошибка: n=%d не поддерживается\n", n);
            break;
    }
    
    return params;
}

void print_polynomial_info(CyclicCodeParams params) {
    if (params.g == 0) {
        printf("Неверные параметры\n");
        return;
    }
    
    printf("Код: [%d, %d]\n", params.n, params.k);
    printf("Порождающий многочлен g(x) = ");
    print_poly(params.g);
    printf(" (0x%lx)\n", params.g);
    printf("Степень: %d\n", poly_degree(params.g));
    printf("Число кодовых слов: 2^%d = %d\n", params.k, 1 << params.k);
}
