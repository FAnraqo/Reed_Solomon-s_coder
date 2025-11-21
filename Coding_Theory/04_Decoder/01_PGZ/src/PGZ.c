// pgz_decoder.c

#include "pgz_decoder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void compute_syndromes(const uint8_t *rec_word, int n, int d, uint8_t *synd) {
    int max_t = (d - 1) / 2;
    
    for (int j = 1; j <= 2*max_t; ++j) {
        synd[j - 1] = 0;
        for (int i = 0; i < n; ++i) {

            int pow = field_pow(alpha, (i * j) % field_size);
            synd[j - 1] = gf_add(synd[j - 1], gf_mul(rec_word[i], pow));
        }
    }
}

bool solve_locator_poly(const uint8_t *synd, int t, uint8_t *locator_poly) {

    uint8_t matrix[t][t], synd_vec[t];

    return true;
}

int find_error_positions(const uint8_t *locator_poly, int t, int n, int *positions) {
    int num = 0;
    for (int i = 0; i < n; ++i) {
        uint8_t sum = 1;
        uint8_t alpha_i = field_pow(alpha, i);
        for (int j = 1; j <= t; ++j) {
            sum = gf_add(sum, gf_mul(locator_poly[j], field_pow(alpha_i, j)));
        }
        if (sum == 0) {
            positions[num++] = i;
        }
    }
    return num;
}

void correct_errors(uint8_t *word, int n, const int *positions, int num) {
    for (int k = 0; k < num; ++k) {
        word[positions[k]] ^= 1;
    }
}

