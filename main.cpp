#include "include/graph.hpp"
#include "include/val.hpp"


int main(void) {
    V<double> a(10.0, "a");
    V<double> b(10.0, "b");
    V<double> c(10.0, "c");

    V<double> d = a * b;
    d.setLabel("d");
    auto root = buildGraphFromV(d);
    std::string filename = "graph.gr";
    saveGraphToFile(std::move(root), filename);
    return 0;
}