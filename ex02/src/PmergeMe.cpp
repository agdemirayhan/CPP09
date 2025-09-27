#include "PmergeMe.hpp"
#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>
#include <iostream>
#include <chrono>
#include <deque>

#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) { (void)other; }
PmergeMe& PmergeMe::operator=(const PmergeMe& rhs) { (void)rhs; return *this; }
PmergeMe::~PmergeMe() {}


namespace
{
std::vector<size_t> buildJacobsthalOrder(size_t m)
{
	size_t	from;
	size_t	to;
	bool	seen;

	std::vector<size_t> order;
	if (m == 0)
		return (order);
	// J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2)
	std::vector<size_t> J;
	J.push_back(0);
	J.push_back(1);
	while (J.back() < m)
	{
		J.push_back(J[J.size() - 1] + 2 * J[J.size() - 2]);
	}
	// The first element is always 1
	order.push_back(1);
	// Add blocks in reverse: (J(k-1)+1 .. min(J(k), m))
	for (size_t k = 2; k < J.size(); ++k)
	{
		from = J[k - 1] + 1;
		to = std::min(J[k], m);
		if (from > to)
			continue ;
		for (size_t j = to; j >= from; --j)
		{
			order.push_back(j);
			if (j == from)
				break ; // prevent size_t underflow
		}
	}
	// If still incomplete (rare), finish 1..m (reverse scan)
	if (order.size() < m)
	{
		for (size_t j = m; j >= 1; --j)
		{
			seen = false;
			for (size_t t = 0; t < order.size(); ++t)
			{
				if (order[t] == j)
				{
					seen = true;
					break ;
				}
			}
			if (!seen)
				order.push_back(j);
			if (j == 1)
				break ;
		}
	}
	return (order);
}
} 

// --- Step 1 (deque): pairing + straggler detection ---
namespace {
    void fj_pair_phase_deque(const std::deque<int>& in,
                             std::vector< std::pair<int,int> >& pairs,
                             bool& hasStraggler,
                             int&  stragglerValue)
    {
        pairs.clear();
        hasStraggler   = false;
        stragglerValue = 0;

        const size_t n = in.size();
        pairs.reserve(n / 2);

        size_t i = 0;
        for (; i + 1 < n; i += 2) {
            int a = in[i];
            int b = in[i + 1];
            if (a >= b) pairs.push_back(std::make_pair(a, b)); // (big, small)
            else        pairs.push_back(std::make_pair(b, a));
        }
        if (i < n) {
            hasStraggler   = true;
            stragglerValue = in[i];
        }
    }
}

// --- Step 3 (deque): insert small up to its big using binary search ---
namespace {
    inline void insertSmallBeforeBig(std::deque<int>& chain, int small, int big) {
        std::deque<int>::iterator limit = std::lower_bound(chain.begin(), chain.end(), big);
        std::deque<int>::iterator pos   = std::lower_bound(chain.begin(), limit, small);
        chain.insert(pos, small);
    }
}

namespace {
    void fj_build_backbone_deque(const std::vector< std::pair<int,int> >& pairs,
                                 std::deque<int>& backbone)
    {
        const size_t m = pairs.size();
        if (m == 0) { backbone.clear(); return; }
        if (m == 1) { backbone.clear(); backbone.push_back(pairs[0].first); return; }

        // 1) extract bigs
        std::deque<int> bigs;
        for (size_t k = 0; k < m; ++k) bigs.push_back(pairs[k].first);

        // 2) re-pair bigs
        std::vector< std::pair<int,int> > pairs2;
        bool hasStr = false; int stray = 0;
        fj_pair_phase_deque(bigs, pairs2, hasStr, stray);

        // 3) recursively build big-of-bigs backbone
        std::deque<int> bbChild;
        if (!pairs2.empty()) fj_build_backbone_deque(pairs2, bbChild);
        else bbChild.clear();

        // 4) insert pairs2's smalls in Jacobsthal order
        std::deque<int> chain = bbChild;
        std::vector<size_t> order = buildJacobsthalOrder(pairs2.size());
        for (size_t t = 0; t < order.size(); ++t) {
            size_t idx = order[t] - 1;
            int big   = pairs2[idx].first;
            int small = pairs2[idx].second;
            std::deque<int>::iterator limit = std::lower_bound(chain.begin(), chain.end(), big);
            std::deque<int>::iterator pos   = std::lower_bound(chain.begin(), limit, small);
            chain.insert(pos, small);
        }

        // 5) if there is a stray big, insert it
        if (hasStr) {
            std::deque<int>::iterator it = std::lower_bound(chain.begin(), chain.end(), stray);
            chain.insert(it, stray);
        }

        backbone.swap(chain);
    }
}

namespace
{
inline void	insertSmallBeforeBig(std::vector<int> &chain, int small, int big)
{
	std::vector<int>::iterator limit = std::lower_bound(chain.begin(),
			chain.end(), big);
	std::vector<int>::iterator pos = std::lower_bound(chain.begin(), limit,
			small);
	chain.insert(pos, small);
}
} 

