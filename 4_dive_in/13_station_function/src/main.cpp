// Copyright rutura/The-C-20-Masterclass-Source-Code 2025
#include <iostream>

#include "src/number/number.hpp"

int main() {
    // 這樣子也可以
    // int first_num{13};
    int first_num = {13};
    int second_num = {7};

    number::println("first_num", first_num);
    number::println("second_num", second_num);

    int sum = number::sum(first_num, second_num);
    number::println("sum", sum);

    sum = number::sum(1, 2);
    number::println("sum", sum);

    return 0;
}
