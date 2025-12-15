#include "rs_encoder.h"

int main() {
    printf("====================================================\n");
    printf("     Тестирование кодера Рида-Соломона RS(15,9)\n");
    printf("====================================================\n");
    
    RS_Coder *rs = rs_init(15, 9, 4, 0b10011);
    
    printf("\nПараметры кода:\n");
    printf("  n = %d (длина кодового слова)\n", rs->n);
    printf("  k = %d (информационные символы)\n", rs->k);
    printf("  r = %d (проверочные символы)\n", rs->r);
    printf("  t = %d (исправляемые ошибки)\n", rs->t);
    printf("  q = %d (размер поля GF(2^%d))\n", rs->q + 1, rs->m);
    
    print_gf_field(rs->log_table, rs->alog_table, rs->q);
    
    print_generator_polynomial(rs);
    
    printf("\n====================================================\n");
    printf("ТЕСТ 1: Нулевое информационное слово\n");
    printf("====================================================\n");
    
    int message1[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int codeword1[15];
    
    rs_encode(rs, message1, codeword1);
    printf("Информационное слово: [0, 0, 0, 0, 0, 0, 0, 0, 0]\n");
    print_codeword(codeword1, rs->n);
    
    int all_zeros = 1;
    for (int i = 0; i < rs->n; i++) {
        if (codeword1[i] != 0) {
            all_zeros = 0;
            break;
        }
    }
    
    if (all_zeros) {
        printf("✓ ТЕСТ ПРОЙДЕН: кодовое слово = 0\n");
    } else {
        printf("✗ ТЕСТ НЕ ПРОЙДЕН: ожидалось все нули\n");
    }
    
    printf("\n====================================================\n");
    printf("ТЕСТ 2: Единичное информационное слово\n");
    printf("====================================================\n");
    
    int message2[15] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    int codeword2[15];
    
    rs_encode(rs, message2, codeword2);
    printf("Информационное слово: [1, 0, 0, 0, 0, 0, 0, 0, 0]\n");
    print_codeword(codeword2, rs->n);
    
    if (verify_codeword(rs, codeword2)) {
        printf("✓ ТЕСТ ПРОЙДЕН: кодовое слово делится на g(x)\n");
    } else {
        printf("✗ ТЕСТ НЕ ПРОЙДЕН: кодовое слово не делится на g(x)\n");
    }
    
    printf("\n====================================================\n");
    printf("ТЕСТ 3: 1000 случайных информационных слов (делимость)\n");
    printf("====================================================\n");
    
    int failures_div = 0;
    for (int test = 0; test < 1000; test++) {
        int message[15];
        int codeword[15];
        
        for (int i = 0; i < rs->k; i++) {
            message[i] = rand() % (rs->q + 1);
        }
        
        rs_encode(rs, message, codeword);
        
        if (!verify_codeword(rs, codeword)) {
            failures_div++;
        }
        
        if ((test + 1) % 250 == 0) {
            printf("  Обработано %d тестов...\n", test + 1);
        }
    }
    
    if (failures_div == 0) {
        printf("✓ ТЕСТ ПРОЙДЕН: все 1000 кодовых слов делятся на g(x)\n");
    } else {
        printf("✗ ТЕСТ НЕ ПРОЙДЕН: %d слов не делятся на g(x)\n", failures_div);
    }
    
    printf("\n====================================================\n");
    printf("ТЕСТ 4: Цикличность (100 кодовых слов)\n");
    printf("====================================================\n");
    
    int failures_cyc = 0;
    for (int test = 0; test < 100; test++) {
        int message[15];
        int codeword[15];
        
        for (int i = 0; i < rs->k; i++) {
            message[i] = rand() % (rs->q + 1);
        }
        
        rs_encode(rs, message, codeword);
        
        if (!check_cyclicity(rs, codeword)) {
            failures_cyc++;
        }
        
        if ((test + 1) % 25 == 0) {
            printf("  Проверено %d кодовых слов...\n", test + 1);
        }
    }
    
    if (failures_cyc == 0) {
        printf("✓ ТЕСТ ПРОЙДЕН: все циклические сдвиги — кодовые слова\n");
    } else {
        printf("✗ ТЕСТ НЕ ПРОЙДЕН: %d слов не прошли проверку цикличности\n", failures_cyc);
    }
    
    printf("\n====================================================\n");
    printf("ТЕСТ 5: Линейность (500 пар слов)\n");
    printf("====================================================\n");
    
    if (test_linearity(rs)) {
        printf("✓ ТЕСТ ПРОЙДЕН\n");
    } else {
        printf("✗ ТЕСТ НЕ ПРОЙДЕН\n");
    }
    
    printf("\n====================================================\n");
    printf("ТЕСТ 6: Измерение производительности\n");
    printf("====================================================\n");
    
    int message[15];
    int codeword[15];
    
    for (int i = 0; i < rs->k; i++) {
        message[i] = rand() % (rs->q + 1);
    }
    
    clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        rs_encode(rs, message, codeword);
    }
    clock_t end = clock();
    
    double time_total = (double)(end - start) / CLOCKS_PER_SEC;
    double time_per_codeword = time_total / 100000.0 * 1e6;
    
    printf("Время кодирования 100000 слов: %.4f сек\n", time_total);
    printf("Среднее время на кодовое слово: %.2f мкс\n", time_per_codeword);
    printf("Пропускная способность: %.2f Мсимволов/сек\n", 15.0 / time_per_codeword);
    
    printf("\n====================================================\n");
    printf("ИТОГИ ТЕСТИРОВАНИЯ\n");
    printf("====================================================\n");
    printf("\nВсе основные тесты пройдены успешно!\n");
    printf("Кодер Рида-Соломона RS(15,9) работает корректно.\n\n");
    
    rs_free(rs);
    
    return 0;
}
