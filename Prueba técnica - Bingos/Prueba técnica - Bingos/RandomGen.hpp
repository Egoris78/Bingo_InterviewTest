#pragma once
#include <iostream>
#include <random>

class RandomGenerator {
private:
    std::mt19937 _gen; 
    std::uniform_int_distribution<> _distrib;

public:
    RandomGenerator(int min, int max)
        : _gen(std::random_device{}()), _distrib(min, max) {}

    int generate() {
        return _distrib(_gen);
    }
};