// Copyright rutura/The-C-20-Masterclass-Source-Code 2025
#include <iostream>

// What is constexpr: https://tjsw.medium.com/%E6%BD%AE-c-constexpr-ac1bb2bdc5e2
// What is consteval: https://tjsw.medium.com/%E6%BD%AE-c-20-consteval-constexpr-%E7%9A%84%E5%A5%BD%E5%85%84%E5%BC%9F-bfbcfdd4c763
consteval int get_value() {
    return 3;
}

using std::cout;
using std::endl;

int main() {
    // 注意是 expr
    constexpr int value = get_value();
    cout << "Value: " << value << endl;
    return 0;
}