namespace {
    void fj_pair_phase_vector(const std::vector<int>& in,
                              std::vector< std::pair<int,int> >& pairs,
                              bool& hasStraggler,
                              int&  stragglerValue)
    {
        pairs.clear();
        hasStraggler   = false;
        stragglerValue = 0;

        const size_t n = in.size();
        pairs.reserve(n / 2);

        size_t i = 0;
        for (; i + 1 < n; i += 2) {
            int a = in[i];
            int b = in[i + 1];
            if (a >= b) pairs.push_back(std::make_pair(a, b)); // (big, small)
            else        pairs.push_back(std::make_pair(b, a));
        }
        if (i < n) { // single leftover (straggler)
            hasStraggler   = true;
            stragglerValue = in[i];
        }
        // DEBUG
        // std::cout << "[DEBUG] Pairs (big,small): ";
        // for (size_t k = 0; k < pairs.size(); ++k) {
        //     std::cout << "(" << pairs[k].first << "," << pairs[k].second << ") ";
        // }
        // if (hasStraggler) std::cout << " stray=" << stragglerValue;
        // std::cout << std::endl;
    }
}

namespace {
    void fj_build_backbone_vector(const std::vector< std::pair<int,int> >& pairs,
                                  std::vector<int>& backbone)
    {
        const size_t m = pairs.size();
        if (m == 0) { backbone.clear(); return; }
        if (m == 1) { backbone.assign(1, pairs[0].first); return; }

        // 1) extract bigs
        std::vector<int> bigs; bigs.reserve(m);
        for (size_t k = 0; k < m; ++k) bigs.push_back(pairs[k].first);

        // 2) re-pair bigs
        std::vector< std::pair<int,int> > pairs2;
        bool hasStr = false; int stray = 0;
        fj_pair_phase_vector(bigs, pairs2, hasStr, stray);

        // 3) recursively build big-of-bigs backbone
        std::vector<int> bbChild;
        if (!pairs2.empty()) fj_build_backbone_vector(pairs2, bbChild);
        else bbChild.clear();

        // 4) insert pairs2's smalls in Jacobsthal order
        std::vector<int> chain = bbChild;
        std::vector<size_t> order = buildJacobsthalOrder(pairs2.size());
        for (size_t t = 0; t < order.size(); ++t) {
            size_t idx = order[t] - 1;
            int big   = pairs2[idx].first;
            int small = pairs2[idx].second;
            // insert small up to its big
            std::vector<int>::iterator limit = std::lower_bound(chain.begin(), chain.end(), big);
            std::vector<int>::iterator pos   = std::lower_bound(chain.begin(), limit, small);
            chain.insert(pos, small);
        }

        // 5) if there is a stray big, insert it
        if (hasStr) {
            std::vector<int>::iterator it = std::lower_bound(chain.begin(), chain.end(), stray);
            chain.insert(it, stray);
        }
        backbone.swap(chain);
    }
}

long PmergeMe::sortWithVector(const std::vector<int>& input, std::vector<int>& output) {
    auto t0 = std::chrono::steady_clock::now();
    // Step 1: pairing
    std::vector< std::pair<int,int> > pairs;
    bool hasStraggler = false;
    int  stragglerVal = 0;
    fj_pair_phase_vector(input, pairs, hasStraggler, stragglerVal);

    // Step 2: backbone (sorted bigs)
    std::vector<int> backbone;
    fj_build_backbone_vector(pairs, backbone);

    // Step 3: insert smalls into the backbone in Jacobsthal order
    std::vector<int> chain = backbone;
    const size_t m = pairs.size();
    std::vector<size_t> order = buildJacobsthalOrder(m);
    for (size_t t = 0; t < order.size(); ++t) {
        size_t idx = order[t] - 1;
        int big   = pairs[idx].first;
        int small = pairs[idx].second;
        insertSmallBeforeBig(chain, small, big);
    }

    // Step 4: if there is a straggler, insert it
    if (hasStraggler) {
        std::vector<int>::iterator pos =
            std::lower_bound(chain.begin(), chain.end(), stragglerVal);
        chain.insert(pos, stragglerVal);
    }

    // return the result
    output.swap(chain);
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}

long PmergeMe::sortWithDeque(const std::deque<int>& input, std::deque<int>& output) {
    auto t0 = std::chrono::steady_clock::now();
    // Step 1: pairing
    std::vector< std::pair<int,int> > pairs;
    bool hasStraggler = false;
    int  stragglerVal = 0;
    fj_pair_phase_deque(input, pairs, hasStraggler, stragglerVal);

    // Step 2: backbone (sorted bigs)
    std::deque<int> backbone;
    fj_build_backbone_deque(pairs, backbone);

    // Step 3: insert smalls into the backbone in Jacobsthal order
    std::deque<int> chain = backbone;
    const size_t m = pairs.size();
    std::vector<size_t> order = buildJacobsthalOrder(m);
    for (size_t t = 0; t < order.size(); ++t) {
        size_t idx = order[t] - 1;     // 1-based -> 0-based
        int big   = pairs[idx].first;
        int small = pairs[idx].second;
        insertSmallBeforeBig(chain, small, big); // deque overload
    }

    // Step 4: if there is a straggler, insert it
    if (hasStraggler) {
        std::deque<int>::iterator pos =
            std::lower_bound(chain.begin(), chain.end(), stragglerVal);
        chain.insert(pos, stragglerVal);
    }

    output.swap(chain);
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}
