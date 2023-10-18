// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table (track words in dictionary)
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Declare variables
unsigned int word_count;
unsigned int h_value;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Hash word to obtain a hash value
    h_value = hash(word);

    // Access linked list at that index in hash table
    node *cursor = table[h_value];

    // Traverse linked list, looking for word (strcasecmp)
    while (cursor)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function (for 143, 091 words)
    // Source: djb2 algorithm http://www.cse.yorku.ca/~oz/hash.html
    unsigned long total = 5381;

    for (int i = 0; i < strlen(word); i++)
    {
        total += (33 * total) + tolower(word[i]);
    }
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }

    char word[LENGTH + 1]; // Declare array for word

    // Read strings from dictionary file one at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        // For each word, create a new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);

        // Hash word to obtain hash value
        h_value = hash(word);

        // Insert node into hash table at that location
        n->next = table[h_value];
        table[h_value] = n;

        word_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Iterate over hash table, go over each linked list
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor)
        {
            node *tmp = cursor;
            cursor = cursor->next; // Traverse linked list
            free(tmp);             // Free each node in linked list
        }
    }
    return true;
}