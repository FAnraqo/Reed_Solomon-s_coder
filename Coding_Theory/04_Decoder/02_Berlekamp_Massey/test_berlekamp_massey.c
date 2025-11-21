#include <stdio.h>
#include "berlekamp_massey.h"

int main() {
    printf("Пример: BCH [15,11] код, t=2 (коррекция 2 ошибок)\n");
    printf("Поле: GF(2^4)\n\n");
    
    GFElement syndromes[] = {
        5,    // S1
        7,    // S2
        3,    // S3
        9,    // S4
    };
    
    BMResult *result = berlekamp_massey(syndromes, 4, 2, 4);
    
    if (result->success) {
        printf("✓ Декодирование успешно!\n");
        printf("✓ Найдено ошибок: %d\n", result->error_count);
    } else {
        printf("✗ Ошибка при декодировании\n");
    }
    
    bm_result_free(result);
    
    return 0;
}
