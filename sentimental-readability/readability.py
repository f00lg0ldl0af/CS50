# TODO

from cs50 import get_string
import re


def main():
    text = get_string("Text: ")
    letter_count = count_letters(text)
    # print(f"Letters: {letter_count}")

    word_count = count_words(text)
    # print(f"Words: {word_count}")

    sentence_count = count_sentences(text)
    # print(f"Sentences: {sentence_count}")

    index = calculate_index(letter_count, word_count, sentence_count)
    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(text):
    letters = 0
    for c in text:
        if c.isalpha():
            letters += 1
    return letters


def count_words(text):
    return int(len(re.findall(r"\s+", text)) + 1)


def count_sentences(text):
    return int(len(re.split(r"[.!?]+", text)) - 1)


def calculate_index(letter_count, word_count, sentence_count):
    return round(
        0.0588 * (letter_count / word_count * 100)
        - 0.296 * (sentence_count / word_count * 100)
        - 15.8
    )


main()
