#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 1024

typedef struct bignum {
    char sign;
    long number_of_digits;
    char *digit;
} bignum;

typedef struct stack {
    bignum stk[MAX_STACK_SIZE];
    int top;
} STACK;

STACK s;

void push(bignum num);
bignum pop();
bignum makeNewBignum();
void display(); //Only for testing..
void clearStack();
int whosBigger(bignum* a, bignum* b);
void mult(bignum* a, bignum* b, bignum* res);
void divd(bignum* a, bignum* b, bignum* res, bignum* factor);
void div_helper(bignum* a, bignum* b, bignum* res, bignum* factor);
void DeepCopyBignums(bignum* a, bignum* b);
void domult2(bignum* a);
void cleanZeros(bignum* a);

extern void add1(bignum* a, bignum* b, bignum* res);
extern void sub1(bignum* a, bignum* b, bignum* res);
extern void mult2(bignum* a);
extern void div2(bignum* a);


char ch;
short readNextChar = 1;
int main()
{
    s.top = -1;
    bignum num;
    num.sign = 0;
    num.number_of_digits = 0;
    num.digit = NULL;
    bignum num2;
    num2.sign = 0;
    num2.number_of_digits = 0;
    num2.digit = NULL;
    bignum res;
    res.sign = 0;
    res.number_of_digits = 0;
    res.digit = NULL;
    while (1==1)
    {
        res.sign = 0;
        if (readNextChar)
            ch = getchar();
        readNextChar = 1;
        if ((ch >= '0' && ch <= '9') || ch == '_')
        {
            //number
            num = makeNewBignum();
            push(num);
            free(num.digit);
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        {
            num2 = pop();
            if (num2.number_of_digits == 0)
            {
                printf("Error\n");
            }
            else
            {
                num = pop();
                if (num.number_of_digits == 0)
                {
                    push(num2);
                    printf("Error\n");
                }
                else
                {
                    if (ch == '+')
                    {
                        //CALL ASSEMBLY TO DO NUM + NUM2
                        int bigger = whosBigger(&num, &num2);
                        if (num.sign == 1 && num2.sign == 1)
                        {
                            res.sign = 1;

                            if(bigger == -1)
                            {
                                res.number_of_digits = num2.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num2, &num, &res);
                            }
                            else
                            {
                                res.number_of_digits = num.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num, &num2, &res);
                            }
                        }
                        else if (num.sign == 1 && num2.sign == 0)
                        {
                            if (bigger == 1)
                            {
                                res.number_of_digits = num.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                res.sign = 1;
                                sub1(&num, &num2, &res);
                            }
                            else if (bigger == -1)
                            {
                                res.number_of_digits = num2.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                res.sign = 0;
                                sub1(&num2, &num, &res);
                            }
                            else
                            {
                                res.number_of_digits = 1;
                                res.digit = (char*)malloc(sizeof(char));
                                res.digit[0] = '0';
                            }
                        }
                        else if (num.sign == 0 && num2.sign == 1)
                        {
                            if (bigger == 1)
                            {
                                res.number_of_digits = num.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                res.sign = 0;
                                sub1(&num, &num2, &res);
                            }
                            else if (bigger == -1)
                            {
                                res.number_of_digits = num2.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                res.sign = 1;
                                sub1(&num2, &num, &res);
                            }
                            else
                            {
                                res.number_of_digits = 1;
                                res.digit = (char*)malloc(sizeof(char));
                                res.digit[0] = '0';
                            }
                        }
                        else
                        {
                            if (bigger == 1)
                            {
                                res.number_of_digits = num.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num, &num2, &res);
                            }
                            else
                            {
                                res.number_of_digits = num2.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num2, &num, &res);
                            }
                        } 
                    }
                    else if (ch == '-')
                    {
                        int bigger = whosBigger(&num, &num2);
                        if (num.sign == 1 && num2.sign == 1)
                        {
                            if(bigger == -1)
                            {
                                res.number_of_digits = num2.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                res.sign = 0;
                                sub1(&num2, &num, &res);
                            }
                            else if(bigger == 1)
                            {
                                res.number_of_digits = num.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                res.sign = 1;
                                sub1(&num, &num2, &res);
                            }
                            else
                            {
                                res.number_of_digits = 1;
                                res.digit = (char*)malloc(sizeof(char));
                                res.digit[0] = '0';
                            }
                        }
                        else if (num.sign == 1 && num2.sign == 0)
                        {
                            res.sign = 1;
                            if (bigger == 1)
                            {
                                res.number_of_digits = num.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num, &num2, &res);
                            }
                            else
                            {
                                res.number_of_digits = num2.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num2, &num, &res);
                            }
                        }
                        else if (num.sign == 0 && num2.sign == 1)
                        {
                            res.sign = 0;
                            if (bigger == 1)
                            {
                                res.number_of_digits = num.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num, &num2, &res);
                            }
                            else
                            {
                                res.number_of_digits = num2.number_of_digits+1;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                add1(&num2, &num, &res);
                            }
                        }
                        else
                        {
                            if (bigger == 1)
                            {
                                res.sign = 0;
                                res.number_of_digits = num.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                sub1(&num, &num2, &res);
                            }
                            else if (bigger == -1)
                            {
                                res.number_of_digits = num2.number_of_digits;
                                res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                                res.sign = 1;
                                sub1(&num2, &num, &res);
                            }
                            else
                            {
                                res.number_of_digits = 1;
                                res.digit = (char*)malloc(sizeof(char));
                                res.digit[0] = '0';
                            }
                        } 
                    }
                    else if (ch == '*')
                    {

                        if((num.sign == 0 && num2.sign == 0) || (num.sign == 1 && num2.sign == 1))
                            res.sign = 0;
                        if((num.sign == 1 && num2.sign == 0) || (num.sign == 0 && num2.sign == 1))
                            res.sign = 1;
                        
                        long i = 0;
                        int bigger = whosBigger(&num, &num2);
                        res.number_of_digits = num.number_of_digits+num2.number_of_digits;
                        res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                        for (i = 0; i < res.number_of_digits; i++)
                        {
                            res.digit[i] = '0';
                        }
                        if(bigger == 1)
                        {
                            mult(&num, &num2, &res);
                        }
                        else
                        {
                            mult(&num2, &num, &res);
                        }
                    }
                    else if (ch == '/')
                    {
                        bignum factor;
                        factor.sign=0;
                        factor.number_of_digits = 1;
                        factor.digit = (char*)malloc(sizeof(char)*factor.number_of_digits);
                        factor.digit[0] = '1';


                        if((num.sign == 0 && num2.sign == 0) || (num.sign == 1 && num2.sign == 1))
                            res.sign = 0;
                        if((num.sign == 1 && num2.sign == 0) || (num.sign == 0 && num2.sign == 1))
                            res.sign = 1;

                        int bigger = whosBigger(&num, &num2);
                        long i = 0;
                        if(bigger == 1)
                        {
                            res.number_of_digits = num.number_of_digits;
                            res.digit = (char*)malloc(sizeof(char)*(res.number_of_digits));
                            for (i = 0; i < res.number_of_digits; i++)
                            {
                                res.digit[i] = '0';
                            }
                            divd(&num, &num2, &res,&factor);
                        }
                        else  if(bigger == -1)
                        {
                            res.sign = 0;
                            res.number_of_digits = 1;
                            res.digit = (char*)malloc(sizeof(char));
                            res.digit[0] = '0';
                        }
                        else
                        {
                            res.sign = 0;
                            res.number_of_digits = 1;
                            res.digit = (char*)malloc(sizeof(char));
                            res.digit[0] = '1';
                        }
                        free(factor.digit);
                    }
                    push(res);
                    free(num.digit);
                    free(num2.digit);
                    free(res.digit);
                }
            }
        }
        else if (ch == 'p')
        {
            long i = 0;
            num = pop();
            if (num.number_of_digits > 0)
            {
                if (num.sign == 1)
                    putchar('-');
                short zerosFlag = 1;
                for (i = 0; i < num.number_of_digits; i++)
                {
                    if (zerosFlag && num.digit[i] == '0')
                        continue;
                    zerosFlag = 0;
                    printf("%c", num.digit[i]);
                }
                if (zerosFlag)
                    printf("0");
                printf("\n");
                push(num);
                free(num.digit);
            }
            else
            {
                printf("Error\n");
            }
        }
        else if (ch == 'c')
        {
            clearStack();
        }
        else if (ch == 'q')
        {
            break;
        }
        
    }
    clearStack();
    return 0;
}

