#include <gmp.h>

#ifndef RAND_HPP_
#define RAND_HPP_

int getRandInt(int len){
    FILE* urandom = fopen("/dev/urandom", "rb");
    int ct;
    do {
        ct = fgetc(urandom);
    } while (ct >= (UCHAR_MAX + 1) / len * len);
    fclose(urandom);
    return ct % len;
}

void getRandomPriv(mpz_t& priv, size_t bitct){
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, getRandInt(256));
    mpz_urandomb(priv, state, bitct);
}



#endif