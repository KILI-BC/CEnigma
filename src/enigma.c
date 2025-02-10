#include "enigma.h"

static int roll_in_alphabet(int i, int shift, int alphabet_size)
{
    if (i < 0)
        return i;
    if (alphabet_size <= 0)
        return 0;
    i += shift;
    while (i < 0)
        i += alphabet_size;

    return i % alphabet_size;
}

