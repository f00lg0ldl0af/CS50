from cs50 import get_int
import math


def main():
    card = get_creditCardNum()
    validity = check_valid(card)  # Returns true or false
    count = get_numOfDigits(card)
    check_whichCard(card, count, validity)


def get_creditCardNum():
    while True:
        c = get_int("Number: ")
        if c > 0:
            break
    return c


def check_valid(card):
    temp_card = card
    temp_card2 = card
    sum = 0
    while temp_card:
        digit = int(temp_card / 10 % 10) * 2
        while digit > 0:
            digit_tens = int(digit % 10)
            sum += int(digit_tens)
            digit /= 10
        temp_card /= 100

    sumNext = 0
    while temp_card2:
        digitNext = int(temp_card2 % 10)
        sumNext += int(digitNext)
        temp_card2 /= 100
    total = int(sum + sumNext)

    return total % 10 == 0


def get_numOfDigits(card):
    return int(1 + math.log10(card))


def check_whichCard(card, count, validity):
    temp_card = card
    temp_card2 = card
    first_twoDigits = int(temp_card / (10 ** (count - 2)))
    first_digit = int(temp_card2 / (10 ** (count - 1)))
    if validity:
        if first_digit == 4 and (count == 13 or count == 16):
                print("VISA")
        elif count == 15 and (first_twoDigits == 34 or first_twoDigits == 37):
                print("AMEX")
        elif count == 16 and (51 <= first_twoDigits <= 55):
                print("MASTERCARD")
        else:
                print("INVALID")
    else:
        print("INVALID")


main()

# Alternative to Luhn Algorithm (Source: https://allwin-raju-12.medium.com/credit-card-number-validation-using-luhns-algorithm-in-python-c0ed2fac6234)
# def Luhn_checksum(card):
#   def digits_of(n):
#       return [int(d) for d in str(n)]
#   digits = digits_of(card)
#   odd_digits = digits[-1::-2] ## This means get the 2nd next element, starting from last element
#   even_digits = digits[-2::2]
#   checksum = 0
#   checksum += sum(odd_digits)
#   for d in even_digits:
#       checksum += sum(digits_of(d * 2))
#   return checksum % 10