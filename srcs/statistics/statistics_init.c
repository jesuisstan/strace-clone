#include <statistics.h>
#include <stdlib.h>

/**
 * @brief Initialize the statistics.
 *
 * @return statistics_t* the initialized statistics or NULL if failed
 */
statistics_t *statistics_init(void)
{
	statistics_t *statistics = malloc(sizeof(statistics_t));
	if (!statistics)
		return NULL;
	
	statistics->entries = malloc(100 * sizeof(statistics_entry_t));
	if (!statistics->entries) {
		free(statistics);
		return NULL;
	}
	
	statistics->count = 0;
	statistics->capacity = 100;
	
	return statistics;
}
