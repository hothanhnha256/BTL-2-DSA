

#include "main.h"
#include "restaurant.cpp"
// #include <chrono>

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    string fileName = "test.txt";
    simulate(fileName);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Execution time: " << diff.count() << " s\n";
    return 0;
}
// int main(int argc, char* argv[]) {
//     string fileName = "test.txt";
//     simulate(fileName);

//     return 0;
// }
