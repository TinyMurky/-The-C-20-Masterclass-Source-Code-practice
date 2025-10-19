// Copyright 2025 Tiny_Murky
#include "src/number/number.hpp"

#include <iostream>
#include <string>

// 這樣也可以
// int number::sum(int a, int b) {
//     return 0;
// }

namespace number {
int sum(int a, int b) {
    return a + b;
}

// & 代表傳參, 不複製 (但外面不用丟 pointer, 直接丟本體)
// const 代表 println不能動 參數指向的 string
void println(const std::string& arg, int n) {
    std::cout << arg << ": ";

    // 注意 std::endl 會強制flush
    // 用 \n 會比較快
    std::cout << n << std::endl;
    return;
}
}  // namespace number
