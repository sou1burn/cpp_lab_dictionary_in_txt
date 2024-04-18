#include <stdio.h>
#include "lab10.h"
#include <stdlib.h>

//дописать сколько раз встречается слово в скобочках рядом
int string_comparison(char* str1, char* str2) {

    while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    return (*str1 > *str2) - (*str1 < *str2);

}

int find_word(char* word, char** words, size_t word_count) {

    for (size_t i = 0; i < word_count; ++i) {
        if (string_comparison(word, words[i]) == 0) {
            return i;
        }
    }
    return -1;
}

char* my_strdup(const char* str) {

    size_t len = my_strlen(str);

    char* new_str = (char*)malloc(len + 1);

    if (new_str == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    for (size_t i = 0; i <= len; i++) {
        new_str[i] = str[i];
    }

    new_str[len] = '\0';

    return new_str;
}

size_t my_strlen(const char* str) {

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void create_dict() {

    FILE* input_file = fopen("lab10.txt", "r");

    if (input_file == NULL) {
        perror("Error occured when trying to open file");
        return;
    }

    char buffer[1000];
    size_t buffer_size = sizeof(buffer);
    char** words = (char**)malloc(1 * sizeof(char*));
    size_t word_count = 0;
    size_t* counts = NULL;

    while (fgets(buffer, buffer_size, input_file) != NULL) {
        /*size_t len = 0;
        while (buffer[len] != '\0' && buffer[len] != '\n') {
            ++len;
        }*/

        char* word = my_strdup(buffer);

        int index = find_word(word, words, word_count);

        if (index == -1) {
            ++word_count;
            char** temp_words = (char**)realloc(words, word_count * sizeof(char*));
            size_t* temp_counts = (size_t*)realloc(counts, word_count * sizeof(size_t));
            if (temp_words == NULL || temp_counts == NULL) {
                perror("Error while reallocating memory");
                return;
            }
            words = temp_words;
            counts = temp_counts;
            words[word_count - 1] = my_strdup(word);
            counts[word_count - 1] = 1;
        }
        else {
            counts[index]++;
            free(word);
        }
    }

    fclose(input_file);

    for (size_t i = 0; i < word_count; ++i) {
        for (size_t j = i + 1; j < word_count; ++j) {
            if (string_comparison(words[i], words[j]) > 0) {
                char* temp_word = words[i];
                size_t temp_count = counts[i];

                words[i] = words[j];
                counts[i] = counts[j];

                words[j] = temp_word;
                counts[j] = temp_count;
            }
        }
    }


    FILE* output_file = fopen("lab10_output.txt", "w");

    if (output_file == NULL) {
        perror("Error occured while opening output file");
        return;
    }

    for (size_t i = 0; i < word_count; ++i) {
        fprintf(output_file, "%s (%zu)\n", words[i], counts[i]);
        free(words[i]);
    }

    free(words);
    fclose(output_file);

}