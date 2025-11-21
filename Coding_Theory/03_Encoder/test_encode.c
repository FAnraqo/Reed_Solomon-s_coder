#include <stdio.h>
#include "universal_encoder.h"

int main() {
    printf("\n--- Пример 1: Код Хемминга [7,4] ---\n");
    
    BitVector *Q1 = bitvector_create(4);
    Q1->data[0] = 1;
    Q1->data[1] = 0;
    Q1->data[2] = 1;
    Q1->data[3] = 1;
    
    BitVector *P1 = bitvector_create(4);  // g(x) = x^3 + x + 1
    P1->data[0] = 1;
    P1->data[1] = 0;
    P1->data[2] = 1;
    P1->data[3] = 1;
    
    BitVector *result1 = koder(Q1, P1);
    
    bitvector_free(Q1);
    bitvector_free(P1);
    bitvector_free(result1);
    
    printf("\n\n--- Пример 2: Информация [1,1,1,1] ---\n");
    
    BitVector *Q2 = bitvector_create(4);
    Q2->data[0] = 1;
    Q2->data[1] = 1;
    Q2->data[2] = 1;
    Q2->data[3] = 1;
    
    BitVector *P2 = bitvector_create(4);
    P2->data[0] = 1;
    P2->data[1] = 0;
    P2->data[2] = 1;
    P2->data[3] = 1;
    
    BitVector *result2 = koder(Q2, P2);
    
    bitvector_free(Q2);
    bitvector_free(P2);
    bitvector_free(result2);
    
    return 0;
}
