#include "../include/BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <string>

// static bool	parseInputLine(const std::string &line, std::string &date,
// 		double &value)
// {
// 	double	v;

// 	std::string l = line;
// 	if (l.empty())
// 		return (false);
// 	if (l.find("date") != std::string::npos
// 		&& l.find("value") != std::string::npos)
// 		return (false);
// 	std::string::size_type bar = l.find('|');
// 	if (bar == std::string::npos)
// 	{
// 		std::cerr << "Error: bad input => " << line << std::endl;
// 		return (false);
// 	}
// 	std::string left = l.substr(0, bar);
// 	std::string right = l.substr(bar + 1);
// 	while (!left.empty() && (left[0] == ' ' || left[0] == '\t'))
// 		left.erase(0, 1);
// 	while (!left.empty() && (left[left.size() - 1] == ' ' || left[left.size()
// 			- 1] == '\t'))
// 		left.erase(left.size() - 1);
// 	while (!right.empty() && (right[0] == ' ' || right[0] == '\t'))
// 		right.erase(0, 1);
// 	while (!right.empty() && (right[right.size() - 1] == ' '
// 			|| right[right.size() - 1] == '\t'))
// 		right.erase(right.size() - 1);
// 	date = left;
// 	if (!BitcoinExchange::parseDouble(right, v))
// 	{
// 		std::cerr << "Error: bad value => " << right << std::endl;
// 		return (false);
// 	}
// 	value = v;
// 	return (true);
// }

int	main(int argc, char **argv)
{
	double	value;
	double	result;

	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}
	try
	{
		BitcoinExchange be("data/data.csv");
		// std::ifstream in(be.getCsvPath().c_str());
		// std::cout << "CSV path: " << be.getCsvPath() << std::endl;
		// if (!in)
		// {
		// 	std::cerr << "Error: could not open file." << std::endl;
		// 	return (1);
		// }
		// std::cout << "CSV opened OK\n";
		// std::string line;
		// while (std::getline(in, line))
		// {
		// 	std::size_t delimiter = line.find(",");
		// 	if (delimiter != std::string::npos)
		// 		_dataMap[line.substr(0,
		// 				delimiter)] = std::stof(line.substr(delimiter + 1));
		// }
		return (0);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
}
