#include "universal_encoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BitVector* bitvector_create(int length) {
    BitVector *vec = malloc(sizeof(BitVector));
    if (!vec) return NULL;
    
    vec->length = length;
    vec->data = calloc(length, sizeof(uint8_t));
    if (!vec->data) {
        free(vec);
        return NULL;
    }
    
    return vec;
}

void bitvector_free(BitVector *vec) {
    if (vec) {
        free(vec->data);
        free(vec);
    }
}

BitVector* bitvector_copy(const BitVector *src) {
    if (!src) return NULL;
    
    BitVector *dst = bitvector_create(src->length);
    if (!dst) return NULL;
    
    memcpy(dst->data, src->data, src->length * sizeof(uint8_t));
    return dst;
}

void bitvector_print(const BitVector *vec, const char *name) {
    if (!vec) return;
    
    printf("%s = [", name);
    for (int i = 0; i < vec->length; i++) {
        printf("%d", vec->data[i]);
        if (i < vec->length - 1) printf(" ");
    }
    printf("]\n");
}

static BitVector* bitvector_xor(const BitVector *a, const BitVector *b) {
    if (!a || !b || a->length != b->length) return NULL;
    
    BitVector *result = bitvector_create(a->length);
    if (!result) return NULL;
    
    for (int i = 0; i < a->length; i++) {
        result->data[i] = a->data[i] ^ b->data[i];
    }
    
    return result;
}

static BitVector* bitvector_stack(const BitVector *a, const BitVector *b) {
    if (!a || !b) return NULL;
    
    BitVector *result = bitvector_create(a->length + b->length);
    if (!result) return NULL;
    
    memcpy(result->data, a->data, a->length * sizeof(uint8_t));
    memcpy(result->data + a->length, b->data, b->length * sizeof(uint8_t));
    
    return result;
}

static BitVector* bitvector_submatrix(const BitVector *vec, int start, int length) {
    if (!vec || start < 0 || length <= 0 || start + length > vec->length) {
        return NULL;
    }
    
    BitVector *result = bitvector_create(length);
    if (!result) return NULL;
    
    memcpy(result->data, vec->data + start, length * sizeof(uint8_t));
    return result;
}

static void bitvector_shift_left(BitVector *vec) {
    if (!vec || vec->length <= 1) return;
    
    for (int j = 0; j < vec->length - 1; j++) {
        vec->data[j] = vec->data[j + 1];
    }
}

BitVector* koder(const BitVector *Q, const BitVector *P) {
    if (!Q || !P) {
        fprintf(stderr, "Ошибка: Q или P равен NULL\n");
        return NULL;
    }
    
    int k = Q->length;              // Длина информационного слова
    int r = P->length - 1;          // Степень порождающего многочлена
    int n = k + r;                  // Длина кодового слова
    
    printf("\n=== УНИВЕРСАЛЬНЫЙ КОДЕР ===\n");
    printf("k (информационные биты) = %d\n", k);
    printf("r (проверочные биты) = %d\n", r);
    printf("n (длина кода) = %d\n\n", n);
    
    BitVector *mul_last_P_minus_1 = bitvector_create(r);
    if (!mul_last_P_minus_1) return NULL;
    
    BitVector *NulP_last = bitvector_create(r);
    BitVector *Qxr = bitvector_stack(Q, NulP_last);
    bitvector_free(NulP_last);
    
    if (!Qxr) {
        bitvector_free(mul_last_P_minus_1);
        return NULL;
    }
    
    bitvector_print(Q, "Q (информация)");
    bitvector_print(P, "P (порождающий)");
    bitvector_print(Qxr, "Qxr (Q + нули)");
    
    BitVector *Q1 = bitvector_submatrix(Qxr, 0, P->length);
    if (!Q1) {
        bitvector_free(mul_last_P_minus_1);
        bitvector_free(Qxr);
        return NULL;
    }
    
    BitVector *D = NULL;
    if (Q1->data[0] == 1) {
        D = bitvector_copy(P);
    } else {
        D = bitvector_create(P->length);
    }
    
    if (!D) {
        bitvector_free(mul_last_P_minus_1);
        bitvector_free(Qxr);
        bitvector_free(Q1);
        return NULL;
    }
    
    BitVector *Reg = bitvector_xor(Q1, D);
    bitvector_free(Q1);
    bitvector_free(D);
    
    if (!Reg) {
        bitvector_free(mul_last_P_minus_1);
        bitvector_free(Qxr);
        return NULL;
    }
    
    printf("\nНачальный регистр:\n");
    bitvector_print(Reg, "Reg");
    
    printf("\n=== ПРОЦЕСС СДВИГА РЕГИСТРА ===\n");
    
    for (int i = P->length; i < Qxr->length; i++) {
        printf("\nИтерация i=%d:\n", i);
        
        for (int j = 0; j < Reg->length - 1; j++) {
            Reg->data[j] = Reg->data[j + 1];
        }
        
        Reg->data[Reg->length - 1] = Qxr->data[i];
        
        printf("  После сдвига: ");
        for (int j = 0; j < Reg->length; j++) {
            printf("%d", Reg->data[j]);
        }
        printf("\n");
        
        BitVector *D_new = NULL;
        if (Reg->data[0] == 1) {
            D_new = bitvector_copy(P);
            printf("  Reg[0]=1, используем P\n");
        } else {
            D_new = bitvector_create(P->length);
            printf("  Reg[0]=0, используем нули\n");
        }
        
        BitVector *Reg_new = bitvector_xor(Reg, D_new);
        bitvector_free(D_new);
        bitvector_free(Reg);
        Reg = Reg_new;
        
        printf("  После XOR:    ");
        for (int j = 0; j < Reg->length; j++) {
            printf("%d", Reg->data[j]);
        }
        printf("\n");
    }
    
    printf("\n=== РЕЗУЛЬТАТ ===\n");
    bitvector_print(Reg, "Финальный регистр (проверочные биты)");
    
    BitVector *parity = bitvector_submatrix(Reg, 1, Reg->length - 1);
    BitVector *KK = bitvector_stack(Q, parity);
    
    bitvector_free(parity);
    bitvector_free(Reg);
    bitvector_free(Qxr);
    bitvector_free(mul_last_P_minus_1);
    
    if (KK) {
        bitvector_print(KK, "KK (кодовое слово)");
    }
    
    return KK;
}