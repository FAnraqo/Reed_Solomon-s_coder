#include "rs_decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void compute_syndromes(RS_Coder *rs, int *received, int *syndromes) {
    if (!rs || !received || !syndromes) return;
    
    memset(syndromes, 0, (2 * rs->t) * sizeof(int));

    for (int j = 1; j <= 2 * rs->t; j++) {
        int sum = 0;
        
        for (int i = 0; i < rs->n; i++) {
            if (received[i] == 0) continue;
            
            int power = (i * j) % (rs->q);
            int alpha_pow = rs->alog_table[power];
            
            int product = gf_multiply(received[i], alpha_pow,
                                     rs->log_table, rs->alog_table, rs->q);
            sum = gf_add(sum, product);
        }
        
        syndromes[j - 1] = sum;
    }
}

bool euclidean_algorithm(RS_Coder *rs, int *syndromes,
                         int *lambda, int *omega) {
    if (!rs || !syndromes || !lambda || !omega) return false;
    
    int t = rs->t;
    
    int *S = (int*)malloc((2 * t + 1) * sizeof(int));
    memset(S, 0, (2 * t + 1) * sizeof(int));
    for (int i = 0; i < 2 * t; i++) {
        S[i + 1] = syndromes[i];
    }
    
    int *r_prev = (int*)malloc((2 * t + 1) * sizeof(int));
    memset(r_prev, 0, (2 * t + 1) * sizeof(int));
    r_prev[2 * t] = 1;
    
    int *r_curr = (int*)malloc((2 * t + 1) * sizeof(int));
    memcpy(r_curr, S, (2 * t + 1) * sizeof(int));
    
    int *v_prev = (int*)malloc((t + 1) * sizeof(int));
    memset(v_prev, 0, (t + 1) * sizeof(int));
    
    int *v_curr = (int*)malloc((t + 1) * sizeof(int));
    memset(v_curr, 0, (t + 1) * sizeof(int));
    v_curr[0] = 1;
    
    printf("\n=== АЛГОРИТМ ЕВКЛИДА ===\n");
    printf("Инициализация:\n");
    printf("  r_prev: X^{%d}\n", 2 * t);
    printf("  r_curr: S(X)\n");
    printf("  v_prev: 0\n");
    printf("  v_curr: 1\n");
    
    int iteration = 0;
    const int MAX_ITERATIONS = 100;
    
    while (true) {
        int r_curr_deg = -1;
        for (int i = 2 * t; i >= 0; i--) {
            if (r_curr[i] != 0) {
                r_curr_deg = i;
                break;
            }
        }
        
        printf("\n--- Итерация %d ---\n", iteration);
        printf("  deg(r_curr) = %d\n", r_curr_deg);
        
        if (r_curr_deg < t) {
            printf("  ВЫХОД: deg(r_curr) < t\n");
            break;
        }
        
        if (iteration >= MAX_ITERATIONS) {
            fprintf(stderr, "ERROR: Too many iterations (>= %d)\n", MAX_ITERATIONS);
            free(S);
            free(r_prev);
            free(r_curr);
            free(v_prev);
            free(v_curr);
            return false;
        }
        
        int *q = (int*)malloc((2 * t + 1) * sizeof(int));
        int *r_next = (int*)malloc((2 * t + 1) * sizeof(int));
        
        memset(q, 0, (2 * t + 1) * sizeof(int));
        memset(r_next, 0, (2 * t + 1) * sizeof(int));
        
        polynomial_divide(r_prev, 2 * t,
                 r_curr, r_curr_deg,
                 q, r_next,
                 rs->log_table, rs->alog_table, rs->q);
        
        int q_deg = -1;
        for (int i = 2 * t; i >= 0; i--) {
            if (q[i] != 0) {
                q_deg = i;
                break;
            }
        }
        printf("  q степень: %d\n", q_deg);
        
        int *q_times_v = (int*)malloc((t + 1) * sizeof(int));
        int *v_next = (int*)malloc((t + 1) * sizeof(int));
        
        memset(q_times_v, 0, (t + 1) * sizeof(int));
        memset(v_next, 0, (t + 1) * sizeof(int));
        
        for (int i = 0; i <= t; i++) {
            if (v_curr[i] == 0) continue;
            for (int j = 0; j <= t && j <= q_deg; j++) {
                if (q[j] == 0) continue;
                if (i + j > t) break;
                
                int prod = gf_multiply(q[j], v_curr[i],
                                      rs->log_table, rs->alog_table, rs->q);
                q_times_v[i + j] = gf_add(q_times_v[i + j], prod);
            }
        }
        
        for (int i = 0; i <= t; i++) {
            v_next[i] = gf_add(v_prev[i], q_times_v[i]);
        }
        
        free(r_prev);
        free(v_prev);
        
        r_prev = r_curr;
        r_curr = r_next;
        v_prev = v_curr;
        v_curr = v_next;
        
        free(q);
        free(q_times_v);
        
        iteration++;
    }
    
    memcpy(lambda, v_curr, (t + 1) * sizeof(int));
    memcpy(omega, r_curr, (t + 1) * sizeof(int));
    
    printf("\nРезультаты:\n");
    printf("  Λ(X) найден\n");
    printf("  Ω(X) найден\n");
    printf("  Итераций: %d\n", iteration);
    
    free(S);
    free(r_prev);
    free(r_curr);
    free(v_prev);
    free(v_curr);
    
    return true;
}

