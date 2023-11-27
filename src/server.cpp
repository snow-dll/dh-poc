#include <iostream>
using std::cout;
using std::endl;
#include <gmp.h>

#include "../includes/rand.hpp"

int main(int argc, char* argv[]){
    // set up public values
    // RFC 3526 - 2048-Bit MODP Group
    mpz_t p, G, a, b, aG, bG, aX, bX;
    mpz_init_set_str(p, "FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1 29024E08 8A67CC74 020BBEA6 3B139B22 514A0879 8E3404DD EF9519B3 CD3A431B 302B0A6D F25F1437 4FE1356D 6D51C245 E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE45B3D C2007CB8 A163BF05 98DA4836 1C55D39A 69163FA8 FD24CF5F 83655D23 DCA3AD96 1C62F356 208552BB 9ED52907 7096966D 670C354E 4ABC9804 F1746C08 CA18217C 32905E46 2E36CE3B E39E772C 180E8603 9B2783A2 EC07A28F B5C55DF0 6F4C52C9 DE2BCBF6 95581718 3995497C EA956AE5 15D22618 98FA0510 15728E5A 8AACAA68 FFFFFFFF FFFFFFFF", 16);
    mpz_init_set_str(G, "2", 16);

    // get random secret value a
    mpz_init(a);
    getRandomPriv(a, 256);
    
    // get random secret value b
    mpz_init(b);
    getRandomPriv(b, 256);

    mpz_inits(aG, bG, aX, bX);
    // get shared value aG
    mpz_powm(aG, G, a, p);
    // get shared value bG
    mpz_powm(bG, G, b, p);
    // get shared secret aX
    mpz_powm(aX, bG, a, p);
    // get shared secret bX
    mpz_powm(bX, aG, b, p);

    gmp_printf("private secret (A): %Zd\n\n", a);
    gmp_printf("private secret (B): %Zd\n\n", b);
    gmp_printf("public (A): %Zd\n\n", aG);
    gmp_printf("public (B): %Zd\n\n", bG);
    gmp_printf("shared secret (A): %Zd\n\n", aX);
    gmp_printf("shared secret (B): %Zd\n\n", bX);

    return 0x0;
}