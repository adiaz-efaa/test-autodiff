//
// Created by Alvaro Diaz Valenzuela on 2019-06-02.
//

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <stdlib.h>
#include <typeinfo>

#include <autodiff/forward.hpp>
#include <autodiff/reverse.hpp>
using namespace autodiff;

#include "newton_raphson.h"
#include "curves_bonds.h"

int main()
{
    double periodicity = .5;
    double nominal = 100.0;
    std::vector<BondGenerator> bondInfo {
            {1, nominal, 1/30.0, var(.03)},
            {1, nominal, .25, var(.0295)},
            {1, nominal, .5, var(.02855)},
            {1, nominal, .75, var(.02815)},
            {1, nominal, 1.0, var(.02795)},
            {1, nominal, 1.5, var(.02725)},
            {4, nominal, periodicity, var(.027)},
            {6, nominal, periodicity, var(.0281)},
            {8, nominal, periodicity, var(.02935)},
            {10, nominal, periodicity, var(.03085)},
            {12, nominal, periodicity, var(.03185)},
            {14, nominal, periodicity, var(.0329)},
            {16, nominal, periodicity, var(.034)},
            {18, nominal, periodicity, var(.0349)},
            {20, nominal, periodicity, var(.03555)},
            {24, nominal, periodicity, var(.03625)},
            {30, nominal, periodicity, var(.0367)},
            {40, nominal, periodicity, var(.0375)}
//            {50, nominal, periodicity, var(.03845)},
//            {60, nominal, periodicity, var(.0389)}
    };
    Curve crv;
    for (const auto& info: bondInfo)
    {
        crv.push_back(ZeroRate(info.size * info.periodicity, var(.01)));
    }

    auto linInterpol = LinInterpol{};
    auto epsilon = var(EPSILON);
    int i = 0;
    std::vector<Curve> curves;
    curves.push_back(crv);
    for (const auto& info: bondInfo)
    {
        auto obj = [info, curves, linInterpol, i]
                (var discountRate, std::vector<var> bondRates) {
            auto newCrv = changeRateAt(i, discountRate, curves[i]);
            auto bond = makeBond2(info);
            return presentValue(bond, newCrv, linInterpol) - info.nominal;
        };
        auto solution = info.rate;
        std::vector<var> bondRates;
        for (const auto& bond: bondInfo)
        {
            bondRates.push_back(bond.rate);
        }
        auto newZ = newtonRaphson3(obj, solution, bondRates, epsilon);
        Derivatives dnewZd = derivatives(newZ);
        std::cout << dnewZd(bondRates[i]) << std::endl;
        auto newCrv = changeRateAt(i, newZ, curves[i]);
        for (size_t j = 0; j <= i; ++j)
        {
            newCrv = changeRateAt(j, var(newCrv[j].value.expr->val), newCrv);
        }
        curves.push_back(newCrv);
        std::cout << std::endl;
        ++i;
    }
    for (const auto& z: curves[curves.size() - 1])
    {
        std::cout << z.timeToMaturity << ", " << z.value << std::endl;
    }
}

