#ifndef CONFIG_H
# define CONFIG_H

typedef struct {
	const char *program_name;
	int show_help;
	int show_statistics;
	int string_limit;
	int argc;
	char **argv;
} config_t;

int config_init(config_t *config, int argc, char **argv);
void config_print_help(const config_t *config);

#endif