
// 题目：Given two binary strings, return their sum (also a binary string).
// For example,
// a = "11"
// b = "1"
// Return "100".

// 思路：二进制加法和普通加法的思路没什么差别，甚至更简单。所需要注意的细节就是进位。从最低位开始，进位可能伴随计算直到最高位。所以每一位的加法运算都要将上一次的进位加进去。

// 对于某一位a和b，上一为的进位c，该为求和之后的和是(a+b+c)%2，进位是(a+b+c)/2。

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char* argv[])
{
    char* ch1 = "11011";
    char* ch2 =  "1110";
    int c = 0;
    int temp = 0;
    char* sum = NULL;
    int length1 = (int)strlen(ch1);
    int length2 = (int)strlen(ch2);
    int length3 = length1 > length2 ? length1 : length2;
    sum = (char *)malloc(length3 + 2); // \0 carry
    sum[length3 + 1] = '\0';
    while(length1 > 0 && length2 > 0){
        temp = ch1[length1 - 1] - '0' + ch2[length2 - 1] - '0' + c;
        c = temp / 2;
        sum[length3] = (char)('0' + temp % 2 );
        length1--;
        length2--;
        length3--;
    }
    while(length1 > 0){
        temp = ch1[length1 - 1] - '0' + c;
        c = temp / 2;
        sum[length3] = (char)('0' + temp % 2);
        length1--;
        length3--;
    }
    while (length2 > 0)
    {
        temp = ch2[length2 - 1] - '0' + c;
        c = temp / 2;
        sum[length3] = (char)('0' + temp % 2);
        length2--;
        length3--;
    }
    if(c == 1){
        sum[length3] = '1';
    }
    printf("%s\n", sum);
    free(sum);
    return 0;
}