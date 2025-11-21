#include <stdio.h>

#include "functions.h"

int main(){
    int degree = 4;
    int size = 1 << degree;
    int field[size];

    create_field(degree, field);
    print_field(degree, field);

    int a = 0, b = 3;

    //adding
    int result = adding_elements(field, a, b);

    if (result != -1)
        printf("add: %d + %d = %d\n\n", a, b, result);
    else
        printf("add: %d + %d = Null\n\n", a, b);

    print_adding_table(field, degree);



    a = 0, b = 3;

    //multiplication
    result = multiplication_elements(a, b);

    if (result != -1)
        printf("multi: %d * %d = %d\n\n", a, b, result);
    else
        printf("multi: %d * %d = Null\n\n", a, b);
    
    print_multiplication_table(field, degree);



    a = 1;

    //reverse
    printf("reverse: ~%d = %d\n\n", a, reverse_element(degree, a));


    for (int i = 0; i < size - 1; i++){
        printf("\t%d", i);
    }

    printf("\nreverse");

    
    for (int i = 0; i < size - 1; i++){
        printf("\t%d", reverse_element(degree, i));
    }
    printf("\n\n");

    return 0;
}