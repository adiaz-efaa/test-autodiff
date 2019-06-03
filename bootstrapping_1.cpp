//
// Created by Alvaro Diaz Valenzuela on 2019-06-01.
//

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <stdlib.h>

#include <autodiff/forward.hpp>
#include <autodiff/reverse.hpp>
using namespace autodiff;

#include "newton_raphson.h"
#include "curves_bonds.h"

int main()
{
    double periodicity = 1.0;
    var rate1 = var(.03);
    auto bond1 = makeBond(1, 100, periodicity, rate1);
    //showBond(bond1);
    var rate2 = var(.035);
    auto bond2 = makeBond(2, 100, periodicity, rate2);
    //showBond(bond2);

    Curve crv;
    crv.push_back(ZeroRate(1.0, var(.01)));
    crv.push_back(ZeroRate(2.0, var(.02)));
    auto linInterpol = LinInterpol {};

    auto pv2 = presentValue(bond2, crv, linInterpol);


    auto epsilon = var(EPSILON);
    auto obj1 = [periodicity, crv, linInterpol](var discountRate, var bondRate){
        auto newCrv = changeRateAt(0, discountRate, crv);
        auto bond = makeBond(1, 100.0, periodicity, bondRate);
        return presentValue(bond, newCrv , linInterpol) - 100.0;
    };
    auto solution1 = rate1;
    auto newZ1 = newtonRaphson2(obj1, solution1, rate1, epsilon);
    auto crv1 = changeRateAt(0, newZ1, crv);

    std::cout << std::endl << "bond1: " << presentValue(bond1, crv1, linInterpol) << std::endl;
    Derivatives dnewZ1d = derivatives(newZ1);
    std::cout << "La solución es: " << newZ1 << std::endl;
    std::cout << "La derivada dnewZ1/drate1: " << dnewZ1d(rate1) << std::endl;

    auto obj2 = [periodicity, crv1, linInterpol](var discountRate, var bondRate){
        auto newCrv = changeRateAt(1, discountRate, crv1);
        auto bond = makeBond(2, 100.0, periodicity, bondRate);
        return presentValue(bond, newCrv , linInterpol) - 100.0;
    };
    auto solution2 = rate2;
    auto newZ2 = newtonRaphson2(obj2, solution2, rate2, epsilon);
    auto crv2 = changeRateAt(1, newZ2, crv1);
    std::cout << std::endl << "bond2: " << presentValue(bond2, crv2, linInterpol) << std::endl;
    Derivatives dnewZ2d = derivatives(newZ2);
    std::cout << "La solución es: " << newZ2 << std::endl;
    std::cout << "La derivada dnewZ2/drate2: " << dnewZ2d(rate2) << std::endl;
}
