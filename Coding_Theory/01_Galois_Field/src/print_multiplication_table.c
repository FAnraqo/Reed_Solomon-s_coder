#include <stdio.h>

#include "functions.h"

void print_multiplication_table(int *field, int degree){
    int size = 1 << degree;

    printf("╔════╦");
    for (int i = 1; i < size - 1; i++){
        printf("════╤");
    }
    printf("════╗\n");

    printf("║  + ║");
    for (int i = 1; i < size; i++){
        if (i == size - 1){
            if (i - 1 < 10){
                printf("  %d ║\n", i - 1);
            }
            else{
                printf(" %d ║\n", i - 1);
            }
        }
        else{
            if (i - 1 < 10){
                printf("  %d │", i - 1);
            }
            else{
                printf(" %d │", i - 1);
            }
        }
    }

    printf("╠════╬");
    for (int i = 1; i < size - 1; i++){
        printf("════╪");
    }
    printf("════╣\n");

    for (int i = 0; i < size - 1; i++){
        if (i < 10){
            printf("║  %d ║", i);
        }
        else{
            printf("║ %d ║", i);
        }
        
        for (int j = 0; j < size - 1; j++){
            int temp = multiplication_elements(i, j);
            
            if (j == size - 2){
                if (temp == -1){
                    printf("  N ║\n");
                }
                else{
                    if (temp < 10){
                        printf("  %d ║\n", temp);
                    }
                    else{
                        printf(" %d ║\n", temp);
                    }
                }
            }
            else{
                if (temp == -1){
                    printf("  N │");
                }
                else{
                    if (temp < 10){
                        printf("  %d │", temp);
                    }
                    else{
                        printf(" %d │", temp);
                    }
                }
            }
        }

        if (i == size - 2){
            printf("╚════╩");
            for (int i = 1; i < size - 1; i++){
                printf("════╧");
            }
            printf("════╝\n");
        }
        else{
            printf("╟────╫");
            for (int i = 1; i < size - 1; i++){
                printf("────┼");
            }
            printf("────╢\n");
        }
    }
    printf("\n");
}