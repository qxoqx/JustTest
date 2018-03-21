#include <stdio.h>
/**
 * CSAPP：p150
 * 用一个32位int表示n!,最大的n的值是多少？
 * 用一个64位long表示n!,最大的n的值是多少？
 */
int main(int argv, char* argc[])
{
    int a = 1;
    long mul = 1;
    long last_mul;
    do{
        last_mul = mul;
        mul *= a ;
    } while (mul / a++ == last_mul);
    printf("max n = %d, mul = %ld\n", a - 2, last_mul);
    return 0;
}
