#include <stdio.h>
#include "generator_poly.h"

int main() {
    printf("=== МОДУЛЬ ВЫБОРА ПОРОЖДАЮЩЕГО МНОГОЧЛЕНА ===\n\n");
    
    printf("Пример 1: Код Хемминга [7,4]\n");
    CyclicCodeParams hamming = get_generator_polynomial(7, 1);
    print_polynomial_info(hamming);
    printf("\n");
    
    printf("Пример 2: BCH код [15,11]\n");
    CyclicCodeParams bch = get_generator_polynomial(15, 1);
    print_polynomial_info(bch);
    printf("\n");
    
    printf("Пример 3: Передача параметров в модуль кодирования\n");
    CyclicCodeParams code = get_generator_polynomial(7, 1);
    
    printf("Передаю в модуль кодирования:\n");
    printf("  n = %d\n", code.n);
    printf("  k = %d\n", code.k);
    printf("  g(x) = 0x%lx\n", code.g);
    printf("\n");
    
    printf("Пример 4: Все многочлены для n=7\n");
    for (int i = 0; i < 10; i++) {
        CyclicCodeParams p = get_generator_polynomial(7, i);
        if (p.g == 0) break;
        printf("  Индекс %d: ", i);
        print_polynomial_info(p);
        printf("\n");
    }
    
    return 0;
}
