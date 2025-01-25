import math


def main():
    number = int(input("Number: "))
    if (validate_checksum(number)):
        get_card_type(number)
    else:
        print("INVALID")


def validate_checksum(card_number):
    sum = 0
    alt = False

    while (card_number > 0):
        dig = card_number % 10

        if (alt):
            dig *= 2
            if (dig > 9):
                dig -= 9

        sum += dig
        alt = not alt
        card_number //= 10

    return (sum % 10) == 0


def get_card_type(card_number):
    str_card = str(card_number)
    first_two_digits = int(str_card[:2])
    first_dig = int(str_card[0])
    length = len(str_card)

    if length == 15 and (first_two_digits in [34, 37]):
        print("AMEX")
        return
    elif length == 16 and (first_two_digits >= 51 and first_two_digits <= 55):
        print("MASTERCARD")
        return
    elif length in [13, 16] and first_dig == 4:
        print("VISA")
        return

    print("INVALID")


main()
