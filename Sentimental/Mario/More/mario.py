def main(height):
    for h in range(height):
        for i in range(height - 1 - h):
            print("", end=" ")
        for space in range(2):
            for tag in range(h + 1):
                if space == 0 and tag == h:
                    print("#", end="  ")
                else:
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
