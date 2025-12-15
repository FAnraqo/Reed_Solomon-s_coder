#include "rs_encoder.h"

/**
 * Построение поля Галуа GF(2^m)
 * @param log_table - таблица логарифмов
 * @param alog_table - таблица антилогарифмов
 * @param m - степень поля
 * @param prim_poly - примитивный многочлен
 */
void build_gf_field(int *log_table, int *alog_table, int m, int prim_poly) {
    int size = (1 << m) - 1;  // 2^m - 1
    int element = 1;
    
    for (int i = 0; i <= size; i++) {
        log_table[i] = 0;
    }
    
    for (int i = 0; i < size; i++) {
        alog_table[i] = element;
        log_table[element] = i;
        
        element <<= 1;
        
        if (element >= (1 << m)) {
            element ^= prim_poly;
        }
    }
    
    for (int i = 0; i < m; i++) {
        alog_table[size + i] = alog_table[i];
    }
}

void build_inverse_table(int *inverse, int *log_table, int *alog_table, int q) {
    inverse[0] = 0;
    
    for (int i = 1; i <= q - 1; i++) {
        int log_inv = (q - 1) - log_table[i];
        inverse[i] = alog_table[log_inv];
    }
}

int gf_add(int a, int b) {
    return a ^ b;
}

int gf_multiply(int a, int b, int *log_table, int *alog_table, int q) {
    if (a == 0 || b == 0)
        return 0;
    
    int log_sum = log_table[a] + log_table[b];

    log_sum %= (q - 1);
    
    return alog_table[log_sum];
}

int gf_divide(int a, int b, int *log_table, int *alog_table, int q) {
    if (b == 0) {
        fprintf(stderr, "Ошибка: деление на ноль\n");
        return 0;
    }
    
    if (a == 0)
        return 0;
    
    int log_diff = log_table[a] - log_table[b];

    if (log_diff < 0)
        log_diff += (q - 1);
    
    return alog_table[log_diff];
}

int gf_inverse(int a, int *log_table, int *alog_table, int q) {
    if (a == 0) {
        fprintf(stderr, "Ошибка: обратного для 0 не существует\n");
        return 0;
    }
    
    int log_inv = (q - 1) - log_table[a];
    return alog_table[log_inv];
}

void polynomial_multiply(int *poly1, int poly1_deg,
                        int *poly2, int poly2_deg,
                        int *result,
                        int *log_table, int *alog_table, int q) {
    int result_deg = poly1_deg + poly2_deg;
    
    for (int i = 0; i <= result_deg; i++) {
        result[i] = 0;
    }
    
    for (int i = 0; i <= poly1_deg; i++) {
        if (poly1[i] == 0) continue;
        
        for (int j = 0; j <= poly2_deg; j++) {
            if (poly2[j] == 0) continue;
            
            int product = gf_multiply(poly1[i], poly2[j], 
                                     log_table, alog_table, q);
            result[i + j] = gf_add(result[i + j], product);
        }
    }
}

void polynomial_divide(int *dividend, int dividend_deg,
                      int *divisor, int divisor_deg,
                      int *quotient, int *remainder,
                      int *log_table, int *alog_table, int q) {
    
    memset(quotient, 0, (dividend_deg + 1) * sizeof(int));
    memset(remainder, 0, divisor_deg * sizeof(int));
    
    int *buffer = (int*)malloc((dividend_deg + 2) * sizeof(int));
    memcpy(buffer, dividend, (dividend_deg + 1) * sizeof(int));
    
    for (int i = dividend_deg; i >= divisor_deg; i--) {
        if (buffer[i] == 0)
            continue;
        
        int coeff = gf_divide(buffer[i], divisor[divisor_deg],
                            log_table, alog_table, q);
        
        quotient[i - divisor_deg] = coeff;
        
        for (int j = 0; j <= divisor_deg; j++) {
            int product = gf_multiply(divisor[j], coeff,
                                    log_table, alog_table, q);
            buffer[i - divisor_deg + j] = gf_add(buffer[i - divisor_deg + j],
                                                 product);
        }
    }
    
    for (int i = 0; i < divisor_deg; i++) {
        remainder[i] = buffer[i];
    }
    
    free(buffer);
}

void build_generator_polynomial(RS_Coder *rs, int *log_table, int *alog_table) {

    int *g = (int*)malloc((rs->r + 1) * sizeof(int));
    memset(g, 0, (rs->r + 1) * sizeof(int));
    g[0] = 1;
    int g_deg = 0;
    
    for (int i = 1; i <= 2 * rs->t; i++) {
        int alpha_i = alog_table[i % (rs->q - 1)];
        
        int *factor = (int*)malloc(2 * sizeof(int));
        factor[0] = alpha_i;
        factor[1] = 1;
        
        int *g_new = (int*)malloc((g_deg + 2) * sizeof(int));
        memset(g_new, 0, (g_deg + 2) * sizeof(int));
        
        polynomial_multiply(g, g_deg, factor, 1, g_new, 
                          log_table, alog_table, rs->q);
        
        free(g);
        free(factor);
        g = g_new;
        g_deg++;
    }
    
    memcpy(rs->g, g, (rs->r + 1) * sizeof(int));
    rs->g_deg = g_deg;
    free(g);
}

