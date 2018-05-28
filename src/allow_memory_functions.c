#include "custom_function.h"

// Allocate memory for an array of chars.
char * allocate_strmem(int len)
{
    char *tmp;

    if (len <= 0) {
        exit_error("allocate_strmem(): Cannot allocate memory because len <= 0");
    }

    tmp = (char *) malloc (len * sizeof (char));

    if (tmp == NULL) {
        exit_error("allocate_strmem(): Cannot allocate memory for array");
    }

    memset(tmp, 0, len * sizeof (char));
    return tmp;
}

// Allocate memory for an array of unsigned chars.
uint8_t* allocate_ustrmem(int len)
{
    uint8_t *tmp;

    if (len <= 0) {
        exit_error("allocate_ustrmem(): Cannot allocate memory because len <= 0");
    }

    tmp = (uint8_t *) malloc(len * sizeof(uint8_t));

    if (tmp == NULL) {
        exit_error("allocate_ustrmem(): Cannot allocate memory for array");
    }

    memset(tmp, 0, len  * sizeof(uint8_t));
    return tmp;
}

// Allocate memory for an array of ints.
int* allocate_intmem(int len)
{
    int *tmp;

    if (len <= 0) {
        exit_error("allocate_intmem(): Cannot allocate memory because len <= 0");
    }

    tmp = (int *) malloc(len * sizeof(int));

    if (tmp == NULL) {
        exit_error("allocate_intmem(): Cannot allocate memory for array");
    }

    memset(tmp, 0, len * sizeof (int));
    return tmp;
}