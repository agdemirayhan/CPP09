#include "../include/BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string trim(const std::string &s) {
    std::string::size_type b = 0, e = s.size();
    while (b < e && (s[b] == ' ' || s[b] == '\t')) ++b;
    while (e > b && (s[e-1] == ' ' || s[e-1] == '\t')) --e;
    return s.substr(b, e - b);
}

int	main(int argc, char **argv)
{
	double	value;
	double	result;
	bool	firstLine;

	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}
	try
	{
		BitcoinExchange be("data/data.csv");
		std::ifstream in(argv[1]);
        if (!in) {
            std::cerr << "Error: could not open file." << std::endl;
            return 1;
        }

        std::string line;
        bool firstLine = true;

       while (std::getline(in, line)) {
    if (line.empty()) continue;

    // header'ı atla
    if (firstLine && line.find("date") != std::string::npos) {
        firstLine = false;
        continue;
    }
    firstLine = false;

    // "date | value" ayır
    std::size_t bar = line.find('|');
    if (bar == std::string::npos) {
        std::cerr << "Error: bad input => " << line << std::endl;
        continue;
    }

    std::string date = trim(line.substr(0, bar));
    std::string valueStr = trim(line.substr(bar + 1));

    // tarih geçerli mi?
    if (!BitcoinExchange::isValidDate(date)) {
        std::cerr << "Error: bad input => " << date << std::endl;
        continue;
    }

    // value'yu sayıya çevir
    std::istringstream iss(valueStr);
    double value;
    if (!(iss >> value) || (iss.peek() != EOF && !std::isspace(iss.peek()))) {
        std::cerr << "Error: bad value => " << valueStr << std::endl;
        continue;
    }
    if (value < 0) {
        std::cerr << "Error: not a positive number." << std::endl;
        continue;
    }
    if (value > 1000) {
        std::cerr << "Error: too large a number." << std::endl;
        continue;
    }

    // map'te tarih için <= en yakın anahtarı bul
    const std::map<std::string, float> &rates = be.getDataMap();
    if (rates.empty()) {
        std::cerr << "Error: csv is empty or unreadable." << std::endl;
        continue;
    }

    std::map<std::string, float>::const_iterator it = rates.lower_bound(date);

    if (it == rates.end()) {
        // girilen tarih map'teki en büyük tarihten büyükse son elemanı kullan
        --it;
    } else if (it->first != date) {
        // tam eşleşmediyse bir önceki (daha küçük) tarihi kullan
        if (it == rates.begin()) {
            std::cerr << "Error: no earlier rate for " << date << std::endl;
            continue;
        }
        --it;
    }

    double result = value * it->second;
    std::cout << date << " => " << value << " = " << result << std::endl;
}

		return (0);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
}
