#include <config.h>
#include <execution.h>
#include <analysis.h>
#include <statistics.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	config_t config;
	struct s_statistics *statistics = NULL;
	
	if (config_init(&config, argc, argv) != 0)
		return 1;
	
	if (config.show_help) {
		config_print_help(&config);
		return 0;
	}
	
	if (config.show_statistics) {
		statistics = statistics_init();
		if (!statistics) {
			fprintf(stderr, "Failed to initialize statistics\n");
			return 1;
		}
	}
	
	if (exec_program(&config, statistics) != 0) {
		if (statistics)
			statistics_destroy(statistics);
		return 1;
	}
	
	if (statistics) {
		statistics_log(statistics);
		statistics_destroy(statistics);
	}
	
	return 0;
}
