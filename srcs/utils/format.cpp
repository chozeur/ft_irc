#include <iostream>


std::string	format(std::string str, unsigned long size){
	std::string ret = str;
	if (static_cast<unsigned long>(str.length()) > size){
		ret = str.substr(0, size);
		ret[size - 1] = '.';
		ret[size - 2] = '.';
		ret[size - 3] = '.';
	}
	else if (str.length() < size){
		for (unsigned long i = 0; i < static_cast<unsigned long>(size - str.length()); i++)
			ret += " ";
	}
	return (ret);
}
