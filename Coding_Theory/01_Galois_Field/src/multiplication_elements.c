int multiplication_elements(int a, int b){
    int result;
    if (a != -1 && b != -1)
        result = (a + b) % 15;
    else
        result = -1;
    return result;
}