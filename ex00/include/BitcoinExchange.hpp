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
	const std::map<std::string, float> &getDataMap() const
	{
		return (_dataMap);
	}
	static bool isValidDate(const std::string &d);

  private:
	static bool isLeap(int y);
	std::map<std::string, float> _dataMap;
	void loadCsv(const std::string &csvPath);
	std::string _csvPath; 
};

#endif
