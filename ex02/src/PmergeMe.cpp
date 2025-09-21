#include "PmergeMe.hpp"
#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>
#include <iostream>

static void debugPrint(const char* label, const std::vector<int>& v) {
    std::cout << label << ": ";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << " ";
        std::cout << v[i];
    }
    std::cout << std::endl;
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
} // namespace

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
        if (i < n) { // tek artan
            hasStraggler   = true;
            stragglerValue = in[i];
        }
        std::cout << "[DEBUG] Pairs (big,small): ";
        for (size_t k = 0; k < pairs.size(); ++k) {
            std::cout << "(" << pairs[k].first << "," << pairs[k].second << ") ";
        }
        if (hasStraggler) std::cout << " stray=" << stragglerValue;
        std::cout << std::endl;
    }
}

namespace {
    // Girdi: (big,small) listesi; Çıktı: big’lerin sıralı hâli (backbone)
    void fj_build_backbone_vector(const std::vector< std::pair<int,int> >& pairs,
                                  std::vector<int>& backbone)
    {
        const size_t m = pairs.size();
        if (m == 0) { backbone.clear(); return; }
        if (m == 1) { backbone.assign(1, pairs[0].first); return; }

        // 1) big’leri çek
        std::vector<int> bigs;
        bigs.reserve(m);
        for (size_t k = 0; k < m; ++k) bigs.push_back(pairs[k].first);

        // 2) bigs’i Ford–Johnson ile sırala (özyinelemeli)
        //    Not: Henüz small eklemiyoruz; “saf big” sıralaması istiyoruz.
        //    Bunun için aynı “pairleme→bigs→…” sürecini bigs üzerinde uygularız.
        //    bigs tek başına bir dizi olduğu için önce onu çiftleyip yeni pairs oluşturacağız.
        std::vector< std::pair<int,int> > pairs2;
        bool hasStr; int stray;
        fj_pair_phase_vector(bigs, pairs2, hasStr, stray);

        // Backbone’u derinlemesine üret
        std::vector<int> bbChild;
        if (!pairs2.empty())
            fj_build_backbone_vector(pairs2, bbChild);
        else
            bbChild.clear();

        // bbChild şu an bigs’in backbone’u (sıralı büyükler).
        // Eğer stray (tek artan) varsa, sadece big’lerden oluşan zincire
        // ikili arama ile eklemek gerekir; ama backbone saf “bigs” sırasını temsil ettiğinden,
        // şimdilik basitçe uygun yere yerleştirelim.
        std::vector<int> chain = bbChild;
        if (hasStr) {
            // basit lineer yerleştirme (n küçükken sorun değil; performans kritik değil)
            std::vector<int>::iterator it = chain.begin();
            while (it != chain.end() && *it < stray) ++it;
            chain.insert(it, stray);
        }

        backbone.swap(chain);
        debugPrint("[DEBUG] Backbone built", backbone);

    }
}

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
	// İlk eleman her zaman 1
	order.push_back(1);
	// Blokları tersten ekle: (J(k-1)+1 .. min(J(k), m))
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
				break ; // size_t underflow önle
		}
	}
	// Eksik kaldıysa (nadir) 1..m’yi tamamla (ters tarama ile)
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
} // namespace

long PmergeMe::sortWithVector(const std::vector<int>& input, std::vector<int>& output) {
    // Adım 1: pairleme
    std::vector< std::pair<int,int> > pairs;
    bool hasStraggler = false;
    int  stragglerVal = 0;
    fj_pair_phase_vector(input, pairs, hasStraggler, stragglerVal);

    // Adım 2: backbone (sıralı big’ler)
    std::vector<int> backbone;
    fj_build_backbone_vector(pairs, backbone);

    // Adım 3: small’ları Jacobsthal sırasıyla backbone’a ekle (kendi big’ini aşmadan)
    std::vector<int> chain = backbone;                 // üzerine ekleyeceğimiz ana zincir
    const size_t m = pairs.size();
    std::vector<size_t> order = buildJacobsthalOrder(m);
    std::cout << "[DEBUG] Jacobsthal order: ";
    for (size_t k = 0; k < order.size(); ++k)
        std::cout << order[k] << " ";
    std::cout << std::endl;

    for (size_t t = 0; t < order.size(); ++t) {
        size_t idx = order[t] - 1;                    // 1-based → 0-based
        int big   = pairs[idx].first;
        int small = pairs[idx].second;
        insertSmallBeforeBig(chain, small, big);
        std::cout << "[DEBUG] Insert small=" << small 
          << " before big=" << big << std::endl;
        debugPrint("   chain now", chain);
    }
    


    // !!! DİKKAT: Straggler henüz eklenmedi. Bir sonraki adımda ekleyeceğiz.
    // Şimdilik kullanıcı çıktısını bozmayalım:
    output = input; // geçici
    return 0;       // süre ölçümü sonraki aşamada eklenecek
}


long PmergeMe::sortWithDeque(const std::deque<int> &input,
	std::deque<int> &output)
{
	output = input;
	return (0);
}
