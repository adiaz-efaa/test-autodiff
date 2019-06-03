//
// Created by Alvaro Diaz Valenzuela on 2019-06-02.
//

#ifndef AUTODIFF_CURVES_BONDS_H
#define AUTODIFF_CURVES_BONDS_H

#include <autodiff/forward.hpp>
#include <autodiff/reverse.hpp>
using namespace autodiff;


struct Cashflow
{
    //Cashflow(double t, double monto): timeToMaturity(t), amount(monto) {}
    double timeToMaturity;
    var amount;
};


typedef std::vector<Cashflow> Bond;
struct BondGenerator
{
    size_t size;
    double nominal;
    double periodicity;
    var rate;
};
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

Bond makeBond2(const BondGenerator& bondGenerator)
{
    return makeBond(bondGenerator.size, bondGenerator.nominal, bondGenerator.periodicity,
            bondGenerator.rate);
}

void showBond(const Bond& bond)
{
    for (const auto& cashflow: bond)
    {
        std::cout << cashflow.timeToMaturity << ", " << cashflow.amount << std::endl;
    }
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
                     (t - previous->timeToMaturity) / (it->timeToMaturity - previous->timeToMaturity) *
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
        auto rate = interpolator(tMat, crv);
        // std::cout << "interpolated rate: " << rate << std::endl;
        result += cashflow.amount / pow(1 + rate, tMat);
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


#endif //AUTODIFF_CURVES_BONDS_H
