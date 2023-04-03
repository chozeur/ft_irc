#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <stdlib.h>
# include <csignal>
#include <string>
#include <vector>

long	parsing_args(int ac, char **av, char **env);
void	sig_handler(int sig);
void 	removeAllOccurrences(std::string& str, const std::string& substr);
void 	cleanLine(std::string &str);
std::string remove_html_header(const std::string& html_string);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);
std::string	format(std::string str, unsigned long size);
#endif
