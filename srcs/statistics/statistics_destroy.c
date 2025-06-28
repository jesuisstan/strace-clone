#include <statistics.h>
#include <stdlib.h>

/**
 * @brief Destroy the statistics.
 *
 * @param statistics the statistics to destroy
 */
void statistics_destroy(statistics_t *statistics)
{
	if (!statistics)
		return;
	
	free(statistics->entries);
	free(statistics);
}