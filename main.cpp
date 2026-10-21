#include "include/graph.hpp"
#include "include/val.hpp"


/*
* L = ((a * b + c) * (a - d)) * (f * g + a)
*
* Every intermediate result is stored in a named variable,
* because V keeps raw pointers to its operands
*/
int main(void) {
    V<double> a(2.0, "a");
    V<double> b(-3.0, "b");
    V<double> c(10.0, "c");
    V<double> d(4.0, "d");
    V<double> f(-2.0, "f");
    V<double> g(3.0, "g");

    V<double> e = a * b;  e.setLabel("e");   // -6
    V<double> h = e + c;  h.setLabel("h");   //  4
    V<double> k = a - d;  k.setLabel("k");   // -2
    V<double> m = h * k;  m.setLabel("m");   // -8
    V<double> n = f * g;  n.setLabel("n");   // -6
    V<double> p = n + a;  p.setLabel("p");   // -4
    V<double> L = m * p;  L.setLabel("L");   // 32

    L.backward();
    auto root = buildGraphFromV(L);
    std::string filename = "graphs/graph.gr";
    saveGraphToFile(std::move(root), filename);
    return 0;
}
