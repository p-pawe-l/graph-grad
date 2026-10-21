#include <memory>
#include <fstream>
#include <string>
#include <iostream>

#include "../include/graph.hpp"
#include "../include/val.hpp"

#define LEFT (0)
#define RIGHT (1)

namespace {
    inline auto opNameDisptach(MathOperation op) -> std::string {
        switch (op) {
        case MathOperation::ADD: return "+"; break;
        case MathOperation::SUB: return "-"; break;
        case MathOperation::MUL: return "*"; break;
        case MathOperation::DIV: return "/"; break;
        case MathOperation::NONE: return "none"; break;
        }
    }

    inline auto constructGrLine(
        const std::string& l, 
        const double d, 
        const MathOperation o, 
        const std::string& c1l, 
        const std::string& c2l
    ) -> std::string {
        return l + " | " + std::to_string(d) + " | " + opNameDisptach(o) + " | " + c1l + " | " + c2l; 
    }

    inline auto constructGrLineFromNode(const value_node& pvn) -> std::string {
        std::string none = "none";
        return constructGrLine(
            pvn.label_,
            pvn.data_,
            pvn.op_,
            pvn.left_ != nullptr ? pvn.left_->label_ : none,
            pvn.right_ != nullptr ? pvn.right_->label_ : none
        );
    }

    auto traverseGraphAndCollectGrLines(const value_node& root, std::vector<std::string>& gr_lines) -> void {
        gr_lines.push_back(constructGrLineFromNode(root));
        if (root.op_ == MathOperation::NONE) {
            // We are in the leaf
            return;
        }
        traverseGraphAndCollectGrLines(*root.left_, gr_lines);
        traverseGraphAndCollectGrLines(*root.right_, gr_lines);
    }
}

std::unique_ptr<value_node> buildGraphFromV(V<double>& root) {
    value_node out;
    // We are on the bottom of the tree (in the leaf)
    if (root.getChildren().empty()) {
        out = {
            .data_ = root.getData(),
            // No operation created this leaf
            .op_ = MathOperation::NONE,
            .label_ = root.getLabel(),
            .left_ = nullptr,
            .right_ = nullptr
        };
        return std::make_unique<value_node>(std::move(out));
    }
        
    out = {
        .data_ = root.getData(),
        .op_ = root.getOp(),
        .label_ = root.getLabel(),
        .left_ = buildGraphFromV(*root.getChildren().at(LEFT)),
        .right_ = buildGraphFromV(*root.getChildren().at(RIGHT))
    };
    return std::make_unique<value_node>(std::move(out));
}

auto saveGraphToFile(std::unique_ptr<value_node> root, std::string& filename) -> void {
    std::ofstream graph_file(filename);
    std::vector<std::string> lines;
    traverseGraphAndCollectGrLines(*root, lines);
    
    if (graph_file.is_open()) {
        for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
            graph_file << *it << "\n";
        }    
    }
}