#ifndef ENIGMA_H_INCLUDED
#define ENIGMA_H_INCLUDED

typedef int error_msg;
#define ALL_FINE 0
#define INVALID_PARAMETERS 1
#define ENCRYPTION_ERROR 2

typedef int key[26];

typedef struct _reflector {
    key *k;
} reflector;

typedef struct _rotor {
    int turnover_markers[2];
    int position;
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
} enigma;

static int roll_in_alphabet(int i, int shift, int alphabet_size);

rotor *rotor_create(const char number[], int ring_setting);
reflector *reflector_create(const char type);
plugboard *plugboard_create(const char key_str[]);
enigma *enigma_create(rotor *rotor_left, rotor *rotor_middle, rotor *rotor_right, reflector *r, plugboard *p);
key *key_create(const char key_str[]);

void rotor_destroy(rotor *r);
void reflector_destroy(reflector *r);
void plugboard_destroy(plugboard *p);
void enigma_destroy(enigma *e);
void key_destroy(key *k);

error_msg rotor_check(rotor *r);
error_msg reflector_check(reflector *r);
error_msg plugboard_check(plugboard *p);
error_msg enigma_check(enigma *e);
error_msg key_check(key *k);

static error_msg key_encrypt(key *k, int i);
static error_msg key_decrypt(key *k, int i);
static error_msg rotor_encrypt(rotor *r, int i);
static error_msg rotor_decrypt(rotor *r, int i);
static error_msg reflector_crypt(reflector *r, int i);
static error_msg plugboard_crypt(plugboard *p, int i);

/*changes structs!*/
error_msg enigma_encrypt(enigma *e, char text[]);
error_msg rotor_rotate(rotor *r);

#endif