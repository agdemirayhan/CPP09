#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <climits>

static bool parseArgs(int argc, char** argv, std::vector<int>& v, std::deque<int>& d) {
    for (int i = 1; i < argc; ++i) {
        std::string s(argv[i]);
        if (s.empty()) return false;
        for (size_t k = 0; k < s.size(); ++k) {
            if (s[k] < '0' || s[k] > '9') return false;
        }
        unsigned long ul = 0;
        for (size_t k = 0; k < s.size(); ++k) {
            ul = ul * 10 + (s[k] - '0');
            if (ul > static_cast<unsigned long>(INT_MAX)) return false;
        }
        if (ul == 0) return false; 
        int val = static_cast<int>(ul);
        v.push_back(val);
        d.push_back(val);
    }
    return !v.empty();
}

static void printSeq(const char* prefix, const std::vector<int>& v, size_t maxPrint = 30) {
    std::cout << prefix;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i >= maxPrint) { std::cout << " [...]"; break; }
        std::cout << (i ? " " : "") << v[i];
    }
    std::cout << std::endl;
}

// DEBUG
// static void printSeqDeque(const char* prefix, const std::deque<int>& d, size_t maxPrint = 30) {
//     std::cout << prefix;
//     for (size_t i = 0; i < d.size(); ++i) {
//         if (i >= maxPrint) { std::cout << " [...]"; break; }
//         std::cout << (i ? " " : "") << d[i];
//     }
//     std::cout << std::endl;
// }


int main(int argc, char** argv) {
    if (argc < 2) { std::cerr << "Error" << std::endl; return 1; }

    std::vector<int> inputVec;
    std::deque<int>  inputDeq;

    if (!parseArgs(argc, argv, inputVec, inputDeq)) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    printSeq("Before \t: ", inputVec);
    // DEBUG
    //printSeqDeque("Before (deque)\t: ", inputDeq);

    std::vector<int> sortedVec;
    std::deque<int>  sortedDeq;

    long usVec = PmergeMe::sortWithVector(inputVec, sortedVec);
    long usDeq = PmergeMe::sortWithDeque(inputDeq, sortedDeq);

    printSeq("After \t: ", sortedVec);
    // DEBUG
    //printSeqDeque("After (deque)\t: ", sortedDeq);

    std::cout << "Time to process a range of " << inputVec.size()
              << " elements with std::vector : " << usVec << " us" << std::endl;
    std::cout << "Time to process a range of " << inputDeq.size()
              << " elements with std::deque  : " << usDeq << " us" << std::endl;
    return 0;
}
