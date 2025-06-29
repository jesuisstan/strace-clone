#include "statistics.h"
#include <stdlib.h>

/**
 * @brief Initialize the statistics.
 *
 * @return statistics_t* the initialized statistics or NULL if failed
 */
t_statistics *statistics_init(void)
{
	t_statistics *statistics = malloc(sizeof(t_statistics));
	if (!statistics)
		return NULL;
	
	statistics->capacity = 100;
	statistics->count = 0;
	statistics->entries = malloc(statistics->capacity * sizeof(t_statistics_entry));
	if (!statistics->entries) {
		free(statistics);
		return NULL;
	}
	
	return statistics;
}
