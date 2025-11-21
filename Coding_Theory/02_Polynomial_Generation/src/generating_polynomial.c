#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t Polynomial;

typedef struct {
    Polynomial divisor;
    int degree;
} PolynomialFactor;

typedef struct {
    PolynomialFactor *factors;
    int count;
    int capacity;
} DivisorList;

typedef struct {
    Polynomial *codewords;
    int count;
    int code_length;
    int info_length;
} CyclicCode;

typedef struct {
    int n;
    int k;
    Polynomial g;
    int error_correcting_capacity;
} CodeParameters;

int poly_degree(Polynomial p) {
    if (p == 0) return -1;
    int degree = 0;
    while (p > 1) {
        p >>= 1;
        degree++;
    }
    return degree;
}

int poly_weight(Polynomial p) {
    int weight = 0;
    while (p) {
        weight += p & 1;
        p >>= 1;
    }
    return weight;
}

void poly_print(Polynomial p) {
    if (p == 0) {
        printf("0");
        return;
    }
    bool first = true;
    for (int i = 0; i <= 64; i++) {
        if ((p >> i) & 1) {
            if (!first) printf(" + ");
            if (i == 0) printf("1");
            else if (i == 1) printf("x");
            else printf("x^%d", i);
            first = false;
        }
    }
}

void poly_print_binary(Polynomial p, int length) {
    for (int i = length - 1; i >= 0; i--) {
        printf("%llu", (p >> i) & 1ULL);
    }
}

void poly_print_hex(Polynomial p) {
    printf("0x%llx", (unsigned long long)p);
}

Polynomial poly_multiply_gf2(Polynomial p1, Polynomial p2) {
    Polynomial result = 0;
    while (p2) {
        if (p2 & 1) {
            result ^= p1;
        }
        p1 <<= 1;
        p2 >>= 1;
    }
    return result;
}

Polynomial poly_mod_gf2(Polynomial dividend, Polynomial divisor) {
    if (divisor == 0) return 0;
    
    int divisor_degree = poly_degree(divisor);
    
    while (poly_degree(dividend) >= divisor_degree) {
        int shift = poly_degree(dividend) - divisor_degree;
        dividend ^= (divisor << shift);
    }
    
    return dividend;
}

Polynomial poly_gcd_gf2(Polynomial a, Polynomial b) {
    while (b != 0) {
        Polynomial temp = poly_mod_gf2(a, b);
        a = b;
        b = temp;
    }
    return a;
}

bool poly_divides(Polynomial dividend, Polynomial divisor) {
    return poly_mod_gf2(dividend, divisor) == 0;
}

DivisorList* divisor_list_create(int capacity) {
    DivisorList *list = malloc(sizeof(DivisorList));
    if (!list) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return NULL;
    }
    list->factors = malloc(capacity * sizeof(PolynomialFactor));
    if (!list->factors) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        free(list);
        return NULL;
    }
    list->count = 0;
    list->capacity = capacity;
    return list;
}

void divisor_list_add(DivisorList *list, Polynomial divisor, int degree) {
    if (!list) return;
    
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->factors = realloc(list->factors, 
                               list->capacity * sizeof(PolynomialFactor));
        if (!list->factors) {
            fprintf(stderr, "Ошибка перераспределения памяти\n");
            return;
        }
    }
    list->factors[list->count].divisor = divisor;
    list->factors[list->count].degree = degree;
    list->count++;
}

void divisor_list_free(DivisorList *list) {
    if (list) {
        free(list->factors);
        free(list);
    }
}

void divisor_list_print(DivisorList *list) {
    if (!list || list->count == 0) {
        printf("Список делителей пуст\n");
        return;
    }
    
    printf("\n=== Найденные делители x^n - 1 ===\n");
    printf("%-3s | %-10s | %-10s | Полином\n", "№", "Степень", "Значение");
    printf("----+------------+------------+--------\n");
    
    for (int i = 0; i < list->count; i++) {
        printf("%-3d | %-10d | ", i + 1, list->factors[i].degree);
        poly_print_hex(list->factors[i].divisor);
        printf(" | ");
        poly_print(list->factors[i].divisor);
        printf("\n");
    }
}

