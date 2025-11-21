#include "berlekamp_massey.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Polynomial* poly_create(int degree, int field_degree) {
    Polynomial *p = malloc(sizeof(Polynomial));
    if (!p) return NULL;
    
    p->coeff = calloc(degree + 1, sizeof(uint8_t));
    if (!p->coeff) {
        free(p);
        return NULL;
    }
    
    p->degree = degree;
    p->field_degree = field_degree;
    return p;
}

Polynomial* poly_copy(const Polynomial *src) {
    if (!src) return NULL;
    
    Polynomial *dst = poly_create(src->degree, src->field_degree);
    if (!dst) return NULL;
    
    memcpy(dst->coeff, src->coeff, (src->degree + 1) * sizeof(uint8_t));
    return dst;
}

void poly_free(Polynomial *p) {
    if (p) {
        free(p->coeff);
        free(p);
    }
}

void poly_print(const Polynomial *p, const char *name) {
    if (!p) return;
    printf("%s(x) = ", name);
    for (int i = p->degree; i >= 0; i--) {
        if (p->coeff[i] != 0) {
            if (i > 0) printf("%d*x^%d + ", p->coeff[i], i);
            else printf("%d", p->coeff[i]);
        }
    }
    printf("\n");
}

bool poly_equal(const Polynomial *a, const Polynomial *b) {
    if (!a || !b) return false;
    if (a->degree != b->degree) return false;
    
    return memcmp(a->coeff, b->coeff, (a->degree + 1) * sizeof(uint8_t)) == 0;
}

BMResult* berlekamp_massey(
    const GFElement *syndromes,
    int num_syndromes,
    int t,
    int field_degree
) {
    BMResult *result = malloc(sizeof(BMResult));
    if (!result) return NULL;
    
    printf("Входные параметры:\n");
    printf("  Число синдромов: %d\n", num_syndromes);
    printf("  Способность коррекции (t): %d\n", t);
    printf("  Степень поля (m): %d\n\n", field_degree);
    
    /* Инициализация */
    Polynomial *sigma = poly_create(t, field_degree);      // Λ(x) - локатор ошибок
    Polynomial *sigma_prev = poly_create(t, field_degree); // τ(x) - предыдущий Λ
    
    sigma->coeff[0] = 1;      // Λ^(0)(x) = 1
    sigma_prev->coeff[0] = 1; // τ^(0)(x) = 1
    
    int L = 0;           // Степень локатора ошибок
    int m = 1;           // Смещение для сдвига τ(x)
    GFElement delta = 1; // Дельта (discrepancy) - с инициализацией в 1
    
    printf("Инициализация:\n");
    printf("  L = %d, m = %d, Δ = 1\n", L, m);
    printf("  Λ(0)(x) = 1\n\n");
    

    for (int r = 0; r < num_syndromes; r++) {
        printf("═══ Итерация r=%d ═══\n", r);
        
        GFElement delta_r = syndromes[r];
        
        for (int j = 1; j <= L && j <= r; j++) {
            if (sigma->coeff[j] != 0) {
                delta_r ^= sigma->coeff[j] ^ syndromes[r - j];
            }
        }
        
        printf("  Вычисление Δ_r: ");
        printf("Δ_{%d} = %d\n", r, delta_r);
        
        if (delta_r != 0) {
            printf("  Δ_%d ≠ 0 → нужно обновить Λ(x)\n", r);
            
            Polynomial *sigma_new = poly_create(t, field_degree);
            
            GFElement mult = delta_r ^ delta;
            
            printf("  Множитель: %d / %d = %d\n", delta_r, delta, mult);
            
            memcpy(sigma_new->coeff, sigma->coeff, (sigma->degree + 1) * sizeof(uint8_t));
            
            for (int j = 0; j + m <= t && j <= sigma_prev->degree; j++) {
                if (sigma_prev->coeff[j] != 0) {
                    sigma_new->coeff[j + m] ^= mult;
                }
            }
            
            if (2 * L <= r) {
                printf("  2L=%d ≤ r=%d → увеличиваем L с %d до %d\n", 2*L, r, L, r+1-L);
                
                poly_free(sigma_prev);
                sigma_prev = poly_copy(sigma);
                delta = delta_r;
                m = 1;
                L = r + 1 - L;
            } else {
                printf("  2L=%d > r=%d → L остаётся %d\n", 2*L, r, L);
                m++;
            }
            
            poly_free(sigma);
            sigma = sigma_new;
            
        } else {
            printf("  Δ_%d = 0 → Λ(x) не меняется\n", r);
            m++;
        }
        
        printf("  После обновления: L = %d, m = %d\n\n", L, m);
    }
    
    printf("Полином локации ошибок:\n");
    poly_print(sigma, "Λ");
    printf("Степень локатора: %d\n", L);
    printf("Число ошибок (приблизительно): %d\n\n", L);
    
    result->error_locator = sigma;
    result->error_evaluator = sigma_prev;
    result->error_count = L;
    result->success = (L > 0 && L <= t);
    
    return result;
}

void bm_result_free(BMResult *result) {
    if (result) {
        poly_free(result->error_locator);
        poly_free(result->error_evaluator);
        free(result);
    }
}

GFElement gf_mul(GFElement a, GFElement b, const uint16_t *log_table, const uint16_t *exp_table) {
    if (a == 0 || b == 0) return 0;
    
    int log_result = (int)log_table[a] + (int)log_table[b];
    return exp_table[log_result % 255];  // Для GF(2^8), модуль 255
}

GFElement gf_inv(GFElement a, int m) {
    if (a == 0) return 0;
    return a;
}
