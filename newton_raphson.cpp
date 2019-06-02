//
// Created by Alvaro Diaz Valenzuela on 2019-06-01.
//

#include <functional>
#include <string>
#include <stdlib.h>

#include "newton_raphson.h"


int main()
{
    std::string which = "o";
    var guess = .01;
    var epsilon = 0.00000001;
    var rate = .03;
    var sol;
    if (which == "compound")
    {
        sol = newtonRaphson2(compoundDiscounting, guess, rate, epsilon);
    }
    else
    {
        sol = newtonRaphson2(linearDiscounting, guess, rate, epsilon);
    }
    Derivatives dsold = derivatives(sol);
    std::cout << "La solución es: " << sol << std::endl;
    std::cout << "La derivada dsol/drate: " << dsold(rate) << std::endl;

    return 0;
}
