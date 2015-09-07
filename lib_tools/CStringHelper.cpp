#include "CStringHelper.h"


bool is_float(const std::string & str)
{
    // creer un flux �  partir de la chaîne donnee
    std::istringstream iss(str);
    // creer un objet temporaire pour la conversion
    float tmp;
    // tenter la conversion et
    // verifier qu'il ne reste plus rien dans la chaîne
    return (iss >> tmp) && (iss.eof());
}

