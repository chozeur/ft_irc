#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <stdlib.h>
# include <csignal>
#include <string>

long	parsing_args(int ac, char **av, char **env);
void	sig_handler(int sig);
void 	removeAllOccurrences(std::string& str, const std::string& substr);
void 	cleanLine(std::string &str); 

#endif
