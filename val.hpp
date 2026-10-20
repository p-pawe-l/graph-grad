#pragma once 

#include <cstdint>
#include <vector>
#include <initializer_list>

enum class MathOperation : uint8_t {
    NONE = 0,
    ADD,
    SUB,
    MUL,
    DIV,
};

template <typename T>
class V {
private:    
    T data_; 
    double grad_ = 0.0;
    std::vector<V<T>*> _children_;
    MathOperation _op_ = MathOperation::NONE;

    V(T data, std::initializer_list<V<T>*> _children, MathOperation _op):
    data_(std::move(data)), _children_(std::move(_children)), _op_(_op)
    {
        _children_.reserve(_children.size());
        auto it = _children.begin();
        for (int i = 0; i < _children.size(); ++i) 
            _children_[i] = *it++;
    }

public:
    V(T data): data_(std::move(data)) {}

    [[nodiscard]] auto getData() const noexcept -> T { return data_; }
    [[nodiscard]] auto getGrad() const noexcept -> T { return grad_; }
    
    auto setData(T data) noexcept -> void { data_ = data; }
    auto setGrad(T grad) noexcept -> void { grad_ = grad; }
    auto storeGrad(T grad) noexcept -> void { grad_ += grad; }

    auto backGradToChildren() -> void {
        switch (_op_) {
        case MathOperation::ADD:
            for (auto& child : _children_) child->storeGrad(grad_);
            break;
        case MathOperation::SUB:
            for (auto& child: _children_) child->storeGrad(-grad_);
            break;
        case MathOperation::MUL:
            for (int i = 0; i < 2; ++i) {
                auto g = _children_[i == 0 ? 1 : 0]->getData();
                _children_[i]->storeGrad(g * grad_);
            }
            break;
        default:
            break;
        }
    }

    auto backward() -> void {
        if (_children_.empty()) return;
        backGradToChildren();
        for (auto& child : _children_) {
            child->backward();
        }
    }

    auto wrp_backward() -> void {
        grad_ = 1.0;
        backward();
    }

    auto operator+(V<T>& other) -> V<T> {
        V<T> out(
            data_ + other.getData(), 
            {this, &other},
            MathOperation::ADD
        );  
        return out;
    } 
    
    auto operator-(V<T>& other) -> V<T> {
        V<T> out(
            data_  - other.getData(),
            {this, &other},
            MathOperation::SUB
        );
        return out;
    }
 
    auto operator*(V<T>& other) -> V<T> {
        V<T> out(
            data_ * other.getData(),
            {this, &other},
            MathOperation::MUL
        );
        return out;
    }

    auto operator/(V<T>& other) -> V<T> {
        V<T> out(
            data_ / other.getData(),
            {this, &other},
            MathOperation::DIV
        );
        return out;
    }
};