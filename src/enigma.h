#ifndef ENIGMA_H_INCLUDED
#define ENIGMA_H_INCLUDED

typedef struct _reflector {
    void *key;
} reflector;

typedef struct _rotor {
    char turnover_markers[2];
    int ringstellung;
    void *key;
} rotor;

typedef struct _plugboard {
    char turnover_markers[2];
    int ringstellung;
    void *key;
} plugboard;

typedef struct _enigma {
    reflector *reflector;
    rotor *rotor_left;
    rotor *rotor_middle;
    rotor *rotor_right;
    void *plugboard_key;
    int rotor_positions[3];
} enigma;

static int roll_in_alphabet(int i, int shift, int alphabet_size);

rotor *rotor_create(const char number[], int ringstellung);
reflector *reflector_create(const char type);
plugboard *plugboard_create(const char key[]);
enigma *enigma_create(rotor rotor_left, rotor rotor_middle, rotor rotor_right, reflector r, plugboard p);

void rotor_destroy(rotor *r);
void reflector_destroy(reflector *r);
void plugboard_destroy(plugboard *p);
void enigma_destroy(enigma *e);

/*changes structs!*/
void encrypt(enigma *e, char *text);
void rotate_rotor(rotor *r);

#endif