DivisorList* find_divisors_of_xn_minus_1(int n) {
    DivisorList *divisors = divisor_list_create(32);
    if (!divisors) {
        return NULL;
    }
    
    printf("Построение факторизации x^%d + 1 в GF(2)...\n", n);
    
    divisor_list_add(divisors, 0x3, 1);

    if (n == 3) {
        divisor_list_add(divisors, 0x7, 2);
    } 
    else if (n == 5) {
        divisor_list_add(divisors, 0x1F, 4);
    } 
    else if (n == 7) {
        divisor_list_add(divisors, 0x0B, 3);
        divisor_list_add(divisors, 0x0D, 3);
    }
    else if (n == 15) {
        divisor_list_add(divisors, 0x13, 4);
        divisor_list_add(divisors, 0x19, 4);
        divisor_list_add(divisors, 0x1F, 4);
    }
    else if (n == 31) {
        divisor_list_add(divisors, 0x25, 5);
        divisor_list_add(divisors, 0x29, 5);
    }
    else {
        printf("Внимание: факторизация для n=%d не предусмотрена\n", n);
    }
    
    return divisors;
}

Polynomial select_generator_polynomial(int n, int divisor_index, 
                                       DivisorList *divisors) {
    if (!divisors || divisor_index < 0 || divisor_index >= divisors->count) {
        fprintf(stderr, "Ошибка: неверный индекс делителя\n");
        return 0;
    }
    
    Polynomial xn_plus_1 = (1ULL << n) | 1;
    Polynomial g = divisors->factors[divisor_index].divisor;
    
    if (!poly_divides(xn_plus_1, g)) {
        fprintf(stderr, "Ошибка: полином не делит x^%d + 1\n", n);
        return 0;
    }
    
    return g;
}

CodeParameters compute_code_parameters(int n, Polynomial g) {
    CodeParameters params;
    params.n = n;
    params.k = n - poly_degree(g);
    params.g = g;
    params.error_correcting_capacity = 0;
    
    if (params.k > 0) {
        params.error_correcting_capacity = (params.n - params.k) / 2;
    }
    
    return params;
}

void print_code_parameters(CodeParameters params) {
    printf("\n=== Параметры циклического кода ===\n");
    printf("Длина кода (n): %d\n", params.n);
    printf("Размерность (k): %d\n", params.k);
    printf("Число кодовых слов: 2^%d = %d\n", params.k, 1 << params.k);
    printf("Степень g(x): %d\n", poly_degree(params.g));
    printf("Генератор g(x) = ");
    poly_print(params.g);
    printf("\n");
    printf("Код обозначается как [%d, %d] код\n", params.n, params.k);
}

CyclicCode* cyclic_code_create(int n, int k) {
    if (k < 0 || n <= 0 || k > n) {
        fprintf(stderr, "Ошибка: неверные параметры кода\n");
        return NULL;
    }
    
    CyclicCode *code = malloc(sizeof(CyclicCode));
    if (!code) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return NULL;
    }
    
    int num_codewords = 1 << k;
    code->codewords = malloc(num_codewords * sizeof(Polynomial));
    if (!code->codewords) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        free(code);
        return NULL;
    }
    
    code->count = num_codewords;
    code->code_length = n;
    code->info_length = k;
    
    return code;
}

void generate_all_codewords(CyclicCode *code, Polynomial g) {
    if (!code) return;
    
    int k = code->info_length;
    int n = code->code_length;
    int num_codewords = 1 << k;
    Polynomial xn_plus_1 = (1ULL << n) | 1;
    
    for (int msg_idx = 0; msg_idx < num_codewords; msg_idx++) {
        Polynomial m = (Polynomial)msg_idx;
        Polynomial c = poly_multiply_gf2(m, g);
        c = poly_mod_gf2(c, xn_plus_1);
        c = c & ((1ULL << n) - 1);
        code->codewords[msg_idx] = c;
    }
}

