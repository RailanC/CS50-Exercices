def main(height):
    for h in range(height):
        for i in range(height - 1 - h):
            print("", end=" ")
        for tag in range(h + 1):
            print("#", end="")
        print()


def intro():
    height = 0
    while (height < 1 or height > 8):
        height = int(input("Height: "))
    main(height)


try:
    intro()
except:
    intro()
