def main(change):
    result = 0
    lim = 1e-3
    while change > lim:
        if change >= 0.249:
            change -= 0.25
        elif change >= 0.099:
            change -= 0.10
        elif change >= 0.049:
            change -= 0.05
        elif change >= 0.009:
            change -= 0.01
        print(f"change: {change}")
        result += 1

    print(result)


def intro():
    change = -1
    while (change <= 0):
        try:
            change = float(input("Change: "))
        except ValueError:
            print()

    main(change)


intro()
