#include <stdio.h>
#include "euclidean_decoder.h"

int main() {
    printf("Пример: BCH [15,11] код, t=2 (коррекция 2 ошибок)\n");
    printf("Поле: GF(2^4)\n\n");
    
    uint8_t syndromes[] = {
        5,    // S1
        7,    // S2
        3,    // S3
        9,    // S4
    };
    
    EuclideanResult *result = euclidean_decode(syndromes, 4, 2, 4);
    
    if (result) {
        print_euclidean_result(result);
        euclidean_result_free(result);
    }
    
    printf("\n");
    return 0;
}
