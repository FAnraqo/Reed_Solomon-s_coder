#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/**
 * @brief Вычисляет сумму двух чисел.
 * 
 * Эта функция принимает два целых числа и возвращает их сумму.
 * 
 * @param a Первое целое число.
 * @param b Второе целое число.
 * @return Сумма чисел a и b.
 */
void create_field(int degree, int *field);

/**
 * @brief Вычисляет сумму двух чисел.
 * 
 * Эта функция принимает два целых числа и возвращает их сумму.
 * 
 * @param a Первое целое число.
 * @param b Второе целое число.
 * @return Сумма чисел a и b.
 */
void print_field(int degree, int *field);

/**
 * @brief Вычисляет сумму двух чисел.
 * 
 * Sgf(x, y)
 * 
 * Эта функция принимает два целых числа и возвращает их сумму.
 * 
 * @param a Первое целое число.
 * @param b Второе целое число.
 * @return Сумма чисел a и b.
 */
int adding_elements(int *field, int a, int b);

/**
 * @brief Вычисляет сумму двух чисел.
 * 
 * Ugf(x, y)
 * 
 * Эта функция принимает два целых числа и возвращает их сумму.
 * 
 * @param a Первое целое число.
 * @param b Второе целое число.
 * @return Сумма чисел a и b.
 */
int multiplication_elements(int a, int b);

/**
 * @brief Вычисляет сумму двух чисел.
 * 
 * Obr(x)
 * 
 * Эта функция принимает два целых числа и возвращает их сумму.
 * 
 * @param a Первое целое число.
 * @param b Второе целое число.
 * @return Сумма чисел a и b.
 */
int reverse_element(int degree, int a);

/**
 * @brief Вычисляет сумму двух чисел.
 * 
 * L(x, a)
 * 
 * Эта функция принимает два целых числа и возвращает их сумму.
 * 
 * @param a Первое целое число.
 * @param b Второе целое число.
 * @return Сумма чисел a и b.
 */
int polinom(int degree, int *field, int *X, int length_X, int a);

void koder_9_5(int symbol, int obr);

void print_adding_table(int *field, int degree);

void print_multiplication_table(int *field, int degree);

#endif