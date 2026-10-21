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
public:
    using Pv = V<T>*;
    
private:    

    T data_; 
    double grad_ = 0.0;
    std::vector<Pv> __children_;
    MathOperation __op_ = MathOperation::NONE;
    std::function<void(double)> __grad_func_ = nullptr;
    std::string __label_;

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

    auto unrwp_calcVal() -> double {
        if (__children_.empty()) return data_;

        auto res = disptachOperation(
            __op_,
            __children_[0]->unrwp_calcVal(),
            __children_[1]->unrwp_calcVal()
        );
        return res;
    }
    
    auto storeGrad(T grad) noexcept -> void { grad_ += grad; }
    
    auto disptachOperation(MathOperation op, double v1, double v2) -> double {
        switch (op) {
            case MathOperation::ADD:
                return v1 + v2;
                break;
            case MathOperation::SUB:
                return v1 - v2;
                break;
            case MathOperation::MUL:
                return v1 * v2;
                break;
            case MathOperation::DIV:
                return v1 / v2;
                break;
            case MathOperation::NONE:
                return v1;
                break;
        }
    } 

public:
    V(T data): data_(data), __label_("") {}
    V(T data, std::string&& label): data_(std::move(data)), __label_(std::move(label)) {}

    [[nodiscard]] auto getData() const noexcept -> T { return data_; }
    [[nodiscard]] auto getGrad() const noexcept -> double { return grad_; }
    [[nodiscard]] auto getChildren() noexcept -> std::vector<Pv>& { return __children_; }
    [[nodiscard]] auto getLabel() noexcept -> std::string& { return __label_; }
    [[nodiscard]] auto getOp() const noexcept -> MathOperation { return __op_; }

    auto setData(T data) noexcept -> void { data_ = data; }
    auto setGrad(double grad) noexcept -> void { grad_ = grad; }
    auto setLabel(std::string&& label) noexcept -> void { __label_ = std::move(label); }

    auto backward() -> void {
        grad_ = 1.0;
        unwrp_backward();
    }

    auto calcVal() -> V<T>& {
        data_ = unrwp_calcVal();
        return *this;
    }

    auto applyGrad(double step) -> void {
        if (__children_.empty()) return;

        for (auto& child : __children_) {
            child->setData(
                child->getData() - step * child->getGrad()
            );
            child->applyGrad(step);
        }
    }

    auto zeroGrad() -> void {
        if (__children_.empty()) return;

        for (auto& child: __children_) {
            child->setGrad(0.0);
            child->zeroGrad();
        }
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
                this->storeGrad(g);
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

template <typename T>
auto minimize(V<T> *root, double step, int runs) -> void {
    int i = runs;
    do {
        root->backward();
        root->applyGrad(step);
        root->zeroGrad();
    } while (runs-- > 0);
} 