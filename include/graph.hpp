#pragma once

#include <memory>

#include "val.hpp"

struct value_node;


struct value_node {
    using Pvn = std::unique_ptr<value_node>;
    double data_;
    double grad_;
    MathOperation op_;
    std::string label_;

    Pvn left_;
    Pvn right_;    
};


/*
* Builds tree representing opeartions performed
* on given root value 
*/
auto buildGraphFromV(V<double>& root) -> std::unique_ptr<value_node>;

/*
* Saves representation of the graph in filename
* with .gr ending 
*/
auto saveGraphToFile(std::unique_ptr<value_node> root, std::string& filename) -> void;