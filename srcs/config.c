#include <config.h>
#include <ft_strace_utils.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
	char letter;
	const char *name;
	const char *description;
} option_t;

static const option_t options[] = {
	{'h', "help", "Display this help and exit"},
	{'c', "count", "Count time, calls, and errors for each syscall and report a summary on program exit"},
	{'s', "string-limit", "Specify the maximum string length to print"},
	{'S', "statistics", "Show statistics for each syscall"},
	{0, NULL, NULL}
};

int config_init(config_t *config, int argc, char **argv)
{
	if (!config || argc < 1 || !argv)
		return -1;
	
	// Initialize default values
	config->program_name = argv[0];
	config->show_help = 0;
	config->show_statistics = 0;
	config->string_limit = 32;
	config->argc = argc - 1;
	config->argv = argv + 1;
	
	// Parse options
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				// Long option
				if (strcmp(argv[i], "--help") == 0) {
					config->show_help = 1;
				} else if (strcmp(argv[i], "--statistics") == 0) {
					config->show_statistics = 1;
				} else {
					fprintf(stderr, "%s: unrecognized option '--%s'\n", 
						   config->program_name, argv[i] + 2);
					return -1;
				}
			} else {
				// Short option
				for (int j = 1; argv[i][j]; j++) {
					switch (argv[i][j]) {
						case 'c':
							config->show_statistics = 1;
							break;
						case 'h':
							config->show_help = 1;
							break;
						case 's':
							if (i + 1 < argc && argv[i + 1][0] != '-') {
								config->string_limit = atoi(argv[i + 1]);
								i++;
							} else {
								fprintf(stderr, "%s: option requires an argument -- '%c'\n", 
									   config->program_name, argv[i][j]);
								return -1;
							}
							break;
						case 'S':
							config->show_statistics = 1;
							break;
						default:
							fprintf(stderr, "%s: invalid option -- '%c'\n", 
								   config->program_name, argv[i][j]);
							return -1;
					}
				}
			}
		} else {
			// Non-option argument, this is the program to trace
			// Keep all remaining arguments including the program name
			config->argc = argc - i;
			config->argv = argv + i;
			break;
		}
	}
	
	return 0;
}

void config_print_help(const config_t *config)
{
	printf("Usage: %s [option(s)] [file(s)]\n", config->program_name);
	printf("\nAvailable options:\n");
	
	for (int i = 0; options[i].letter; i++) {
		printf("  -%c, --%-16s %s\n", options[i].letter, options[i].name,
			   options[i].description);
	}

	printf("\nCreated by Stanislav Krivtsoff for educational purpose, Paris, 2025.\n");
}