RS_Coder* rs_init(int n, int k, int m, int prim_poly) {
    RS_Coder *rs = (RS_Coder*)malloc(sizeof(RS_Coder));
    
    rs->q = (1 << m) - 1;
    rs->n = n;
    rs->k = k;
    rs->r = n - k;
    rs->t = rs->r / 2;
    rs->m = m;
    
    rs->log_table = (int*)malloc((rs->q + 1) * sizeof(int));
    rs->alog_table = (int*)malloc((2 * rs->q) * sizeof(int));
    rs->inverse = (int*)malloc((rs->q + 1) * sizeof(int));
    rs->g = (int*)malloc((rs->r + 1) * sizeof(int));
    
    build_gf_field(rs->log_table, rs->alog_table, m, prim_poly);
    build_inverse_table(rs->inverse, rs->log_table, rs->alog_table, rs->q + 1);
    
    build_generator_polynomial(rs, rs->log_table, rs->alog_table);
    
    return rs;
}

void rs_free(RS_Coder *rs) {
    if (rs) {
        free(rs->log_table);
        free(rs->alog_table);
        free(rs->inverse);
        free(rs->g);
        free(rs);
    }
}

void rs_encode(RS_Coder *rs, int *message, int *codeword) {

    int *shifted = (int*)malloc((rs->k + rs->r + 1) * sizeof(int));
    memset(shifted, 0, (rs->k + rs->r + 1) * sizeof(int));
    
    for (int i = 0; i < rs->k; i++) {
        shifted[i + rs->r] = message[i];
    }
    
    int *remainder = (int*)malloc(rs->r * sizeof(int));
    int *dummy_quotient = (int*)malloc((rs->k + rs->r + 1) * sizeof(int));
    memset(remainder, 0, rs->r * sizeof(int));
    

    polynomial_divide(shifted, rs->k + rs->r - 1,
                    rs->g, rs->g_deg,
                    dummy_quotient, remainder,
                    rs->log_table, rs->alog_table, rs->q);
    
    for (int i = 0; i < rs->r; i++) {
        codeword[i] = remainder[i];
    }
    for (int i = 0; i < rs->k; i++) {
        codeword[i + rs->r] = message[i];
    }
    
    free(shifted);
    free(dummy_quotient);
    free(remainder);
}

int verify_codeword(RS_Coder *rs, int *codeword) {
    int *remainder = (int*)malloc(rs->r * sizeof(int));
    int *dummy_quotient = (int*)malloc(rs->n * sizeof(int));
    memset(remainder, 0, rs->r * sizeof(int));
    
    polynomial_divide(codeword, rs->n - 1,
                 rs->g, rs->g_deg,
                 dummy_quotient, remainder,
                 rs->log_table, rs->alog_table, rs->q);

    int is_valid = 1;
    for (int i = 0; i < rs->r; i++) {
        if (remainder[i] != 0) {
            is_valid = 0;
            break;
        }
    }

    free(dummy_quotient);
    free(remainder);
    return is_valid;
}

int check_cyclicity(RS_Coder *rs, int *codeword) {
    int *shifted = (int*)malloc(rs->n * sizeof(int));
    
    shifted[0] = codeword[rs->n - 1];
    for (int i = 1; i < rs->n; i++) {
        shifted[i] = codeword[i - 1];
    }
    
    int result = verify_codeword(rs, shifted);
    free(shifted);
    return result;
}

void print_gf_field(int *log_table, int *alog_table, int q) {
    printf("\n=== Таблица поля GF(2^4) ===\n");
    printf("Степень | Элемент | Логарифм\n");
    printf("--------|---------|----------\n");
    
    for (int i = 0; i < q; i++) {
        printf("   α%-2d  |   %2d    |    %2d\n", i, alog_table[i], i);
    }
    printf("\n");
}

void print_generator_polynomial(RS_Coder *rs) {
    printf("\nПорождающий многочлен g(x) = ");
    
    int printed = 0;
    for (int i = rs->g_deg; i >= 0; i--) {
        if (rs->g[i] == 0) continue;
        
        if (printed) printf(" + ");
        
        if (i == 0) {
            printf("%d", rs->g[i]);
        } else if (i == 1) {
            printf("%d·x", rs->g[i]);
        } else {
            printf("%d·x^%d", rs->g[i], i);
        }
        
        printed = 1;
    }
    printf("\n");
}

void print_codeword(int *codeword, int n) {
    printf("Кодовое слово: [");
    for (int i = 0; i < n; i++) {
        printf("%d", codeword[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int test_linearity(RS_Coder *rs) {
    printf("\n=== Тест линейности ===\n");
    
    int failures = 0;
    for (int test = 0; test < 1000; test++) {
        int m1[15], m2[15], m_sum[15];
        int c1[15], c2[15], c_sum[15];
        
        for (int i = 0; i < rs->k; i++) {
            m1[i] = rand() % (rs->q + 1);
            m2[i] = rand() % (rs->q + 1);
        }
        
        rs_encode(rs, m1, c1);
        rs_encode(rs, m2, c2);
        
        for (int i = 0; i < rs->k; i++) {
            m_sum[i] = gf_add(m1[i], m2[i]);
        }
        rs_encode(rs, m_sum, c_sum);
        
        for (int i = 0; i < rs->n; i++) {
            int expected = gf_add(c1[i], c2[i]);
            if (c_sum[i] != expected) {
                failures++;
                break;
            }
        }
    }
    
    if (failures == 0) {
        printf("✓ Все 1000 тестов пройдены\n");
        return 1;
    } else {
        printf("✗ %d тестов не пройдено\n", failures);
        return 0;
    }
}