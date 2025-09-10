#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>

class BitcoinExchange
{
  public:
	// data.csv yolunu vererek kur
	explicit BitcoinExchange(const std::string &csvPath);

  private:
	std::map<std::string, double> _rates; // date -> rate

	void loadCsv(const std::string &path);
};

#endif
