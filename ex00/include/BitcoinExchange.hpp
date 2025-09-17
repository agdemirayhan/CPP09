#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <stdexcept>
# include <string>

class BitcoinExchange
{
  public:
	explicit BitcoinExchange(const std::string &csvPath);
	const std::string &getCsvPath() const;

	// double evaluate(const std::string &date, double value) const;

	// static bool isValidDate(const std::string &date);

	// static bool parseDouble(const std::string &s, double &out);
  private:
	std::map<std::string, float> _dataMap;
	void loadCsv(const std::string &csvPath);
	std::string _csvPath; // constructor ile set edilen yol

	// std::map<std::string, double> _rates;
	// void loadCsv(const std::string &path);
	// static bool isLeap(int y);
	// static bool splitKV(const std::string &line, char sep, std::string &left,
	// 	std::string &right);
	// static std::string trim(const std::string &s);
};

#endif
