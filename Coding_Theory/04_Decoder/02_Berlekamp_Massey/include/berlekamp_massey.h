#ifndef BERLEKAMP_MASSEY_H
#define BERLEKAMP_MASSEY_H

#include <stdint.h>
#include <stdbool.h>

/* Полином над GF(2^m) */
typedef struct {
    uint8_t *coeff;     // Коэффициенты
    int degree;         // Степень (максимум t для BCH)
    int field_degree;   // m (размер поля GF(2^m))
} Polynomial;

/* Результат BM алгоритма */
typedef struct {
    Polynomial *error_locator;      // Λ(x) - полином локации ошибок
    Polynomial *error_evaluator;    // Ω(x) - полином значения ошибок
    int error_count;                 // Число найденных ошибок
    bool success;
} BMResult;

/* Создать полином */
Polynomial* poly_create(int degree, int field_degree);

/* Скопировать полином */
Polynomial* poly_copy(const Polynomial *src);

/* Освободить полином */
void poly_free(Polynomial *p);

/* Вывести полином */
void poly_print(const Polynomial *p, const char *name);

/* Сравнить полиномы */
bool poly_equal(const Polynomial *a, const Polynomial *b);


/* Для примера - используйте вашу реализацию GF */
typedef uint16_t GFElement;

/* Сложение в GF(2^m) */
static inline GFElement gf_add(GFElement a, GFElement b) {
    return a ^ b;  // XOR в GF(2)
}

/* Умножение в GF(2^m) - требует таблиц поля */
GFElement gf_mul(GFElement a, GFElement b, const uint16_t *log_table, const uint16_t *exp_table);

/* Инверсия в GF(2^m) */
GFElement gf_inv(GFElement a, int m);


/* Декодирование BCH кода методом Берликемпа-Месси
 * 
 * Параметры:
 *   syndromes - массив синдромов S1, S2, ..., S_{2t}
 *   num_syndromes - число синдромов (обычно 2t)
 *   t - способность коррекции (t ошибок)
 *   field_degree - m (для GF(2^m))
 * 
 * Возвращает:
 *   BMResult с полиномом локации ошибок Λ(x)
 */
BMResult* berlekamp_massey(
    const GFElement *syndromes,
    int num_syndromes,
    int t,
    int field_degree
);

/* Освободить результат */
void bm_result_free(BMResult *result);

#endif /* BERLEKAMP_MASSEY_H */
