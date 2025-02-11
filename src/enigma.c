#include "enigma.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

rotor *rotor_create(const char number[], int ring_setting)
{
	rotor *r_ptr;
	int rotor_number, i;
	char* number_cpy;
    /*check ring setting*/
    if(ring_setting < 0 || ring_setting > 25)
        return NULL;

    /*copy number and convert to lower*/
    number_cpy = malloc(sizeof(number));
	if(number_cpy == NULL)
        return NULL;
    strcpy(number_cpy, number);
    for (i = 0; i < strlen(number_cpy); i++)
        number_cpy[i] = tolower(number_cpy[i]);

    /*create rotor*/
    r_ptr = malloc(sizeof(rotor));
    if(r_ptr == NULL){
        free(number_cpy);
        return NULL;
    }

    r_ptr->turnover_markers[1] = -1;
    /*set key and turnover markers on used rotor*/
    if(strcmp(number_cpy, "i")){
        r_ptr = key_create("ekmflgdqvzntowyhxuspaibrcj");
        r_ptr->turnover_markers[0] = "q" - "a";
    } else if(strcmp(number_cpy, "ii")){
        r_ptr = key_create("ajdksiruxblhwtmcqgznpyfvoe");
        r_ptr->turnover_markers[0] = "e" - "a";
    } else if(strcmp(number_cpy, "iii")){
        r_ptr = key_create("bdfhjlcprtxvznyeiwgakmusqo");
        r_ptr->turnover_markers[0] = "v" - "a";
    } else if(strcmp(number_cpy, "iv")){
        r_ptr = key_create("esovpzjayquirhxlnftgkdcmwb");
        r_ptr->turnover_markers[0] = "j" - "a";
    } else if(strcmp(number_cpy, "v")){
        r_ptr = key_create("vzbrgityupsdnhlxawmjqofeck");
        r_ptr->turnover_markers[0] = "z" - "a";
    } else if(strcmp(number_cpy, "vi")){
        r_ptr = key_create("jpgvoumfyqbenhzrdkasxlictw");
        r_ptr->turnover_markers[0] = "z" - "a";
        r_ptr->turnover_markers[1] = "m" - "a";
    } else if(strcmp(number_cpy, "vii")){
        r_ptr = key_create("nzjhgrcxmyswboufaivlpekqdt");
        r_ptr->turnover_markers[0] = "z" - "a";
        r_ptr->turnover_markers[1] = "m" - "a";
    } else if(strcmp(number_cpy, "viii")){
        r_ptr = key_create("fkqhtlxocbjspdzramewniuygv");
        r_ptr->turnover_markers[0] = "z" - "a";
        r_ptr->turnover_markers[1] = "m" - "a";
    } else {
        free(number_cpy);
        free(r_ptr);
        return NULL;
    }
    r_ptr->turnover_markers[0] = roll_in_alphabet(r_ptr->turnover_markers[0], ring_setting, 26);
    if(r_ptr->turnover_markers[1] != -1)
        r_ptr->turnover_markers[1] = roll_in_alphabet(r_ptr->turnover_markers[1], ring_setting, 26);

    /*free in case of unsuccessful memory allocation for key*/
    if(r_ptr->k == NULL){
        free(number_cpy);
        free(r_ptr);
        return NULL;
    }

    return r_ptr;
}

reflector *reflector_create(const char type)
{
    rotor *r_ptr;
	int rotor_number, i;
    char type_lower = tolower(type);
    /*create reflector*/
    r_ptr = malloc(sizeof(reflector));
    if(r_ptr == NULL)
        return NULL;

    /*set key depending on used reflector*/
    switch (tolower(type))
    {
    case 'a':
        r_ptr->k = key_create("ejmzalyxvbwfcrquontspikhgd");
        break;
    case 'b':
        r_ptr->k = key_create("yruhqsldpxngokmiebfzcwvjat");
        break;
    case 'c':
        r_ptr->k = key_create("fvpjiaoyedrzxwgctkuqsbnmhl");
        break;
    default:
        free(r_ptr);
        return NULL;
    }
    /*free in case of unsuccessful memory allocation for key*/
    if(r_ptr->k == NULL){
        free(r_ptr);
        return NULL;
    }

    return r_ptr;
}

plugboard *plugboard_create(const char key_str[])
{
    plugboard *p_ptr;
    int i;

    if(sizeof(key_str) != 26)
        return NULL;

    /*make sure only letter pairs are swapped*/
    for (i = 0; i < 26; i++)
    {
        if(!islower(key_str[i]))
            return NULL;
        if(key_str[i] - 'a' == i)
            return NULL;
        if((key_str[key_str[i] - 'a'] - 'a') != i)
            return NULL;
    }

    p_ptr = malloc(sizeof(plugboard));
    if(p_ptr == NULL)
        return NULL;
    p_ptr->k = key_create(key_str);
    if(p_ptr->k == NULL){
        free(p_ptr);
        return NULL;
    }
    return p_ptr;
}