void push(bignum num)
{
    bignum copy;
    if (num.number_of_digits == 0)
    {
        printf("Number with 0 digits does not exist.\n");
        return;
    }
    if (s.top == MAX_STACK_SIZE - 1)
    {
        printf("Stack is full\n");
        return;
    }
    copy.digit = (char*)malloc(sizeof(char));
    DeepCopyBignums(&num, &copy);
    s.stk[++s.top] = copy;
}

bignum pop()
{
    bignum num;
    if (s.top == -1)
    {
        bignum b;
        b.number_of_digits = 0;
        b.digit = NULL;
        return b;
    }
    num = s.stk[s.top--];
    return num;
}

bignum makeNewBignum()
{
    bignum b;
    b.sign = 0;
    long size = 1;
    long buffer = 0;
    b.digit = (char*)malloc(size);
    fflush(stdin);
    while(ch > 32)
    {
        if (ch < '0' || ch > '9')
        {
            if (ch == '_' && buffer == 0)
            {
                b.sign = 1;
            }
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == 'c' || ch == 'p' || ch == 'q')
            {
                readNextChar = 0;
                break;
            }
            else
            {
                printf("Only numbers or '_' at the start (for minus) allowed.\n");
                fflush(stdin);
                free(b.digit);
                b.digit = NULL;
                b.number_of_digits = 0;
                return b;
            }
        }
        else
        {
            if (buffer == size)
            {
                size += 1;
                b.digit = (char*)realloc(b.digit, size);
            }
            b.digit[buffer++] = ch;
        }
        ch = getchar();
    }
    b.number_of_digits = size;
    return b;
}

