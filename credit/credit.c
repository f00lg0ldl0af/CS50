#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

//Add in first line(s) of respective functions to tease the compiler
long get_creditCardNum(void);
bool check_valid(long tempCard, int sum, long creditCard);
int get_numOfDigits(long creditCard);
void check_whichCard(long tempCard, int count, bool notValid);

//Main function
int main(void)
{
    //Prompts user for credit card number
    long creditCard = get_creditCardNum();

    //Luhn's Algorithm to check if credit card number is VALID
    long tempCard = creditCard; //Create temporary variable to store credit card number
    int sum = 0;
    //check_valid(tempCard, sum, creditCard);

    //Counts number of digits on credit card
    int count = get_numOfDigits(creditCard);
    //printf("Number of digits: %i\n", count);

    //Check only if VISA, AMEX or MASTERCARD
    tempCard = creditCard; //Restore this variable which became zero from check_valid function
    bool isValid =  check_valid(tempCard, sum, creditCard);
    check_whichCard(tempCard, count, isValid);
}

//Functions
long get_creditCardNum(void)
{
    long creditCard;
    creditCard = get_long("Number: ");
    return creditCard;
}

bool check_valid(long tempCard, int sum, long creditCard)
{
    bool isValid = false;
    //printf("Variable isValid: %i\n", isValid);

    while (tempCard)
    {
        //Get every other digit, starting from second-to-last digit
        //Multiply each of these digits by 2
        int digit = (tempCard / 10 % 10) * 2;
        //printf("Digits: %i\n", digit);

        //Add those products' digits. '12' would be '1' + '2'.
        //Note that some of the products could be 2-digit numbers themselves after multiplication
        while (digit > 0)
        {
            int digit_tens = (digit % 10);
            sum += digit_tens; /*int sum here i.e., int sum += digit_tens;
            will result in "error: invalid '+=' at end of declaration; did
            you mean '='?"*/
            digit /= 10;
            //printf("Sum: %i\n", sum);
        }
        tempCard /= 100;
    }

    tempCard = creditCard;
    int sumNext = 0;

    //Add above sum to sum of digits (not multiplied by two)
    while (tempCard)
    {
        int digitNext = (tempCard % 10); //Get last digit
        sumNext += digitNext;
        //printf("Next sum: %i\n", sumNext);
        tempCard /= 100;
    }
    //if total of both sums % 10 == 0, credit card number is VALID
    int total = sum + sumNext;
    //(total % 10 == 0)? printf("VALID\n") : printf("INVALID\n");
    if (total % 10 != 0)
    {
        //printf("INVALID\n");
        isValid = false;
        return isValid;
        //printf("Variable isValid: %i\n", isValid);
    }
    else
    {
        isValid = true;
        return isValid;
    }
}

int get_numOfDigits(long creditCard)
{
    int count = 0;
    count = (creditCard == 0) ? 1 : (1 + log10(creditCard));
    return count;

    /*Alternative below
        while (creditCard != 0)
    {
        creditCard /= 10;
        count++;
    }*/
}

void check_whichCard(long tempCard, int count, bool isValid)
{
    //Get first two digits of credit card number
    long firstTwoDigit = tempCard / (pow(10, (count - 2)));
    //printf("First Two Digits: %li\n", firstTwoDigit);

    //Get first digit of credit card number
    long firstDigit = tempCard / (pow(10, count - 1));
    //printf("First Digit: %li\n", firstDigit);

    //printf("Variable isValid: %i\n", isValid);
    if (isValid == true)
    {
        //Visa: 13- or 16-digit numbers; start with 4
        if (firstDigit == 4)
        {
            if (count == 13 || count == 16)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        //AMEX: 15-digit numbers; start with 34 or 37
        else if (count == 15)
        {
            if (firstTwoDigit == 34 || firstTwoDigit == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        //MasterCard: 16-digit numbers; start with 51 - 55
        else if (count == 16)
        {
            if (firstTwoDigit == 51 || firstTwoDigit == 52 || firstTwoDigit == 53 || firstTwoDigit == 54 || firstTwoDigit == 55)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

