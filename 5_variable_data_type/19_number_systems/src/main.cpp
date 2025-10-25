// Copyright rutura 2025
#include <iostream>

int main() {
    int number1 = {15};      // Decimal
    int number2 = {017};     // Octal (開頭是0)
    int number3 = {0xF};     // Hexadecimal
    int number4 = {0b1111};  // binary

    std::cout << "number1 : " << number1 << std::endl;
    std::cout << "number2 : " << number2 << std::endl;
    std::cout << "number3 : " << number3 << std::endl;
    std::cout << "number4 : " << number4 << std::endl;

    /*
    Output:
        number1 : 15
        number2 : 15
        number3 : 15
        number4 : 15
    */
    return 0;
}
