#include <iostream>
using std::cout;
using std::endl;
#include <string>
#include <sstream>
#include <gmp.h>

#include "../includes/httplib.h"
#include "../includes/rand.hpp"

#include <vector>

int main(int argc, char* argv[]){
  mpz_t p, G, b, aG, bG, bX;
  mpz_inits(p, G, b, aG, bG, bX, NULL);

  // get random secret value b
  getRandPriv(b, 256);

  httplib::Client cli("localhost:8087");
  auto res = cli.Get("/auth");
  std::string pubVals = res->body;
  std::stringstream vals(pubVals);
  std::string pub_p;
  std::string pub_G;

  std::vector<std::string> tokens;
  std::string inter;

  while (getline(vals, inter, '.')){
    tokens.push_back(inter);
  }
  pub_p = tokens[0];
  pub_G = tokens[1];

  mpz_set_str(p, pub_p.c_str(), 10);
  mpz_set_str(G, pub_G.c_str(), 10);
  std::cout << "Prime: " << pub_p << endl << endl;
  std::cout << "Generator: " << pub_G << endl << endl;

  mpz_powm(bG, G, b, p);
  std::string str_bG = mpz_get_str(NULL, 10, bG);

  res = cli.Post("/auth2", str_bG, "text/plain");
  std::string privVal = res->body;
  std::cout << "Public Key (SERVER): " << privVal << endl << endl;
  mpz_set_str(aG, privVal.c_str(), 10);
  mpz_powm(bX, aG, b, p);
  std::string privBstr = std::string(mpz_get_str(NULL, 10, bX));
  std::cout << "Private Key (CLIENT): " << privBstr << endl << endl;
}