void clearStack()
{
    int i;
    if (s.top != -1) //free everything
    {
        for (i = s.top; i >= 0; i--)
        {
            free(s.stk[i].digit);
        }
    }
    s.top = -1;
}

void display()
{
    int i, j;
    if (s.top == -1)
    {
        printf ("Stack is empty\n");
        return;
    }
    printf ("\nSTACK:\n");
    for (i = s.top; i >= 0; i--)
    {
        for(j = 0; j < s.stk[i].number_of_digits; j++)
            printf ("%c", s.stk[i].digit[j]);
        printf("\n");
    }
    printf("\n");
}

int whosBigger(bignum* a, bignum* b)
{
    long i = 0;
    long a_num = 0, b_num = 0, diff1 = 0, diff2 = 0;
    for(i = 0; i < a->number_of_digits; i++)
    {
        if (a->digit[i] == '0')
            diff1++;
        else
            break;
    }
    for(i = 0; i < b->number_of_digits; i++)
    {
        if (b->digit[i] == '0')
            diff2++;
        else
            break;
    }
    a_num = a->number_of_digits - diff1;
    b_num = b->number_of_digits - diff2;
    if (a_num == 0 && b_num != 0)
        return -1;
    else if (a_num != 0 && b_num == 0)
        return 1;
    else if (a_num == 0 && b_num == 0)
        return 0;
    if (a_num > b_num)
        return 1;
    else if (a_num < b_num)
        return -1;
    for (i = 0; i < a_num; i++)
    {
        if (a->digit[i+diff1] > b->digit[i + diff2])
            return 1;
        else if (a->digit[i+diff1] < b->digit[i+ diff2])
            return -1;
    }
    return 0;
}

