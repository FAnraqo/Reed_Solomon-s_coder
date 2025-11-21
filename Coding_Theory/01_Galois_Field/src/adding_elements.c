int adding_elements(int *field, int a, int b) //Sgf(x,y)
{
    int result;
    if (a != -1 && b != -1){
        int R = field[a + 1] ^ field[b + 1];
        if (R != 0){
            int k = 1;
            while ((R ^ field[k]) != 0)
                k ++;
            result = k - 1;
        }
        else
            if (R == 0)
                result = -1;
    }
    else
        if (a == -1 || b == -1){
            if (a == -1)
                result = b;
            else
                if (b == -1)
                    result = a;
        }
    return result;
}