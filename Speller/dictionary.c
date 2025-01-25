// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char *word;
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 1125;

int count_words = 0;
char word_used[LENGTH + 1];

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char new_word[LENGTH + 1] = {0};
    for (int i = 0; word[i] != '\0'; i++)
    {
        new_word[i] = tolower(word[i]); // Convert each character to lowercase
    }
    int ascii_sum = hash(new_word);

    node *new_next;
    new_next = table[ascii_sum];
    while (new_next != NULL)
    {
        if (strcmp(new_next->word, new_word) == 0)
            return true;

        new_next = new_next->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    int i = 0;

    while (word[i] != '\0')
    {
        if (isalpha(word[i]))
            sum += word[i] - 'a';
        i++;
    }
    return sum;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }

    char c;
    int i = 0;

    // Read each word in the file
    while (fscanf(source, "%s\n", word_used) != EOF)
    {
        node *new_word = malloc(sizeof(node));

        new_word->word = malloc(strlen(word_used) + 1);

        strcpy(new_word->word, word_used);

        int key = hash(word_used);

        if (table[key] == NULL)
        {
            new_word->next = NULL;
            table[key] = new_word;
        }
        else
        {
            new_word->next = table[key];
            table[key] = new_word;
        }
        count_words++;
    }

    // Close the dictionary file
    fclose(source);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        node *last;

        last = table[i];

        while (last != NULL)
        {
            node *tmp = last;
            last = last->next;
            free(tmp->word);
            free(tmp);
        }
        free(last);
    }

    return true;
}
