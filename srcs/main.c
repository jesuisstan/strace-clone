#include <config.h>
#include <execution.h>
#include <analysis.h>
#include <statistics.h>
#include <stdlib.h>
#include <stdio.h>

// Global variable to track statistics mode
bool g_statistics_mode = false;

int main(int argc, char **argv)
{
	t_config config = {0};
	t_statistics *statistics = NULL;

	// Parse command line arguments
	if (config_parse(&config, argc, argv) != 0) {
		return 1;
	}
	
	if (config.show_help) {
		config_print_help(&config);
		return 0;
	}
	
	if (config.statistics) {
		g_statistics_mode = true;
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
