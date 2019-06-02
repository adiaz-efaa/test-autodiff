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


struct Cashflow
{
    //Cashflow(double t, double monto): timeToMaturity(t), amount(monto) {}
    double timeToMaturity;
    var amount;
};


typedef std::vector<Cashflow> Bond;
Bond makeBond(size_t num, double nominal, double periodicity, const var& rate)
{
    std::vector<Cashflow> result;
    result.resize(num);
    for (size_t i = 0; i < num - 1; ++i)
    {
        Cashflow c {(i + 1) * periodicity, nominal * rate * periodicity};
        result[i] = c;
    }
    Cashflow c {num * periodicity, nominal * (1 + rate * periodicity)};
    result[num - 1] = c;
    return result;
}


struct ZeroRate
{
    ZeroRate(double t, var z): timeToMaturity(t), value(z) {}
    double timeToMaturity;
    var value;
};
typedef std::vector<ZeroRate> Curve;


class LinInterpol
{
public:
    LinInterpol() {}

    var operator () (double t, const Curve& crv) const
    {
        auto lessThan = [](const ZeroRate& zr, double t){
            return zr.timeToMaturity < t;
        };
        auto const it = std::lower_bound(crv.begin(), crv.end(), t, lessThan);
        if (it == crv.end())
        {
            return (it - 1)->value;
        }
        if (it == crv.begin())
        {
            return it->value;
        }
        auto previous = std::prev(it);
        var result = previous->value +
                     (t - it->timeToMaturity) / (it->timeToMaturity - previous->timeToMaturity) *
                     (it->value - previous->value);
        return result;
    }
};


var presentValue(const Bond& bond, const Curve& crv, const LinInterpol& interpolator)
{
    var result = 0.0;
    for (const auto& cashflow: bond)
    {
        auto tMat = cashflow.timeToMaturity;
        result += cashflow.amount / pow(1 + interpolator(tMat, crv), tMat);
    }
    return result;
}


Curve changeRateAt(size_t index, const var& value,  const Curve& curve)
{
    auto result = curve;
    auto maturity = result.at(index).timeToMaturity;
    result.at(index) = ZeroRate(maturity, value);
    return result;
}


int main()
{
    var rate1 = var(.03);
    auto bond1 = makeBond(1, 100, 1, rate1);
    var rate2 = var(.035);
    auto bond2 = makeBond(2, 100, 1, rate2);

    Curve crv;
    crv.push_back(ZeroRate(1.0, var(.01)));
    crv.push_back(ZeroRate(2.0, var(.02)));

    auto linInterpol = LinInterpol {};

    auto epsilon = var(.0001);
    auto obj1 = [crv, linInterpol](var discountRate, var bondRate){
        auto newCrv = changeRateAt(0, discountRate, crv);
        auto bond = makeBond(1, 100.0, 1, bondRate);
        return presentValue(bond, newCrv , linInterpol) - 100.0;
    };
    auto solution1 = var(.01);
    auto newZ1 = newtonRaphson2(obj1, solution1, rate1, epsilon);
    auto crv1 = changeRateAt(0, newZ1, crv);

    std::cout << std::endl << "bond1: " << presentValue(bond1, crv1, linInterpol) << std::endl;
    Derivatives dnewZ1d = derivatives(newZ1);
    std::cout << "La solución es: " << newZ1 << std::endl;
    std::cout << "La derivada dnewZ1/drate1: " << dnewZ1d(rate1) << std::endl;

    auto obj2 = [crv1, linInterpol](var discountRate, var bondRate){
        auto newCrv = changeRateAt(1, discountRate, crv1);
        auto bond = makeBond(2, 100.0, 1, bondRate);
        return presentValue(bond, newCrv , linInterpol) - 100.0;
    };
    auto solution2 = var(.01);
    auto newZ2 = newtonRaphson2(obj1, solution2, rate2, epsilon);
    auto crv2 = changeRateAt(0, newZ2, crv);
    std::cout << std::endl << "bond2: " << presentValue(bond2, crv2, linInterpol) << std::endl;
    Derivatives dnewZ2d = derivatives(newZ2);
    std::cout << "La solución es: " << newZ2 << std::endl;
    std::cout << "La derivada dnewZ2/drate2: " << dnewZ2d(rate2) << std::endl;


}

/*
std::cout << std::endl << "Función obj1" << std::endl;
auto dRate = var(.01);
auto bRate = var(.03);
std::cout << dRate << ", " << obj1(dRate, bRate) << std::endl;
*/

/*
std::cout << "Curva" << std::endl;
for (const auto& z: crv)
{
std::cout << z.timeToMaturity << ", " << z.value << std::endl;
}
*/

/*
auto newCrv = changeRateAt(0, var(.02), crv);
std::cout << std::endl << "Nueva Curva" << std::endl;
for (const auto& z: newCrv)
{
std::cout << z.timeToMaturity << ", " << z.value << std::endl;
}
std::cout << "bond1: " << presentValue(bond1, crv, linInterpol) << std::endl;
std::cout << "bond2: " << presentValue(bond2, crv, linInterpol) << std::endl;
*/

