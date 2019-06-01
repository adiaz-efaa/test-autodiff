//
// Created by Alvaro Diaz Valenzuela on 2019-06-01.
//

#include <autodiff/forward.hpp>
using namespace autodiff;

typedef std::function<dual(dual)> dual2dual;
typedef std::function<dual(dual, dual)> dualdual2dual;

dual newtonRaphson(dual2dual f, dual solution, dual epsilon)
{
    double error = 10.0 * epsilon.val;
    dual fx;
    double dfdx;
    while (error > epsilon.val)
    {
        fx = f(solution);
        dfdx = derivative(f, wrt(solution), solution);
        auto previous = solution;
        solution.val -= fx.val / dfdx;
        error = abs(solution.val - previous.val);
    }
    return solution;
};

dual parabola(dual x)
{
    return x * x - 9.0;
}

dual deposit(dual rate)
{
    double monto = 1000.0;
    return monto * (1 + rate.val);
}

dual presentValue(dual discountRate, dual rate, dual2dual f)
{
    return f(rate) / (1 + discountRate.val);
}


