# test-autodiff
Pruebas de la librería AutoDiff (C++) utilizando struct, std::vector y std::shared_ptr&lt;std::vector&lt;>>.

Se calcula el valor y las derivadas de una función (x0, x1): R2 -> R.
La función se invocará con los parámetros (x0, x1) sueltos y envueltos en un:
- struct,
- en un std::vector<var>
- y en ustd::shared_ptr<std::vector<var>>.

var es el tipo que ofrece autodiff para hacer reverse mode. Para hacer forward mode se utiliza el tipo dual.
Como es usual autodiff en general recomienda forward para funciones R -> Rn y reverse para funciones Rn -> R.

