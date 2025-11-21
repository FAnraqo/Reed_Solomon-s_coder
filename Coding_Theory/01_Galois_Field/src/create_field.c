void create_field(int degree, int *field){
    int size = 1 << degree;
    field[0] = 0;
    field[1] = 1;
    for(int i = 2; i < size; i++){
        field[i] = field[i-1] << 1;
        if ((field[i] >> degree) == 1){
            field[i] ^= 0b1 << degree;
            field[i] ^= 0b11;
        }
    }
}