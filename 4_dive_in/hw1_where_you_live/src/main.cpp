// Copyright Tiny_Murky 2025
#include <iostream>
#include <string>

int main() {
    std::string living_place;

    std::cout << "Where do you live?" << std::endl;

    std::cin >> living_place;

    std::cout << "I've heard great things about "
              << living_place
              << ". I'd like to go sometime."
              << std::endl;

    return 0;
}
