#include "statistics.h"
#include <stdlib.h>

/**
 * @brief Destroy the statistics.
 *
 * @param statistics the statistics to destroy
 */
void statistics_destroy(t_statistics *statistics)
{
	if (statistics) {
		if (statistics->entries) {
			free(statistics->entries);
		}
		free(statistics);
	}
}