int find_error_positions(RS_Coder *rs, int *lambda, int *positions) {
    if (!rs || !lambda || !positions) return 0;
    
    int t = rs->t;
    int num_errors = 0;
    
    printf("\n=== CHIEN SEARCH ===\n");
    
    for (int i = 0; i < rs->n; i++) {
        int sum = lambda[0];
        
        for (int j = 1; j <= t; j++) {
            if (lambda[j] == 0) continue;
            
            int power = ((rs->q) - (i * j) % (rs->q)) % (rs->q);
            int alpha_pow = rs->alog_table[power];
            
            int product = gf_multiply(lambda[j], alpha_pow,
                                     rs->log_table, rs->alog_table, rs->q);
            sum = gf_add(sum, product);
        }
        
        if (sum == 0) {
            positions[num_errors] = i;
            printf("  Ошибка найдена на позиции: %d\n", i);
            num_errors++;
        }
    }
    
    printf("  Всего ошибок найдено: %d\n", num_errors);
    
    return num_errors;
}

void calculate_error_values(RS_Coder *rs, int *omega, int *lambda,
                           int *positions, int num_errors,
                           int *error_values) {
    if (!rs || !omega || !lambda || !positions || !error_values) return;
    
    printf("\n=== FORNEY FORMULA ===\n");
    
    int t = rs->t;
    
    for (int idx = 0; idx < num_errors; idx++) {
        int pos = positions[idx];
        
        int inv_pos_power = ((rs->q) - pos % (rs->q)) % (rs->q);
        int alpha_inv_pos = rs->alog_table[inv_pos_power];
        
        int omega_val = omega[0];
        for (int j = 1; j <= t; j++) {
            if (omega[j] == 0) continue;
            int alpha_pow = gf_multiply(alpha_inv_pos, 
                                       rs->alog_table[((j - 1) * inv_pos_power) % rs->q],
                                       rs->log_table, rs->alog_table, rs->q);
            int prod = gf_multiply(omega[j], alpha_pow,
                                  rs->log_table, rs->alog_table, rs->q);
            omega_val = gf_add(omega_val, prod);
        }
        
        int lambda_prime = 0;
        for (int j = 1; j <= t; j++) {
            if (lambda[j] == 0) continue;
            if (j % 2 == 0) continue;

            int alpha_pow = gf_multiply(lambda[j],
                                       rs->alog_table[((j - 1) * inv_pos_power) % rs->q],
                                       rs->log_table, rs->alog_table, rs->q);
            lambda_prime = gf_add(lambda_prime, alpha_pow);
        }
        
        int error_val = gf_divide(omega_val, lambda_prime,
                                 rs->log_table, rs->alog_table, rs->q);
        
        error_values[idx] = error_val;
        printf("  Позиция %d: ошибка = %d\n", pos, error_val);
    }
}

void correct_errors(int *received, int *positions, int *error_values,
                   int num_errors) {
    if (!received || !positions || !error_values) return;
    
    printf("\n=== ИСПРАВЛЕНИЕ ОШИБОК ===\n");
    
    for (int i = 0; i < num_errors; i++) {
        int pos = positions[i];
        received[pos] = gf_add(received[pos], error_values[i]);
        printf("  Позиция %d: исправлена (ошибка = %d)\n", pos, error_values[i]);
    }
}

