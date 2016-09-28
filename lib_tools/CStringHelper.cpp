﻿#include "CStringHelper.h"


bool is_float(const std::string & str)
{
    // creer un flux Ã  partir de la chaÃ®ne donnee
    std::istringstream iss(str);
    // creer un objet temporaire pour la conversion
    float tmp;
    // tenter la conversion et
    // verifier qu'il ne reste plus rien dans la chaÃ®ne
    return (iss >> tmp) && (iss.eof());
}

