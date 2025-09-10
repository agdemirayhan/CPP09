#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <string>

BitcoinExchange::BitcoinExchange(const std::string &csvPath)
{
	loadCsv(csvPath);
	if (_rates.empty())
		throw std::runtime_error("Error: csv is empty or unreadable.");
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}
	try
	{
		// data.csv proje kökünde bekleniyor
		BitcoinExchange be("data.csv");

		std::ifstream in(argv[1]);
		if (!in)
		{
			std::cerr << "Error: could not open file." << std::endl;
			return (1);
		}
		std::string line;
		while (std::getline(in, line))
		{
			std::string date;
			double value;
			// if (!parseInputLine(line, date, value))
			// 	continue ;
			// try
			// {
			// 	double result = be.evaluate(date, value);
			// 	std::cout << date << " => " << value << " = " << result << std::endl;
			// }
			// catch (const std::exception &e)
			// {
			// 	std::cerr << e.what() << std::endl;
			// }
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}