void extract_message(int *codeword, int n, int k, int *message) {
    if (!codeword || !message) return;
    
    int r = n - k;

    for (int i = 0; i < k; i++) {
        message[i] = codeword[r + i];
    }
}

RS_DecoderResult* rs_decode_euclidean(RS_Coder *rs, int *received) {
    if (!rs || !received) return NULL;
    
    RS_DecoderResult *result = (RS_DecoderResult*)malloc(sizeof(RS_DecoderResult));
    memset(result, 0, sizeof(RS_DecoderResult));
    
    result->message = (int*)malloc(rs->k * sizeof(int));
    result->error_positions = (int*)malloc(rs->t * sizeof(int));
    result->success = false;
    result->num_errors = 0;
    result->iterations = 0;
    
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║     ДЕКОДИРОВАНИЕ РИДА-СОЛОМОНА (ЕВКЛИД)         ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    
    int *syndromes = (int*)malloc((2 * rs->t) * sizeof(int));
    compute_syndromes(rs, received, syndromes);
    
    printf("\nСиндромы вычислены\n");
    
    int all_zero = 1;
    for (int i = 0; i < 2 * rs->t; i++) {
        if (syndromes[i] != 0) {
            all_zero = 0;
            break;
        }
    }
    
    if (all_zero) {
        printf("✓ Синдромы = 0: ошибок не найдено\n");
        extract_message(received, rs->n, rs->k, result->message);
        result->success = true;
        result->num_errors = 0;
        free(syndromes);
        return result;
    }
    
    int *lambda = (int*)malloc((rs->t + 1) * sizeof(int));
    int *omega = (int*)malloc((rs->t + 1) * sizeof(int));
    
    if (!euclidean_algorithm(rs, syndromes, lambda, omega)) {
        fprintf(stderr, "✗ Алгоритм Евклида не сошёлся\n");
        free(syndromes);
        free(lambda);
        free(omega);
        return result;
    }
    
    int num_errors = find_error_positions(rs, lambda, result->error_positions);
    
    if (num_errors == 0) {
        printf("✓ Ошибок не найдено\n");
        extract_message(received, rs->n, rs->k, result->message);
        result->success = true;
        result->num_errors = 0;
        free(syndromes);
        free(lambda);
        free(omega);
        return result;
    }
    
    if (num_errors > rs->t) {
        fprintf(stderr, "✗ Число ошибок (%d) > способность коррекции (%d)\n",
                num_errors, rs->t);
        free(syndromes);
        free(lambda);
        free(omega);
        return result;
    }
    
    int *error_values = (int*)malloc(num_errors * sizeof(int));
    calculate_error_values(rs, omega, lambda,
                          result->error_positions, num_errors,
                          error_values);
    
    int *corrected = (int*)malloc(rs->n * sizeof(int));
    memcpy(corrected, received, rs->n * sizeof(int));
    correct_errors(corrected, result->error_positions, error_values, num_errors);
    
    extract_message(corrected, rs->n, rs->k, result->message);
    
    result->success = true;
    result->num_errors = num_errors;
    
    printf("\n✓ ДЕКОДИРОВАНИЕ УСПЕШНО\n");
    printf("  Исправлено ошибок: %d\n", num_errors);
    
    free(syndromes);
    free(lambda);
    free(omega);
    free(error_values);
    free(corrected);
    
    return result;
}

void print_decoder_result(RS_DecoderResult *result, int k) {
    if (!result) return;
    
    printf("\n╔════════════════════════════════════════════════════╗\n");
    printf("║           РЕЗУЛЬТАТ ДЕКОДИРОВАНИЯ                  ║\n");
    printf("╚════════════════════════════════════════════════════╝\n");
    
    printf("\nСтатус: %s\n", result->success ? "✓ УСПЕХ" : "✗ ОШИБКА");
    printf("Исправлено ошибок: %d\n", result->num_errors);
    printf("Позиции ошибок: ");
    for (int i = 0; i < result->num_errors; i++) {
        printf("%d ", result->error_positions[i]);
    }
    printf("\n");
    
    printf("\nИнформационное слово: [");
    for (int i = 0; i < k; i++) {
        printf("%d", result->message[i]);
        if (i < k - 1) printf(", ");
    }
    printf("]\n");
}

void decoder_result_free(RS_DecoderResult *result) {
    if (result) {
        free(result->message);
        free(result->error_positions);
        free(result);
    }
}