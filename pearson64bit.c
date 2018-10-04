/**
 * pearson64bit.c
 * Test implementation of Pearson Hashing with 64-bit values.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


static unsigned char rand_uchar(int n) {
    int limit = RAND_MAX - RAND_MAX % n;
    int rnd;

    do {
        rnd = rand();
    } while (rnd >= limit);
    return rnd % n;
}


/**
 * Implements Fisher-Yates shuffling on an array.
 */
void fy_shuffle(unsigned char *array, size_t n) {

    if (n > 1) {
        size_t i;
        for (i = n - 1; i > 0; i--) {
            size_t j = rand_uchar(i + 1);
            unsigned char temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}


/**
 * Generate a random Pearson table with n elements (0 - n-1).
 * Mallocs the table; **caller must handle the free.**
 */
unsigned char* generate_pearson_table(int n) {

    unsigned char *table = malloc(n * sizeof(char));
    for (size_t i = 0; i <= n-1; i++) {
        table[i] = i;
    }
    fy_shuffle(table, n);
    return table;
}


/** 
 * Implementation of the Pearson Hash algorithm on 64-bits/16 hex chars.
 */
void pearson_64bit(const unsigned char *input_str, size_t len, char *hex, size_t hexlen) {

    unsigned char hash;
    unsigned char hash_array[8];
    unsigned char *table = generate_pearson_table(256);
    size_t i;
    size_t j;
    for (j = 0; j < 8; ++j) {
        size_t offset = (input_str[0] + j) % 256;
        hash = table[offset];
        for (i = 1; i < len; ++i) {
            hash = table[hash ^ input_str[i]];
        }
        hash_array[j] = hash;
    }
    free(table); // cleanup

    snprintf(hex, hexlen, "%02X%02X%02X%02X%02X%02X%02X%02X",
             hash_array[0], hash_array[1], hash_array[2], hash_array[3],
             hash_array[4], hash_array[5], hash_array[6], hash_array[7]);
}


/**
 * Run through a single usage of pearson_64bit()
 */
int main(void) {

    const unsigned char xyz[8] = {0x55, 0xAA, 0x02, 0x03, 0x07, 0x60, 0x90, 0x04};
    size_t len_xyz = 8;
    char *hex = malloc(25 * sizeof(char));
    size_t hexlen = 25;

    pearson_64bit(xyz, len_xyz, hex, hexlen);
    printf("%s\n", hex);
    free(hex);
}
