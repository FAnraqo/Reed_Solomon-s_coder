#ifndef PGZ_DECODER_H
#define PGZ_DECODER_H

#include <stdbool.h>
#include <stdint.h>

void compute_syndromes(const uint8_t *rec_word, int n, int d, uint8_t *out_syndr);

bool solve_locator_poly(const uint8_t *synd, int t, uint8_t *locator_poly);

// Поиск позиций ошибок (корней полинома)
int find_error_positions(const uint8_t *locator_poly, int t, int n, int *positions);

// Поправка ошибок
void correct_errors(uint8_t *word, int n, const int *positions, int num);

#endif