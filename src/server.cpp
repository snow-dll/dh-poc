#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <gmp.h>

#include "../includes/httplib.h"
using httplib::Server;
using httplib::Request;
using httplib::Response;
#include "../includes/rand.hpp"

int main(int argc, char* argv[]){
    Server svr;

    // set up public values
    // RFC 3526 - 2048-Bit MODP Group
    mpz_t p, G, a, aG, bG, aX;
    mpz_init_set_str(p, "FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1 29024E08 8A67CC74 020BBEA6 3B139B22 514A0879 8E3404DD EF9519B3 CD3A431B 302B0A6D F25F1437 4FE1356D 6D51C245 E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE45B3D C2007CB8 A163BF05 98DA4836 1C55D39A 69163FA8 FD24CF5F 83655D23 DCA3AD96 1C62F356 208552BB 9ED52907 7096966D 670C354E 4ABC9804 F1746C08 CA18217C 32905E46 2E36CE3B E39E772C 180E8603 9B2783A2 EC07A28F B5C55DF0 6F4C52C9 DE2BCBF6 95581718 3995497C EA956AE5 15D22618 98FA0510 15728E5A 8AACAA68 FFFFFFFF FFFFFFFF", 16);
    mpz_init_set_str(G, "2", 16);
    
    // get random secret value a
    mpz_init(a);
    getRandPriv(a, 256);

    std::string p_Str = std::string(mpz_get_str(NULL, 10, p));
    std::string G_Str = std::string(mpz_get_str(NULL, 10, G));
    std::string pubVals = p_Str + std::string(".") + G_Str + std::string(".");

    std::cout << "Prime: " << p_Str << endl << endl;
    std::cout << "Generator: " << G_Str << endl << endl;

    mpz_inits(aG, bG, aX, NULL);
    // get shared value aG
    mpz_powm(aG, G, a, p);
    std::string pubA_str = std::string(mpz_get_str(NULL, 10, aG));
    static int stage = 0;
    std::string cliVals;
    mpz_t pubClient;
    mpz_init_set_str(pubClient, "0", 16);
    svr.Get("/auth", [&](const Request &req, Response &res){
        //if (stage == 0){
            res.set_content(pubVals, "text/plain");
            //stage++;
        //}
    });

    svr.Post("/auth2", [&](const auto& req, auto& res){
        cliVals = req.body;
        res.set_content(pubA_str, "text/plain");
        std::cout << "Public Key (CLIENT): " << cliVals << endl << endl;

        mpz_set_str(bG, cliVals.c_str(), 10);
        // get shared secret aX
        mpz_powm(aX, bG, a, p);
        std::string privAstr = std::string(mpz_get_str(NULL, 10, aX));
        std::cout << "Private Key (SERVER): " << privAstr << endl << endl;
    });


    svr.listen("localhost", 8087);


    return 0x0;
}