void print_codewords_table(CyclicCode *code) {
    if (!code) return;
    
    printf("\n=== Таблица кодирования: все %d кодовых слов ===\n", code->count);
    printf("Информация | Кодовое слово\n");
    printf("----------+");
    for (int i = 0; i < code->code_length; i++) printf("-");
    printf("\n");
    
    for (int i = 0; i < code->count; i++) {
        Polynomial c = code->codewords[i];
        
        /* Информационные биты */
        for (int j = code->info_length - 1; j >= 0; j--) {
            printf("%llu", (i >> j) & 1ULL);
        }
        
        printf(" | ");
        poly_print_binary(c, code->code_length);
        printf("\n");
    }
}

void print_codewords_compact(CyclicCode *code) {
    if (!code) return;
    
    printf("\n=== Кодовые слова (компактный вид) ===\n");
    
    int cols = 4;
    for (int row = 0; row < (code->count + cols - 1) / cols; row++) {
        for (int col = 0; col < cols; col++) {
            int idx = row * cols + col;
            if (idx >= code->count) break;
            
            printf("[");
            poly_print_binary(code->codewords[idx], code->code_length);
            printf("]  ");
        }
        printf("\n");
    }
}

void verify_all_codewords(CyclicCode *code, Polynomial g) {
    if (!code) return;
    
    printf("\n=== Проверка корректности кодовых слов ===\n");
    
    int all_valid = 1;
    int errors = 0;
    
    for (int i = 0; i < code->count; i++) {
        Polynomial c = code->codewords[i];
        Polynomial remainder = poly_mod_gf2(c, g);
        
        if (remainder != 0) {
            printf("✗ Кодовое слово %d НЕ делится на g(x)\n", i);
            errors++;
            all_valid = 0;
        }
    }
    
    if (all_valid) {
        printf("✓ Все %d кодовых слов правильно делятся на g(x)\n", 
               code->count);
    } else {
        printf("✗ Найдено ошибок: %d из %d\n", errors, code->count);
    }
}

int compute_min_hamming_distance(CyclicCode *code) {
    if (!code || code->count < 2) return 0;
    
    int min_dist = code->code_length + 1;
    
    for (int i = 1; i < code->count; i++) {
        Polynomial c1 = code->codewords[0];
        Polynomial c2 = code->codewords[i];
        Polynomial diff = c1 ^ c2;
        int dist = poly_weight(diff);
        
        if (dist > 0 && dist < min_dist) {
            min_dist = dist;
        }
    }
    
    return min_dist;
}

void cyclic_code_free(CyclicCode *code) {
    if (code) {
        free(code->codewords);
        free(code);
    }
}

Polynomial systematic_encode(Polynomial msg, Polynomial g, int n, int k) {
    int r = n - k;
    
    Polynomial shifted = msg << r;
    
    Polynomial parity = poly_mod_gf2(shifted, g);
    
    Polynomial codeword = shifted ^ parity;
    codeword = codeword & ((1ULL << n) - 1);
    
    return codeword;
}

void print_systematic_example(Polynomial msg, Polynomial g, int n, int k) {
    Polynomial codeword = systematic_encode(msg, g, n, k);
    
    printf("\n=== Пример систематического кодирования ===\n");
    printf("Информационное слово m = ");
    poly_print_binary(msg, k);
    printf("\n");
    
    printf("Кодовое слово c = ");
    poly_print_binary(codeword, n);
    printf("\n");
    
    printf("Структура: [проверочные биты | информационные биты]\n");
    printf("           [");
    for (int i = n - 1; i >= k; i--) {
        printf("%llu", (codeword >> i) & 1ULL);
    }
    printf("|");
    for (int i = k - 1; i >= 0; i--) {
        printf("%llu", (codeword >> i) & 1ULL);
    }
    printf("]\n");
}

void print_menu() {
    printf("\n=== ЦИКЛИЧЕСКИЕ КОДЫ: ВЫБОР ПОРОЖДАЮЩЕГО МНОГОЧЛЕНА ===\n");
    printf("1. Выбрать длину кода и показать делители\n");
    printf("2. Выбрать порождающий многочлен\n");
    printf("3. Сгенерировать кодовые слова\n");
    printf("4. Проверить кодовые слова\n");
    printf("5. Показать систематическое кодирование\n");
    printf("6. Выход\n");
    printf("Выберите опцию: ");
}

