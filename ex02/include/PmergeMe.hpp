#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>

class PmergeMe {
public:
    PmergeMe();                              
    PmergeMe(const PmergeMe& other);         
    PmergeMe& operator=(const PmergeMe& rhs);
    ~PmergeMe();                             

    static long sortWithVector(const std::vector<int>& input, std::vector<int>& output);
    static long sortWithDeque (const std::deque<int>&  input, std::deque<int>&  output);
};

# ifndef DEBUG
#  define DEBUG 0
# endif

#endif
