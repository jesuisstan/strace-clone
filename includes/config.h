#ifndef CONFIG_H
# define CONFIG_H

#include <stdbool.h>

typedef struct s_config {
	const char *program_name;
	bool show_help;
	bool statistics;
	int string_limit;
	int argc;
	char **argv;
} t_config;

int config_parse(t_config *config, int argc, char **argv);
void config_print_help(const t_config *config);

#endif