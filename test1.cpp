// C++ includes
#include <iostream>
#include <vector>
#include <memory>

// autodiff include
#include <autodiff/reverse.hpp>
//using namespace autodiff;

// Se calcula el valor y las derivadas de una función (x0, x1): R2 -> R.
// La función se invocará con los parámetros (x0, x1) sueltos y envueltos en un
// struct, en un std::vector<var> y en un std::shared_ptr<std::vector<var>>.
// var es el tipo que ofrece autodiff para hacer reverse mode. Para hacer forward
// mode se utiliza el tipo dual.
// Como es usual autodiff en general recomienda forward para funciones R -> Rn
// y reverse para funciones Rn -> R.

// Se define el struct de autodiff::var.
struct variables
{
    autodiff::var x0;
    autodiff::var x1;
};

// Versión que toma los parámetros sueltos.
autodiff::var f(const autodiff::var&x0, const autodiff::var& x1)
{
    return 2.0 * x0 + 3.0 * x1;
}

// Versión que toma el struct como variable de entrada.
autodiff::var g(const variables& x)
{
    return 2.0 * x.x0 + 3.0 * x.x1;
}

// Versión que toma std::vector<var>.
autodiff::var h(std::vector<autodiff::var> x)
{
    return 2.0 * x[0] + 3.0 * x[1];
}

// Versión que toma std::shared_ptr<std::vector<var>>.
autodiff::var s(const std::shared_ptr<std::vector<autodiff::var>>& x)
{
    return 2.0 * (*x)[0] + 3.0 * (*x)[1];
}

int main()
{
    // Se prueba la función f.
    autodiff::var x0 = 1.0;
    autodiff::var x1 = 1.0;

    // Se calcula el valor de la función.
    autodiff::var u = f(x0, x1);

    // El objeto Derivatives es una función que calcula las derivadas.
    autodiff::Derivatives dud = derivatives(u);

    // Ahora se calculan las derivadas.
    autodiff::var dudx0 = dud(x0);
    autodiff::var dudx1 = dud(x1);

    // Se muestra el resultado.
    std::cout << "Resultados para f" << std::endl;
    std::cout << "f: " << u << std::endl;
    std::cout << "df/dx0: " << dudx0 << std::endl;
    std::cout << "df/dx1: " << dudx1 << std::endl << std::endl;

    // Se prueba la función g.
    variables x;
    x.x0 = 2.0;
    x.x1 = 2.0;

    // Se calcula el valor de la función.
    u = g(x);

    // El objeto Derivatives es una función que calcula las derivadas.
    dud = derivatives(u);

    // Ahora se calculan las derivadas.
    dudx0 = dud(x.x0);
    dudx1 = dud(x.x1);

    // Se muestra el resultado.
    std::cout << "Resultados para g" << std::endl;
    std:: cout << "g: " << u << std::endl;
    std::cout << "dg/dx.x0: " << dudx0 << std::endl;
    std::cout << "dg/dx.x1: " << dudx1 << std::endl << std::endl;

    // Se repite para h.
    std::vector<autodiff::var> vecx {3.0, 3.0};
    u = h(vecx);
    dud = derivatives(u);
    dudx0 = dud(vecx[0]);
    dudx1 = dud(vecx[1]);
    std::cout << "Resultados para h" << std::endl;
    std::cout << "s: " << u << std::endl;
    std::cout << "ds/dx[0]: " << dudx0 << std::endl;
    std::cout << "ds/dx[1]: " << dudx1 << std::endl << std::endl;

    // Se repite para s.
    std::vector<autodiff::var> vecx2 {4.0, 4.0};
    auto sharedVecX2 = std::make_shared<std::vector<autodiff::var>>(vecx2);
    u = s(sharedVecX2);
    dud = derivatives(u);
    dudx0 = dud((*sharedVecX2)[0]);
    dudx1 = dud((*sharedVecX2)[1]);
    std::cout << "Resultados para s" << std::endl;
    std::cout << "s: " << u << std::endl;
    std::cout << "ds/d(*x)[0]: " << dudx0 << std::endl;
    std::cout << "ds/d(*x)[1]: " << dudx1 << std::endl << std::endl;
}
