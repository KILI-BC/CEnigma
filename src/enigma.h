#ifndef ENIGMA_H_INCLUDED
#define ENIGMA_H_INCLUDED

typedef int key[26];

typedef struct _reflector {
    key *k;
} reflector;

typedef struct _rotor {
    int turnover_markers[2];
    int ring_setting;
    key *k;
} rotor;

typedef struct _plugboard {
    key *k;
} plugboard;

typedef struct _enigma {
    reflector *reflector;
    rotor *rotor_left;
    rotor *rotor_middle;
    rotor *rotor_right;
    plugboard *plugboard;
    int rotor_positions[3];
} enigma;

static int roll_in_alphabet(int i, int shift, int alphabet_size);

rotor *rotor_create(const char number[], int ringstellung);
reflector *reflector_create(const char type);
plugboard *plugboard_create(const char key_str[]);
enigma *enigma_create(rotor *rotor_left, rotor *rotor_middle, rotor *rotor_right, reflector *r, plugboard *p);
key *key_create(const char key_str[]);

void rotor_destroy(rotor *r);
void reflector_destroy(reflector *r);
void plugboard_destroy(plugboard *p);
void enigma_destroy(enigma *e);
void key_destroy(key *k);

void rotor_check(rotor *r);
void reflector_check(reflector *r);
void plugboard_check(plugboard *p);
int enigma_check(enigma *e);
int key_check(key *k);

static void key_encrypt(key *k, int i);
static void key_decrypt(key *k, int i);

/*changes structs!*/
void enigma_encrypt(enigma *e, char *text);
void rotor_rotate(rotor *r);

#endif