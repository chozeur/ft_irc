#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <stdlib.h>
# include <csignal>

long	parsing_args(int ac, char **av, char **env);
void	sig_handler(int sig);

#endif
