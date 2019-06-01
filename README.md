# Read Me

AutoDiff[https://github.com/autodiff/autodiff] es una librería en C++17 para diferenciación automática. Es un header only library que implementa forward y reverse mode.

## Pruebas de la librería AutoDiff (C++)

### 1 Casos Base
Se calcula el valor y las derivadas de una función (x0, x1): R2 -> R.
La función se invocará con los parámetros (x0, x1) sueltos y envueltos en un:
- `struct`,
- en un `std::vector<var>`
- y en un `std::shared_ptr<std::vector<var>>`.

`var` es el tipo que ofrece autodiff para hacer reverse mode. Para hacer forward mode se utiliza el tipo `dual`.
Como es usual autodiff en general recomienda forward para funciones R -> Rn y reverse para funciones Rn -> R.

### 2 Newton-Raphson
Se calcula la derivada de una solución obtenida con el método de Newton-Raphson. La función objetivo es el valor presente
de un depósito a 6M menos el valor del nominal. El objetivo es dar el primer paso para un algoritmo de Bootstrapping que
junto con la curva devuelva las derivadas de las tasas cero respecto a los inputs utilzados (familia de swaps y tasas cero iniciales).