enigma *enigma_create(rotor *rotor_left, rotor *rotor_middle, rotor *rotor_right, reflector *r, plugboard *p)
{
    enigma *e_ptr;
    if(rotor_left == NULL || rotor_middle == NULL || rotor_right == NULL || r == NULL || p == NULL)
        return NULL;

    e_ptr = malloc(sizeof(enigma));
    if(e_ptr == NULL)
        return NULL;

    e_ptr->rotor_left = rotor_left;
    e_ptr->rotor_middle = rotor_middle;
    e_ptr->rotor_right = rotor_right;
    e_ptr->reflector = r;
    e_ptr->plugboard = p;

    return e_ptr;
}

key *key_create(const char key_str[])
{
    char c;
    int *key_ptr;

    if(strlen(key_str) != 26)
        return NULL;

    /*check wether key_str contains all letters once*/
    for (c = 'a'; c <= 'z'; c++) {
        if (strchr(key_str, c) == NULL)
            return NULL;
    }

    key_ptr = malloc(sizeof(key));

    if (key_ptr == NULL)
        return NULL;

    for (c = 0; c < 26; c++)
        key_ptr[c] = key_str[c] - 'a';

    return key_ptr;
}


void rotor_destroy(rotor *r)
{
    if(r == NULL)
        return;
    key_destroy(r->k);
    free(r);
}

void reflector_destroy(reflector *r)
{
    if(r == NULL)
        return;
    key_destroy(r->k);
    free(r);
}

void plugboard_destroy(plugboard *p)
{
    if(p == NULL)
        return;
    key_destroy(p->k);
    free(p);
}

void enigma_destroy(enigma *e)
{
    if(e == NULL)
        return NULL;
    rotor_destroy(e->rotor_left);
    rotor_destroy(e->rotor_middle);
    rotor_destroy(e->rotor_right);
    reflector_destroy(e->reflector);
    plugboard_destroy(e->plugboard);
    free(e);
}

void key_destroy(key *k)
{
    if(k == NULL)
        return;
    free(k);
}

static error_msg key_encrypt(key *k, int i)
{
    if(i < 0 || i >= 26 || key_check(k) != ALL_FINE)
        return INVALID_PARAMETERS;
    return k[i];
}

static error_msg key_decrypt(key *k, int i)
{
    int idx;
    if(i < 0 || i >= 26 || key_check(k) != ALL_FINE)
        return INVALID_PARAMETERS;

    for (idx = 0; idx < 26; idx++) {
        if(k[idx] == i)
        return idx;
    }

    return INVALID_PARAMETERS;
}


error_msg enigma_encrypt(enigma *e, char text[])
{
    int i, upper, let;
    if(enigma_check(e) != ALL_FINE)
        return INVALID_PARAMETERS;

    for (i = 0; i < strlen(text); i++) {
        if (!isalpha(text[i]))
            continue;
        upper = isupper(text[i]);
        let = tolower(text[i]) - 'a';

        /*step rotors*/

        if ((e->rotor_middle->position == e->rotor_middle->turnover_markers[0]) ||
            (e->rotor_middle->position == e->rotor_middle->turnover_markers[1])) {
            rotor_rotate(e->rotor_right);
            rotor_rotate(e->rotor_middle);
            rotor_rotate(e->rotor_left);
        } else if ((e->rotor_right->position == e->rotor_right->turnover_markers[0]) ||
                   (e->rotor_right->position == e->rotor_right->turnover_markers[1])) {
            rotor_rotate(e->rotor_right);
            rotor_rotate(e->rotor_middle);
        } else {
            rotor_rotate(e->rotor_right);
        }

        /*plugboard*/
        if(plugboard_crypt(e->plugboard, let) != ALL_FINE)
            return ENCRYPTION_ERROR;

        /*rotors (right to left)*/
        if(rotor_encrypt(e->rotor_right, let) != ALL_FINE)
            return ENCRYPTION_ERROR;
        if(rotor_encrypt(e->rotor_middle, let) != ALL_FINE)
            return ENCRYPTION_ERROR;
        if(rotor_encrypt(e->rotor_left, let) != ALL_FINE)
            return ENCRYPTION_ERROR;

        /*reflector*/
        if(reflector_crypt(e->reflector, let) != ALL_FINE)
            return ENCRYPTION_ERROR;

        /*rotors (left to right)*/
        if(rotor_decrypt(e->rotor_left, let) != ALL_FINE)
            return ENCRYPTION_ERROR;
        if(rotor_decrypt(e->rotor_middle, let) != ALL_FINE)
            return ENCRYPTION_ERROR;
        if(rotor_decrypt(e->rotor_right, let) != ALL_FINE)
            return ENCRYPTION_ERROR;

        /*plugboard*/
        if(plugboard_crypt(e->plugboard, let) != ALL_FINE)
            return ENCRYPTION_ERROR;

        text[i] = upper ? let + 'A' : let + 'a';
    }
    return ALL_FINE;
}

error_msg rotor_rotate(rotor *r)
{
    if(rotor_check(r) != ALL_FINE)
        return INVALID_PARAMETERS;

    (r->position)++;

    if(r->position >= 26)
        r->position = r->position - 26;

    return ALL_FINE;
}