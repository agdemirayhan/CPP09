#include "PmergeMe.hpp"

// Şimdilik sadece "output = input" yapıp 0 süre döndürüyoruz.
// Bir sonraki adımda gerçek algoritmayı ekleyeceğiz.

long PmergeMe::sortWithVector(const std::vector<int>& input, std::vector<int>& output) {
    output = input;
    return 0;
}

long PmergeMe::sortWithDeque(const std::deque<int>& input, std::deque<int>& output) {
    output = input;
    return 0;
}
