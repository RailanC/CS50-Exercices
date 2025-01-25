def main():
    phrase = input("Text: ")
    grade = count_score_grade(phrase)
    if grade > 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")

def count_score_grade(text):
    countWords = 0
    countLetters = 0
    countSentences = 0
    for char in text:
        if char in ' ':
            countWords += 1
        elif char in {'!','.','?'}:
            countSentences += 1
        elif char.isalpha():
            countLetters += 1

    countWords += 1
    return compute_coleman_liau(countWords, countLetters, countSentences)

def compute_coleman_liau(words, letters, sentences):
    avgLetters = 0.0
    avgSentences = 0.0
    fwords = float(words)
    avgLetters = (letters / fwords) * 100.0
    avgSentences = (sentences / fwords) * 100.0
    return round(0.0588 * avgLetters - 0.296 * avgSentences - 15.8)

main()
