#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN {
public:
    // Hata durumunda exception fırlatır
    static long long evaluate(const std::string& expr);

private:
    RPN(); // nesne üretme yok
    RPN(const RPN&);
    RPN& operator=(const RPN&);
    ~RPN();
};

#endif // RPN_HPP
