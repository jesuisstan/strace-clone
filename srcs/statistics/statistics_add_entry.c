#include <statistics.h>
#include <stdlib.h>

/**
 * @brief Add an entry to the statistics.
 *
 * @param statistics the statistics
 * @param type the registers type
 * @param syscall_no the syscall number
 * @param time the time taken for the syscall
 * @return 0 on success, -1 on failure
 */
int statistics_add_entry(statistics_t *statistics, architecture_t type,
                        unsigned long long syscall_no, unsigned long long time)
{
	(void)type; // Currently not used, but kept for future extensibility
	
	if (!statistics)
		return -1;
	
	// Find existing entry
	for (size_t i = 0; i < statistics->count; i++) {
		if (statistics->entries[i].syscall_no == syscall_no) {
			statistics->entries[i].count++;
			statistics->entries[i].total_time += time;
			return 0;
		}
	}
	
	// Add new entry
	if (statistics->count >= statistics->capacity) {
		statistics->capacity *= 2;
		statistics_entry_t *new_entries = realloc(statistics->entries, 
												 statistics->capacity * sizeof(statistics_entry_t));
		if (!new_entries)
			return -1;
		statistics->entries = new_entries;
	}
	
	statistics->entries[statistics->count].syscall_no = syscall_no;
	statistics->entries[statistics->count].count = 1;
	statistics->entries[statistics->count].total_time = time;
	statistics->count++;
	
	return 0;
}