//assume a >= b
void mult(bignum* a, bignum* b, bignum* res)
{
    cleanZeros(b);
    if (b->number_of_digits == 0)
    {
        res->sign = 0;
        return;
    }
    long i = 0;
    if((b->number_of_digits == 1) && (b->digit[0] == '1'))
    {
        int bigger = whosBigger(a, res);
        bignum temp;
        temp.digit = (char*)malloc(sizeof(char));
        if(bigger == 1)
        {
            DeepCopyBignums(res, &temp);
            cleanZeros(&temp);
            if (temp.number_of_digits == 0)
            {
                temp.number_of_digits = 1;
                temp.digit = (char*)realloc(temp.digit, temp.number_of_digits);
                temp.digit[0] = '0';
                res->number_of_digits = a->number_of_digits+1;
                res->digit = (char*)realloc(res->digit, res->number_of_digits);
                for(i = 0; i < res->number_of_digits; i++)
                {
                    res->digit[i] = '0';
                }
                add1(a, &temp, res);
            }
            else
            {
                res->number_of_digits = a->number_of_digits+1;
                res->digit = (char*)realloc(res->digit, res->number_of_digits);
                for(i = 0; i < temp.number_of_digits; i++)
                {
                    res->digit[i+a->number_of_digits-temp.number_of_digits+1] = temp.digit[i];
                }
                for (i = 0; i < (res->number_of_digits - temp.number_of_digits); i++)
                {
                    res->digit[i] = '0';
                }
                add1(a,&temp,res);
            }
        }
        else
        {
            cleanZeros(res);

            add1(&temp,a,res);
        }
        free(temp.digit);
        return;
    }
    int lsbB = b->digit[b->number_of_digits-1] - '0';
    if(lsbB%2 == 1)
    {
        int bigger = whosBigger(a, res);
        bignum temp;
        temp.digit = (char*)malloc(sizeof(char));
        DeepCopyBignums(res, &temp);
        cleanZeros(&temp);
        if(bigger == 1)
        {
            if (temp.number_of_digits == 0)
            {
                temp.number_of_digits = 1;
                temp.digit = (char*)realloc(temp.digit, temp.number_of_digits);
                temp.digit[0] = '0';
                res->number_of_digits = a->number_of_digits+1;
                res->digit = (char*)realloc(res->digit, res->number_of_digits);
                for(i = 0; i < res->number_of_digits; i++)
                {
                    res->digit[i] = '0';
                }
                add1(a, &temp, res);
            }
            else
            {
                res->number_of_digits = a->number_of_digits+1;
                res->digit = (char*)realloc(res->digit, res->number_of_digits);
                for(i = 0; i < temp.number_of_digits; i++)
                {
                    res->digit[i+a->number_of_digits-temp.number_of_digits+1] = temp.digit[i];
                }
                for (i = 0; i < (res->number_of_digits - temp.number_of_digits); i++)
                {
                    res->digit[i] = '0';
                }
                add1(a,&temp,res);
            }
        }
        else
        {
            res->number_of_digits = temp.number_of_digits+1;
            res->digit = (char*)realloc(res->digit, res->number_of_digits);
            for(i = 0; i < temp.number_of_digits; i++)
            {
                res->digit[i+1] = temp.digit[i];
            }
            res->digit[0] = '0';
            add1(&temp, a, res);
        }
        free(temp.digit);
    }
    domult2(a);
    div2(b);
    mult(a,b,res);
}

//assume a >= b
void divd(bignum* a, bignum* b, bignum* res, bignum* factor)
{
    if (b->number_of_digits == 1 && b->digit[0] == '0')
    {
        printf("You can't divide by zero.\n");
        return;
    }
    long i = 0;
    int bigger = 0;
    bignum temp;
    div_helper(a,b,res,factor);
    temp.digit = (char*)malloc(sizeof(char));
    DeepCopyBignums(a, &temp);
    bigger = whosBigger(a, b);
    if(bigger >= 0)
    {
        sub1(&temp, b, a);
        DeepCopyBignums(res, &temp);
        cleanZeros(&temp);
        bigger = whosBigger(res, factor);
        if (bigger == 1)
        {
            res->number_of_digits = temp.number_of_digits+1;
            res->digit = (char*)realloc(res->digit, res->number_of_digits);
            for(i = 0; i < temp.number_of_digits; i++)
            {
                res->digit[i+1] = temp.digit[i];
            }
            res->digit[0] = '0';
            add1(&temp, factor, res);
        }
        else
        {
            res->number_of_digits = factor->number_of_digits+1;
            res->digit = (char*)realloc(res->digit, res->number_of_digits);
            for(i = 0; i < temp.number_of_digits; i++)
            {
                res->digit[i+factor->number_of_digits-temp.number_of_digits+1] = temp.digit[i];
            }
            for (i = 0; i < (res->number_of_digits - temp.number_of_digits); i++)
            {
                res->digit[i] = '0';
            }
            add1(factor,&temp,res);
        }
    }
    free(temp.digit);
}

