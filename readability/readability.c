#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int calculate_index(int avg_letters, int avg_words, int avg_sentences);

int main(void)
{
    // Prompts user for text
    string text = get_string("Text: ");

    // Count number of letters (alphabetical) in text
    count_letters(text);

    // Count number of words in text
    count_words(text);

    // Count number of sentences in text
    count_sentences(text);

    // Calculate index
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences =  count_sentences(text);

    calculate_index(letters, words, sentences);

    // Print Grade
    int index = calculate_index(letters, words, sentences);
    if (index >= 16)
    {
        printf("Grade 16+\n");

    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    // Keep track of number of letters
    int letters = 0;

    // Loop through the strings in the array
    // Strings have the special null character '\0'. Arrays do not.
    // String length known. But array length unknown in advance.
    // strlen() does count the spaces i.e., strlen("David Malan") returns 11

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    //printf("%i letters\n", letters);
    return letters;
}

int count_words(string text)
{
    // Keep track of number of words
    int words = 0;

    //text is array of string(s) which are array(s) of char(s)
    // E.g., string text[2]; text[0] = "HI!"; text[1] = "BYE";
    // text[0][0] = 'H'; text[0][1] = 'I'

    // Declare Pointer c with type char
    char *c = NULL;

    // Tolerate no words
    if (text != 0)
    {
        // Deference c, for loop ends when reach null-terminator
        for (c = text; *c; c++) // Pointer c increment by 1 byte since char type
        {
            //printf("Pointer is: %c\n", *c);
            //printf("Text is: %s\n", c);
            // If current char is [aA-zZ] and (first or follows space)
            if (isalpha(*c) && ((c == text) || (*(c - 1) == ' ') || (*(c - 1) == '"')))
            {
                words++;
            }
        }
        //printf("%i words\n", words);
        return words;
    }
    else
    {
        //printf("%i words\n", words);
        return words;
    }
}

int count_sentences(string text)
{
    // Keep track of number of sentences
    int sentences = 0;

    char *c = NULL;

    for (c = text; *c; c++)
    {
        if (*c == '.' || *c == '?' || *c == '!')
        {
            sentences++;
        }
    }
    //printf("%i sentences\n", sentences);
    return sentences;
}

int calculate_index(int avg_letters, int avg_words, int avg_sentences)
{
    //printf("Letters %i Words %i Sentences %i \n", avg_letters, avg_words, avg_sentences);
    float L = ((float)(avg_letters)) / avg_words * 100;
    //printf("L %f\n",L);
    float S = ((float)(avg_sentences)) / avg_words * 100;
    //printf("S %f\n",S);

    int index = round(0.0588 * L - 0.296 * S - 15.8);
    //printf("%i\n", index);
    return index;
}