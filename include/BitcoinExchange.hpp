#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <stdexcept>
# include <string>

class BitcoinExchange
{
  public:
	// data.csv yolunu vererek kur
	explicit BitcoinExchange(const std::string &csvPath);

	// "YYYY-MM-DD" formatındaki bir tarihteki fiyata göre value çarpar,
	// tam eşleşme yoksa en yakın (<= tarih) değeri kullanır.
	// Geçersiz tarih, negatif değer, çok büyük değer vs. için istisna atar.
	double evaluate(const std::string &date, double value) const;

	// Yardımcı: Tarih uygun mu?
	static bool isValidDate(const std::string &date);
	// Yardımcı: sayısal double parse (C++98 güvenli)
	static bool parseDouble(const std::string &s, double &out);

  private:
	std::map<std::string, double> _rates; // date -> rate

	void loadCsv(const std::string &path);
	static bool isLeap(int y);
	static bool splitKV(const std::string &line, char sep, std::string &left,
		std::string &right);
	static std::string trim(const std::string &s);
};

#endif
