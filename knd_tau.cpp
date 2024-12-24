#include <iostream>
#include <vector>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <string>


std::vector<int> readFromFile(const std::string& filename) {
    std::vector<int> numbers;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "### cant open the file " << filename << std::endl;
        return numbers;
    }

    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        int number;
        while (iss >> number) {
            numbers.push_back(number);
        }
    }

    file.close();
    return numbers;
}

double kendall_tau_sequential(const std::vector<int>& list1, const std::vector<int>& list2) {
    if (list1.size() != list2.size()) {
        throw std::invalid_argument("The lists must be of equal length");
    }

    size_t n = list1.size();
    long long concordant = 0;
    long long discordant = 0;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            int a = (list1[i] - list1[j]) * (list2[i] - list2[j]);
            if (a > 0) {
                concordant += 1;
            } 
            else if (a < 0) {
                discordant += 1;
            }
        }
    }

    return static_cast<double>(concordant - discordant) / (concordant + discordant);
}

double kendall_tau_parallel(const std::vector<int>& list1, const std::vector<int>& list2) {
    if (list1.size() != list2.size()) {
        throw std::invalid_argument("The lists must be of equal length");
    }

    size_t n = list1.size();
    long long concordant = 0;
    long long discordant = 0;

    #pragma omp parallel for reduction(+:concordant, discordant)
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            int a = (list1[i] - list1[j]) * (list2[i] - list2[j]);
            if (a > 0) {
                concordant += 1;
            } 
            else if (a < 0) {
                discordant += 1;
            }
        }
    }

    return static_cast<double>(concordant - discordant) / (concordant + discordant);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "### Example: " << argv[0] << " <vector_file_name_1> <vector_file_name_1> <parallel/sequential>" << std::endl;
        return 1;
    }

    std::vector<int> list1 = readFromFile(argv[1]);
    std::vector<int> list2 = readFromFile(argv[2]);
    std::string cond = argv[3];

    try {
        double result = 0;
        if (cond == "parallel") {
            result = kendall_tau_parallel(list1, list2);
        } 
        else if (cond == "sequential") {
            result = kendall_tau_sequential(list1, list2);
        }
        else {
            std::cout << "### error mode" << std::endl;
            return 1;
        }
        std::cout << "### " << cond <<" tau: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
