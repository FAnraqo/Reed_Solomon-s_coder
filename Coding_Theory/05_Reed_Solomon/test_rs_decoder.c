#include "rs_encoder.h"
#include "rs_decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    printf("═══════════════════════════════════════════════════════════\n");
    printf("     ТЕСТИРОВАНИЕ ДЕКОДИРОВАНИЯ РИДА-СОЛОМОНА (ЕВКЛИД)\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    srand(time(NULL));
    
    RS_Coder *rs = rs_init(15, 9, 4, 0b10011);
    
    printf("\nПараметры:\n");
    printf("  n = %d (длина кодового слова)\n", rs->n);
    printf("  k = %d (информационные символы)\n", rs->k);
    printf("  r = %d (проверочные символы)\n", rs->r);
    printf("  t = %d (способность коррекции)\n", rs->t);
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("ТЕСТ 1: Безошибочное принятие\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    {
        int message1[9] = {1, 0, 1, 1, 0, 0, 1, 0, 1};
        int codeword1[15];
        
        printf("\nОригинальное сообщение: [");
        for (int i = 0; i < 9; i++) printf("%d ", message1[i]);
        printf("]\n");
        
        rs_encode(rs, message1, codeword1);
        printf("Кодовое слово: [");
        for (int i = 0; i < 15; i++) printf("%d ", codeword1[i]);
        printf("]\n");
        
        RS_DecoderResult *result = rs_decode_euclidean(rs, codeword1);
        print_decoder_result(result, rs->k);
        
        int match = 1;
        for (int i = 0; i < 9; i++) {
            if (result->message[i] != message1[i]) {
                match = 0;
                break;
            }
        }
        printf("%s\n", match ? "✓ ТЕСТ ПРОЙДЕН" : "✗ ТЕСТ НЕ ПРОЙДЕН");
        
        decoder_result_free(result);
    }
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("ТЕСТ 2: Одна ошибка\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    {
        int message2[9] = {1, 0, 1, 1, 0, 0, 1, 0, 1};
        int codeword2[15];
        
        printf("\nОригинальное сообщение: [");
        for (int i = 0; i < 9; i++) printf("%d ", message2[i]);
        printf("]\n");
        
        rs_encode(rs, message2, codeword2);
        
        int error_pos = 5;
        codeword2[error_pos] = (codeword2[error_pos] + 1) % (rs->q + 1);
        printf("Принятое слово (с ошибкой на позиции %d): [", error_pos);
        for (int i = 0; i < 15; i++) printf("%d ", codeword2[i]);
        printf("]\n");
        
        RS_DecoderResult *result = rs_decode_euclidean(rs, codeword2);
        print_decoder_result(result, rs->k);
        
        int match = 1;
        for (int i = 0; i < 9; i++) {
            if (result->message[i] != message2[i]) {
                match = 0;
                break;
            }
        }
        printf("%s: Восстановленное сообщение совпадает\n",
               match ? "✓ ТЕСТ ПРОЙДЕН" : "✗ ТЕСТ НЕ ПРОЙДЕН");
        
        decoder_result_free(result);
    }
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("ТЕСТ 3: Две ошибки\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    {
        int message3[9] = {1, 1, 1, 0, 0, 1, 1, 1, 0};
        int codeword3[15];
        
        printf("\nОригинальное сообщение: [");
        for (int i = 0; i < 9; i++) printf("%d ", message3[i]);
        printf("]\n");
        
        rs_encode(rs, message3, codeword3);
        
        int error_pos1 = 3;
        int error_pos2 = 10;
        codeword3[error_pos1] = (codeword3[error_pos1] + 1) % (rs->q + 1);
        codeword3[error_pos2] = (codeword3[error_pos2] + 2) % (rs->q + 1);
        
        printf("Принятое слово (ошибки на позициях %d и %d): [", error_pos1, error_pos2);
        for (int i = 0; i < 15; i++) printf("%d ", codeword3[i]);
        printf("]\n");
        
        RS_DecoderResult *result = rs_decode_euclidean(rs, codeword3);
        print_decoder_result(result, rs->k);
        
        int match = 1;
        for (int i = 0; i < 9; i++) {
            if (result->message[i] != message3[i]) {
                match = 0;
                break;
            }
        }
        printf("%s: Восстановленное сообщение совпадает\n",
               match ? "✓ ТЕСТ ПРОЙДЕН" : "✗ ТЕСТ НЕ ПРОЙДЕН");
        
        decoder_result_free(result);
    }
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("ТЕСТ 4: 50 случайных тестов\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    {
        int successes = 0;
        int total_tests = 50;
        
        for (int test = 0; test < total_tests; test++) {
            int message[9];
            int codeword[15];
            
            for (int i = 0; i < 9; i++) {
                message[i] = rand() % (rs->q + 1);
            }
            
            rs_encode(rs, message, codeword);
            
            int num_errors = rand() % 3;
            for (int i = 0; i < num_errors; i++) {
                int pos = rand() % 15;
                int error_val = (rand() % (rs->q) + 1);
                codeword[pos] = (codeword[pos] + error_val) % (rs->q + 1);
            }
            
            RS_DecoderResult *result = rs_decode_euclidean(rs, codeword);
            
            int match = 1;
            for (int i = 0; i < 9; i++) {
                if (result->message[i] != message[i]) {
                    match = 0;
                    break;
                }
            }
            
            if (match) successes++;
            decoder_result_free(result);
            
            if ((test + 1) % 10 == 0) {
                printf("  Обработано %d тестов...\n", test + 1);
            }
        }
        
        printf("\n✓ РЕЗУЛЬТАТ: %d / %d тестов успешны (%.1f%%)\n",
               successes, total_tests, (100.0 * successes / total_tests));
    }
    
    rs_free(rs);
    
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("Тестирование завершено\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    return 0;
}