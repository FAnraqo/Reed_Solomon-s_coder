#include <stdio.h>

void print_field(int degree, int *field){
    int size = 1 << degree;

    printf("╔══════╤");
    for (int i = 1; i < size - 1; i++){
        printf("════╤");
    }
    printf("════╗\n");

    printf("║ Null │");
    for (int i = 1; i < size; i++){
        
        if (i != size - 1)
            if (i - 1 < 10)
                printf("  %d │", i - 1);
            else
                printf(" %d │", i - 1);
        else
            if (i - 1 < 10)
                printf("  %d ║\n", i - 1);
            else
                printf(" %d ║\n", i - 1);
    }

    printf("╟──────┼");
    for (int i = 1; i < size - 1; i++){
        printf("────┼");
    }
    printf("────╢\n");

    for (int i = 0; i < size; i++){
        if (i == 0)
            printf("║   %d  │", field[i]);
        else
            if (i == size - 1)
                if (field[i] < 10)
                    printf("  %d ║\n", field[i]);
                else
                    printf(" %d ║\n", field[i]);
            else
                if (field[i] < 10)
                    printf("  %d │", field[i]);
                else
                    printf(" %d │", field[i]);
    }

    printf("╚══════╧");
    for (int i = 1; i < size - 1; i++){
        printf("════╧");
    }
    printf("════╝\n");
        
    printf("\n");
}
