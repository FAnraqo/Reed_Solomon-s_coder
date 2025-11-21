#ifndef UNIVERSAL_ENCODER_H
#define UNIVERSAL_ENCODER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t *data;    // Массив битов
    int length;       // Длина вектора
} BitVector;

/* Создать вектор битов */
BitVector* bitvector_create(int length);

/* Освободить вектор битов */
void bitvector_free(BitVector *vec);

/* Скопировать вектор */
BitVector* bitvector_copy(const BitVector *src);

/* Вывести вектор */
void bitvector_print(const BitVector *vec, const char *name);

/* ОСНОВНАЯ ФУНКЦИЯ: Универсальный кодер
 * 
 * Параметры:
 *   Q - информационное слово (k бит)
 *   P - порождающий многочлен (степени r, длина r+1)
 * 
 * Возвращает:
 *   KK - кодовое слово (n бит), где n = k + r
 *        Формат: [проверочные биты | информационные биты]
 */
BitVector* koder(const BitVector *Q, const BitVector *P);

#endif