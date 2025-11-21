#ifndef EUCLIDEAN_DECODER_H
#define EUCLIDEAN_DECODER_H

#include <stdint.h>
#include <stdbool.h>

/* Полином над GF(2^m) */
typedef struct {
    uint8_t *coeff;     // Коэффициенты
    int degree;         // Степень многочлена
    int max_degree;     // Максимальная выделенная степень
} Polynomial;

typedef struct {
    Polynomial *error_locator;       // Λ(x) - полином локации ошибок
    Polynomial *error_evaluator;     // Ω(x) - полином значения ошибок
    int error_count;                  // Число найденных ошибок
    int iterations;                   // Число итераций алгоритма
    bool success;
    char *debug_info;                // Информация для отладки
} EuclideanResult;

/* Структура для итерационного процесса */
typedef struct {
    Polynomial *r_prev, *r_curr;     // r_{-1}(x), r_0(x), r_1(x), ...
    Polynomial *v_prev, *v_curr;     // v_{-1}(x), v_0(x), v_1(x), ...
    Polynomial *u_prev, *u_curr;     // u_{-1}(x), u_0(x), u_1(x), ...
} EuclideanState;

/* Создать полином */
Polynomial* poly_create(int degree);

/* Скопировать полином */
Polynomial* poly_copy(const Polynomial *src);

/* Освободить полином */
void poly_free(Polynomial *p);

/* Вывести полином */
void poly_print(const Polynomial *p, const char *name);

/* Получить степень полинома */
int poly_degree(const Polynomial *p);

/* Сложение полиномов (XOR в GF(2)) */
Polynomial* poly_add(const Polynomial *a, const Polynomial *b);

/* Умножение полиномов */
Polynomial* poly_mul(const Polynomial *a, const Polynomial *b);

/* Деление полиномов с остатком: a(x) / b(x) = q(x) rem r(x) */
void poly_divmod(const Polynomial *a, const Polynomial *b, 
                 Polynomial **quotient, Polynomial **remainder);

/* Скалярное умножение (q(x) * p(x)) */
Polynomial* poly_scalar_mul(const Polynomial *p, const Polynomial *q);

/* Масштабирование: коэффициент за элементом поля c */
Polynomial* poly_scale(const Polynomial *p, uint8_t scale);

/* Привести степень к длине t */
Polynomial* poly_truncate(const Polynomial *p, int max_degree);

/* Декодирование BCH кода методом Евклида (Sugiyama algorithm)
 * 
 * Параметры:
 *   syndromes - массив синдромов S1, S2, ..., S_{2t}
 *   num_syndromes - число синдромов (обычно 2t)
 *   t - способность коррекции (t ошибок)
 *   field_degree - m (для GF(2^m))
 * 
 * Возвращает:
 *   EuclideanResult с полиномами Λ(x) и Ω(x)
 */
EuclideanResult* euclidean_decode(
    const uint8_t *syndromes,
    int num_syndromes,
    int t,
    int field_degree
);

/* Вывести результат декодирования */
void print_euclidean_result(const EuclideanResult *result);

/* Освободить результат */
void euclidean_result_free(EuclideanResult *result);

#endif