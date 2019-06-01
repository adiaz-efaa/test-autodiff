//
// Created by Alvaro Diaz Valenzuela on 2019-06-01.
//

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stdlib.h>

#include <autodiff/forward.hpp>
#include <autodiff/reverse.hpp>
using namespace autodiff;

typedef std::function<var(var, var)> varvar2var;

var newtonRaphson2(varvar2var f, var solution, const var& parameter, const var& epsilon)
{
    var error = 10.0 * val(epsilon);
    var fx;
    var dfdx;
    Derivatives dfd;
    std::vector<var> solutions;
    int i = 0;
    solutions.push_back(solution);
    while (error > epsilon)
    {
        fx = f(solutions[i], parameter);
        dfd = derivatives(fx);
        dfdx = dfd(solutions[i]);
        if (dfdx == 0.0)
        {
            std::cout << "derivada 0" << std::endl;
            break;
        }
        solutions.push_back(solutions[i]- fx / dfdx);
        error = abs(solutions[i + 1] - solutions[i]);
        ++i;
    }
    return solutions[i];
};


var compoundDiscounting(var dr, var r)
{
    return 1000.0 * (1 + .5 * r) / pow((1 + dr), .5) - 1000.0;
}

var linearDiscounting(var dr, var r)
{
    return 1000.0 * (1 + .5 * r) / (1 + .5 * dr) - 1000.0;
}

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
