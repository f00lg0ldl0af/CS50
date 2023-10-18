
from cs50 import get_int


def main():
    height = get_height()
    for row in range(height):  # Row starts at 0 [0, 1, 2, 3], if height is 4
        for space in range(height - row - 1, 0, -1):  # Space stops before 0
            print(" ", end="")
        for brick in range(row + 1):
            print("#", end="")
        print(" " * 2, end="")
        for brick in range(row + 1):
            print("#", end="")
        print()


def get_height():
    while True:
        h = get_int("height: ")
        if h > 0 and h < 9:
            break
    return h


main()
