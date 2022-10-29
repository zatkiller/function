
#include <iostream>
#include <memory>
#include <functional>

template<typename T>
struct function;

template<typename R, typename ...Args>
struct function<R(Args...)> {

    template<typename FunctionObject>
    function(FunctionObject fo) : callable(std::make_unique<callable_impl<FunctionObject>>(fo)) {} 

    R operator()(Args... args) {
        return callable->call(args...);
    };

private:
    struct callable_interface {
        virtual R call(Args...) = 0;
        virtual ~callable_interface() = default;
    };

    struct callable_impl : callable_interface {
        Callback callback;
        callable_impl(Callback callback_) : callback(std::move(callback_)) {}
        
        R call(Args ...args) {
            return std::invoke(callback, args...);        }
    };

    std::unique_ptr<callable_interface> callable;
};

int f(int x, int y) {
    return x + y;
}

struct A {
    int f(int x, int y) {
        return 100 * x * y;
    }
};

int main() {
    // function<int> f1; -> uncomment should throw error
    function<int(int,int)> f2(f);
    function<int(int,int)> f3([](int a, int b){return a * b;});
    function<int(A&,int,int)> f4(&A::f);
    assert(f2(1, 2) == 3); 
    assert(f3(2, 3) == 6); 
    A a;
    assert(f4(a, 2, 3) == 600); 

    return 0;
}
