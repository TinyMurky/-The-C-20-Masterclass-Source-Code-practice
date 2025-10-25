// Copyright rutura 2025
#include <iostream>

int main() {
    // int
    // 4 bytes (32 bit) (不是4bit)
    std::cout << "sizeof(int): " << sizeof(int) << std::endl;

    // Braced initialization

    // init 垃圾值
    int garbage_int;
    std::cout << "garbage_int: " << garbage_int << std::endl;  // 會是隨便的值

    // init 零值
    int zero_int{};  // 0
    std::cout << "zero_int: " << zero_int << std::endl;

    int dog_int{10};
    int cat_int{15};

    int animal_int{dog_int + cat_int};  // 25

    std::cout << "animal_int: " << animal_int << std::endl;

    // 禁止 Narrowing Conversion, 比較安全
    // 下面這個會出現
    //
    // error: narrowing conversion of ‘2.8999999999999999e+0’
    // from ‘double’ to ‘int’ [-Wnarrowing]
    // int braced_narrow_int{2.9};

    // 會被 narrow 成 2, 不安全
    int func_narrow_int(2.9);
    std::cout << "func_narrow_int: " << func_narrow_int << std::endl;

    // 會被 narrow 成 2, 不安全
    int assign_narrow_int = 2.9;
    std::cout << "assign_narrow_int: " << assign_narrow_int << std::endl;

    // sizeof 也可以 直接用 variable
    // 4 bytes (32 bit) (不是4bit)
    std::cout << "sizeof(assign_narrow_int): "
              << sizeof(assign_narrow_int)
              << std::endl;
    return 0;
}
