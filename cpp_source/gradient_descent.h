#ifndef Grad_H_
#define Grad_H_
#include <cmath>

template <typename T, typename F>
T gradient_descent(T initial_value, T step_size, F math_function) {
    T current_value = initial_value;
    T current_result = math_function(current_value);
    T e = step_size / 10'000'000;

    while (true) {
        T gradient = (math_function(current_value + step_size) - current_result) / step_size;
        T next_value = current_value - step_size * gradient;
        T next_result = math_function(next_value);

        if (std::abs(next_result - current_result) < e) {
            // Converged to the minimum point
            break;
        }
        if (next_result > current_result) {
            step_size /= 2;
        }

        current_value = next_value;
        current_result = next_result;
    }

    return current_value;
}
template <typename T, typename F>
T gradient_descent(T initial_value, T step_size) {
    return gradient_descent(initial_value, step_size, F{});
}

#endif  // Q1_H