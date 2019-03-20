int sub(int a, int b)
{
    return (a-b);
}


// pass multiple paramter
int add(int a, int b, int c, int d, int e, int f)
{
        return a + b + c + d + e + f;
}

void reset_handler(void)
{
        int a = 7, b = 5;
        int c;

                // pass two integers
        c = sub(a, b);

                    // pass six integers
        c = add(1, 2, 3, a, b, c);

        while (1)
         ;
}
