#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
public:
    
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other); 
    ~BitcoinExchange();

    
    explicit BitcoinExchange(const std::string& csvPath);
    const std::string& getCsvPath() const;
    const std::map<std::string, float>& getDataMap() const;

    static bool isValidDate(const std::string& d);

private:
    static bool isLeap(int y);
    void loadCsv(const std::string& csvPath);

    std::map<std::string, float> _dataMap;
    std::string _csvPath;
};

#endif
