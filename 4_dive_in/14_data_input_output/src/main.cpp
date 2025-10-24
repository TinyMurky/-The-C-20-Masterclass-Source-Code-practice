// Copyright 2025 rutura
#include <iostream>
#include <string>

// 給 numeric_limits
#include <limits>

void discard_line() {
    // 從輸入緩衝區中丟棄（忽略）直到遇到 \n 為止的所有字元，最多丟掉 std::numeric_limits<std::streamsize>::max() 個。
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void flush_input_after_failure() {
    if (!std::cin) {       // 讀取失敗 (failbit/badbit)
        std::cin.clear();  // 先清錯誤旗標
    }

    discard_line();
}

int main() {
    std::cout << "This is cout" << std::endl;

    int age = {21};

    std::cout << "The age is: " << age << std::endl;

    std::clog << "This is clog" << std::endl;

    std::cerr << "THis is cerr" << std::endl;

    // cin
    std::cout << "=====" << "cint practice" << "=====" << std::endl;

    std::string my_name;
    int my_age;

    // 下面這樣子 分開來拿, 但是不能輸入有空格的會出錯 ex:
    std::cout << "Please enter your name:" << std::endl;

    std::cin >> my_name;

    std::cout << "Please enter your age:" << std::endl;

    // 這邊可以試試看 打文字會是 0
    std::cin >> my_age;

    if (!std::cin) {
        flush_input_after_failure();
        std::cerr << "Age must be a number.\n";
        return 1;
    }

    std::cout << "Your name is: " << my_name
              << ", your age is: " << my_age
              << std::endl;

    // 下面一次取兩個

    std::cout << "Please enter your name and age:" << std::endl;

    // 可以用同一組 variable
    std::cin >> my_name >> my_age;

    if (!std::cin) {
        flush_input_after_failure();
        std::cerr << "Age must be a number.\n";
        return 1;
    }

    std::cout << "Your name is: " << my_name
              << ", your age is: " << my_age
              << std::endl;

    // 用get line 來獲得完整的名稱
    std::cout << "Please enter your name and age:" << std::endl;
    // 這裡不要用舊的

    // 只有在前一個步驟是用 >> 才需要清一次殘留換行
    // 這裡前一步是 >>，所以清一次就好
    discard_line();
    std::getline(std::cin, my_name);
    std::cin >> my_age;
    if (!std::cin) {
        // 使用者真的輸入了非數字才需要清錯 + 丟棄
        flush_input_after_failure();
        std::cerr << "Age must be a number.\n";
        return 1;
    }

    std::cout << "Your name is: " << my_name
              << ", your age is: " << my_age
              << std::endl;
    return 0;
}
