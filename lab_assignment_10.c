#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie {
    struct Trie *children[ALPHABET_SIZE];
    int count; // Number of occurrences of the word ending at this node
};

// Function to initialize a trie node
struct Trie *createTrie() {
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node) {
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Function to insert a word into the trie
void insert(struct Trie *root, char *word) {
    struct Trie *current = root;
    for (int i = 0; word[i] != '\0'; ++i) {
        char ch = tolower(word[i]); // Convert character to lowercase
        int index = ch - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) {
            //printf("Invalid character: %c\n", word[i]);
            continue; // Skip invalid characters
        }
        if (!current->children[index]) {
            current->children[index] = createTrie();
        }
        current = current->children[index];
    }
    current->count++;
}

// Function to compute the number of occurrences of a word
int numberOfOccurrences(struct Trie *root, char *word) {
    struct Trie *current = root;
    for (int i = 0; word[i] != '\0'; ++i) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->count;
}

// Function to deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (pTrie->children[i]) {
                deallocateTrie(pTrie->children[i]);
            }
        }
        free(pTrie);
    }
    return NULL;
}

// Function to read the dictionary file and populate the array of words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);

    struct Trie *root = createTrie();
    for (int i = 0; i < numWords; ++i) {
        insert(root, inWords[i]);
    }

    //parse lineby line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; ++i) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    root = deallocateTrie(root);
    if (root != NULL) {
        printf("There is an error in this program\n");
    }
    return 0;
}