int main() {
    int n = 7;
    int divisor_index = 0;
    DivisorList *divisors = NULL;
    CyclicCode *code = NULL;
    Polynomial g = 0;
        
    int choice = 0;
    bool running = true;
    bool code_generated = false;
    
    while (running) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Ошибка ввода\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("\nВыберите длину кода:\n");
                printf("1 - n=3 (Хемминг [3,1])\n");
                printf("2 - n=5\n");
                printf("3 - n=7 (Хемминг [7,4])\n");
                printf("4 - n=15 (BCH код)\n");
                printf("5 - n=31\n");
                printf("Выберите: ");
                
                int n_choice;
                if (scanf("%d", &n_choice) != 1) {
                    fprintf(stderr, "Ошибка ввода\n");
                    while (getchar() != '\n');
                    break;
                }
                
                switch (n_choice) {
                    case 1: n = 3; break;
                    case 2: n = 5; break;
                    case 3: n = 7; break;
                    case 4: n = 15; break;
                    case 5: n = 31; break;
                    default: printf("Неверный выбор\n"); continue;
                }
                
                if (divisors) divisor_list_free(divisors);
                divisors = find_divisors_of_xn_minus_1(n);
                
                if (divisors) {
                    divisor_list_print(divisors);
                }
                code_generated = false;
                break;
            }
            
            case 2: {
                if (!divisors || divisors->count == 0) {
                    printf("Ошибка: сначала выберите длину кода (опция 1)\n");
                    break;
                }
                
                printf("\nВыберите порождающий многочлен (номер делителя): ");
                if (scanf("%d", &divisor_index) != 1) {
                    fprintf(stderr, "Ошибка ввода\n");
                    while (getchar() != '\n');
                    break;
                }
                divisor_index--;
                
                g = select_generator_polynomial(n, divisor_index, divisors);
                if (g == 0) {
                    printf("Ошибка при выборе многочлена\n");
                    break;
                }
                
                CodeParameters params = compute_code_parameters(n, g);
                print_code_parameters(params);
                
                code_generated = false;
                break;
            }
            
            case 3: {
                if (g == 0) {
                    printf("Ошибка: сначала выберите порождающий многочлен (опция 2)\n");
                    break;
                }
                
                CodeParameters params = compute_code_parameters(n, g);
                
                if (code) cyclic_code_free(code);
                code = cyclic_code_create(n, params.k);
                
                if (code) {
                    printf("\nГенерирование %d кодовых слов...\n", code->count);
                    generate_all_codewords(code, g);
                    code_generated = true;
                    
                    if (code->count <= 16) {
                        print_codewords_table(code);
                    } else {
                        print_codewords_compact(code);
                    }
                }
                break;
            }
            
            case 4: {
                if (!code || !code_generated) {
                    printf("Ошибка: сначала сгенерируйте кодовые слова (опция 3)\n");
                    break;
                }
                
                verify_all_codewords(code, g);
                
                int d_min = compute_min_hamming_distance(code);
                printf("Минимальное расстояние Хемминга: d_min = %d\n", d_min);
                printf("Способность коррекции ошибок: t = %d\n", (d_min - 1) / 2);
                break;
            }
            
            case 5: {
                if (g == 0) {
                    printf("Ошибка: сначала выберите порождающий многочлен (опция 2)\n");
                    break;
                }
                
                CodeParameters params = compute_code_parameters(n, g);
                
                printf("\nПримеры систематического кодирования:\n");
                for (int msg = 0; msg < (1 << params.k) && msg < 4; msg++) {
                    print_systematic_example(msg, g, n, params.k);
                }
                break;
            }
            
            case 6: {
                printf("До свидания!\n");
                running = false;
                break;
            }
            
            default:
                printf("Неверный выбор. Попробуйте ещё раз.\n");
        }
    }
    
    if (divisors) divisor_list_free(divisors);
    if (code) cyclic_code_free(code);
    
    return 0;
}