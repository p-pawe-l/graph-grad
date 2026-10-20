#pragma once 

#include <cstdint>
#include <utility>
#include <initializer_list>
#include <functional>

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
    using Pv = V<T>*;

    T data_; 
    double grad_ = 0.0;
    std::vector<Pv> __children_;
    MathOperation __op_ = MathOperation::NONE;
    std::function<void(double)> __grad_func_ = nullptr;

    V(
        T data, 
        std::initializer_list<V<T>*> __children, 
        MathOperation __op,
        std::function<void(double)> __grad_func
    ):
        data_(std::move(data)), 
        __children_(__children), 
        __op_(__op), 
        __grad_func_(__grad_func)
    {}

    auto unwrp_backward() -> void {
        if (__children_.empty() || __grad_func_ == nullptr) return;
        
        __grad_func_(grad_);
        for (auto& child : __children_) {
            child->unwrp_backward();
        }
    }
    
    auto storeGrad(T grad) noexcept -> void { grad_ += grad; }

public:
    V(T data): data_(std::move(data)) {}

    [[nodiscard]] auto getData() const noexcept -> T { return data_; }
    [[nodiscard]] auto getGrad() const noexcept -> double { return grad_; }
    
    auto setData(T data) noexcept -> void { data_ = data; }
    auto setGrad(double grad) noexcept -> void { grad_ = grad; }

    auto backward() -> void {
        grad_ = 1.0;
        unwrp_backward();
    }

    [[nodiscard]] auto operator+(V<T>& other) -> V<T> {
        V<T> out(
            data_ + other.getData(), 
            {this, &other},
            MathOperation::ADD,
            [&](double g) {
                this->storeGrad(g);
                other.storeGrad(g);
            }
        );  
        return out;
    } 
    
    [[nodiscard]] auto operator-(V<T>& other) -> V<T> {
        V<T> out(
            data_  - other.getData(),
            {this, &other},
            MathOperation::SUB,
            [&](double g) {
                this->storeGrad(-g);
                other.storeGrad(-g);
            }
        );
        return out;
    }
 
    [[nodiscard]] auto operator*(V<T>& other) -> V<T> {
        V<T> out(
            data_ * other.getData(),
            {this, &other},
            MathOperation::MUL,
            [&](double g) {
                this->storeGrad(other.getData() * g);
                other.storeGrad(this->getData() * g);
            }
        );
        return out;
    }

    [[nodiscard]] auto operator/(V<T>& other) -> V<T> {
        V<T> out(
            data_ / other.getData(),
            {this, &other},
            MathOperation::DIV
        );
        return out;
    }
};