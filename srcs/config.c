#include "config.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int config_parse(t_config *config, int argc, char **argv) {
	if (!config || argc < 1) {
		return -1;
	}
	
	// Initialize config
	config->program_name = argv[0];
	config->show_help = false;
	config->statistics = false;
	config->string_limit = 32;
	config->argc = argc;
	config->argv = argv;
	
	// Parse arguments
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			config->show_help = true;
		} else if (strcmp(argv[i], "-c") == 0) {
			config->statistics = true;
		} else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
			config->string_limit = atoi(argv[i + 1]);
			i++; // Skip next argument
		}
	}
	
	return 0;
}

void config_print_help(const t_config *config) {
	(void)config; // Not used in this implementation
	
	printf("Usage: %s [OPTIONS] COMMAND [ARGS...]\n", config->program_name);
	printf("\nOptions:\n");
	printf("  -h, --help     Show this help message\n");
	printf("  -c             Count time, calls, and errors for each syscall\n");
	printf("  -s N           Limit string length to N characters (default: 32)\n");
	printf("\nExamples:\n");
	printf("  %s ls -la\n", config->program_name);
	printf("  %s -c ls -la\n", config->program_name);
	printf("  %s -s 10 echo hello\n", config->program_name);
}