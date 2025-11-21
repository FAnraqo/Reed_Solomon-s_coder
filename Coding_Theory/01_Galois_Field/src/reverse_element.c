int reverse_element(int degree, int a){
    int size = 1 << degree;
    int result = ((-1 * a) + (size - 1)) % (size - 1);
    return result;
}