void div_helper(bignum* a, bignum* b, bignum* res, bignum* factor)
{
    bignum temp;
    long i = 0;

    int bigger = whosBigger(a, b);
    if(bigger == -1)
    {
        div2(b);
        cleanZeros(b);
        div2(factor);
        cleanZeros(factor);
        return;
    }
    domult2(factor);
    domult2(b);
    div_helper(a,b,res,factor);
    temp.digit = (char*)malloc(sizeof(char));
    bigger = whosBigger(a, b);
    if(bigger >= 0)
    {
        DeepCopyBignums(a, &temp);
        sub1(&temp,b,a);
        DeepCopyBignums(res, &temp);
        cleanZeros(&temp);
        if (temp.number_of_digits == 0)
        {
            temp.number_of_digits = 1;
            temp.digit = (char*)realloc(temp.digit, temp.number_of_digits);
            temp.digit[0] = '0';
            res->number_of_digits = factor->number_of_digits+1;
            res->digit = (char*)realloc(res->digit, res->number_of_digits);
            for(i = 0; i < res->number_of_digits; i++)
            {
                res->digit[i] = '0';
            }
            add1(factor, &temp, res);
        }
        else
        {
            bigger = whosBigger(factor, &temp);
            if (bigger == 1)
            {
                res->number_of_digits = factor->number_of_digits+1;
                res->digit = (char*)realloc(res->digit, res->number_of_digits);
                for(i = 0; i < temp.number_of_digits; i++)
                {
                    res->digit[i+factor->number_of_digits-temp.number_of_digits+1] = temp.digit[i];
                }
                for (i = 0; i < (res->number_of_digits - temp.number_of_digits); i++)
                {
                    res->digit[i] = '0';
                }
                add1(factor, &temp, res);
            }
            else
            {
                res->number_of_digits = temp.number_of_digits+1;
                res->digit = (char*)realloc(res->digit, res->number_of_digits);
                for(i = 0; i < temp.number_of_digits; i++)
                {
                    res->digit[i+1] = temp.digit[i];
                }
                res->digit[0] = '0';
                add1(&temp, factor, res);
            }
        }
    }
    div2(b);
    cleanZeros(b);
    div2(factor);
    cleanZeros(factor);
    free(temp.digit);
}



void DeepCopyBignums(bignum* a, bignum* b) //copies a to b, assuming b->digit is already allocated (doing realloc)
{
    long i = 0;
    b->sign = a->sign;
    b->number_of_digits = a->number_of_digits;
    b->digit = (char*)realloc(b->digit, sizeof(char)*b->number_of_digits);
    for(i=0; i < b->number_of_digits; i++)
        b->digit[i] = a->digit[i];
}

void domult2(bignum* a)
{
    long i = 0;
    bignum temp;
    temp.digit = (char*)malloc(sizeof(char));
    DeepCopyBignums(a, &temp);
    a->number_of_digits += 1;
    a->digit = (char*)realloc(a->digit, a->number_of_digits);
    for(i = 0; i < temp.number_of_digits; i++)
        a->digit[i+1] = temp.digit[i];
    a->digit[0] = '0';
    mult2(a);
    if (a->digit[0] == '0')
    {
        cleanZeros(a);
    }
    free(temp.digit);
}

void cleanZeros(bignum* a)
{
    long i = 0, keep = 0;
    bignum temp;
    temp.digit = (char*)malloc(sizeof(char));
    for(i = 0; i < a->number_of_digits; i++)
    {
        if (a->digit[i] != '0')
        {
            break;
        }
    }
    if (i > 0)
    {
        DeepCopyBignums(a, &temp);
        a->number_of_digits -= i;
        a->digit = (char*)realloc(a->digit, a->number_of_digits);
        keep = i;
        for(i = 0; i < a->number_of_digits; i++)
        {
            a->digit[i] = temp.digit[i + keep];
        }
    }
    free(temp.digit);
}
