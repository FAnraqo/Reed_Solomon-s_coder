#ifndef RS_DECODER_H
#define RS_DECODER_H

#include "rs_encoder.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int *message;              /* Восстановленное сообщение (k символов) */
    int *error_positions;      /* Позиции ошибок */
    int num_errors;            /* Число исправленных ошибок */
    bool success;              /* Успешно ли декодировано */
    int iterations;            /* Число итераций алгоритма */
} RS_DecoderResult;

/**
 * Полное декодирование по методу Евклида
 * @param rs - структура RS кодера (с таблицами GF)
 * @param received - принятое кодовое слово (n символов)
 * @return результат декодирования
 */
RS_DecoderResult* rs_decode_euclidean(RS_Coder *rs, int *received);


void compute_syndromes(RS_Coder *rs, int *received, int *syndromes);

bool euclidean_algorithm(RS_Coder *rs, int *syndromes,
                         int *lambda, int *omega);

int find_error_positions(RS_Coder *rs, int *lambda, int *positions);


void calculate_error_values(RS_Coder *rs, int *omega, int *lambda,
                           int *positions, int num_errors,
                           int *error_values);

void correct_errors(int *received, int *positions, int *error_values,
                   int num_errors);


void extract_message(int *codeword, int n, int k, int *message);


void print_decoder_result(RS_DecoderResult *result, int k);


void decoder_result_free(RS_DecoderResult *result);

#endif