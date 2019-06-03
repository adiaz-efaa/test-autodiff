//
// Created by Alvaro Diaz Valenzuela on 2019-06-02.
//

#ifndef AUTODIFF_NEWTON_RAPHSON_H
#define AUTODIFF_NEWTON_RAPHSON_H

#include <iostream>
#include <vector>
#include <initializer_list>

#include <autodiff/forward.hpp>
#include <autodiff/reverse.hpp>
using namespace autodiff;

const double EPSILON = .0001;

typedef std::function<var(var, var)> varvar2var;
var newtonRaphson2(varvar2var f, const var& solution, const var& parameter, const var& epsilon)
{
    var error = 10.0 * val(epsilon);
    var fx;
    var dfdx;
    Derivatives dfd;
    std::vector<var> solutions;
    int i = 0;

    // En este vector se guarda la sucesión de soluciones.
    // Se debe hacer de esta manera porque si se muta el valor de la
    // solución anterior, la variable dfdx (línea 34) se contamina y
    // en la segunda iteración se hace igual a cero.
    solutions.push_back(solution);
    while (error > epsilon)
    {
        //std::cout << error << std::endl;
        fx = f(solutions[i], parameter);
        dfd = derivatives(fx);
        dfdx = dfd(solutions[i]);
        if (dfdx == 0.0)
        {
            std::cout << "Derivada = 0" << std::endl;
            break;
        }
        solutions.emplace_back(solutions[i]- fx / dfdx);
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

typedef std::function<var(var, std::vector<var>)> varVectorVar2var;
var newtonRaphson3(varVectorVar2var f, const var& solution, const std::vector<var>& parameters,
        const var& epsilon)
{
    var error = 10.0 * val(epsilon);
    var fx;
    var dfdx;
    Derivatives dfd;
    std::vector<var> solutions;
    int i = 0;

    // En este vector se guarda la sucesión de soluciones.
    // Se debe hacer de esta manera porque si se muta el valor de la
    // solución anterior, la variable dfdx (línea 34) se contamina y
    // en la segunda iteración se hace igual a cero.
    solutions.push_back(solution);
    while (error > epsilon)
    {
        //std::cout << error << std::endl;
        fx = f(solutions[i], parameters);
        dfd = derivatives(fx);
        dfdx = dfd(solutions[i]);
        if (dfdx == 0.0)
        {
            std::cout << "Derivada = 0" << std::endl;
            break;
        }
        solutions.push_back(solutions[i]- fx / dfdx);
        error = abs(solutions[i + 1] - solutions[i]);
        ++i;
    }
    return solutions[i];
};

#endif //AUTODIFF_NEWTON_RAPHSON_H
