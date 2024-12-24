#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <omp.h>

std::vector<int> generateRandomList(size_t length, int minValue, int maxValue) {
    std::vector<int> randomList(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minValue, maxValue);

    #pragma omp parallel for
    for (size_t i = 0; i < length; ++i) {
        randomList[i] = dis(gen);
    }

    return randomList;
}

void saveToFile(const std::vector<int>& list, const std::string& filename) {
    std::ofstream file(filename);
    for (int num : list) {
        file << num << " ";
    }
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "### Example: " << argv[0] << " <lenght> <min_val> <max_val> <file_name>" << std::endl;
        return 1;
    }

    size_t length = std::stoul(argv[1]);
    int minValue = std::stoi(argv[2]);
    int maxValue = std::stoi(argv[3]);
    std::string filename = argv[4];

    std::vector<int> randomList = generateRandomList(length, minValue, maxValue);

    saveToFile(randomList, filename);

    std::cout << "### New vector in " << filename << std::endl;

    return 0;
}