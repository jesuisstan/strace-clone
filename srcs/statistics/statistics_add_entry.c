#include "statistics.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Add an entry to the statistics.
 *
 * @param statistics the statistics
 * @param type the registers type
 * @param syscall_no the syscall number
 * @param time the time taken for the syscall
 * @return 0 on success, -1 on failure
 */
int statistics_add_entry(t_statistics *statistics, architecture_t type,
                        unsigned long long syscall_no, unsigned long long time)
{
	(void)type;
	if (!statistics || !statistics->entries) {
		return -1;
	}
	
	// Find existing entry for this syscall
	for (size_t i = 0; i < statistics->count; i++) {
		if (statistics->entries[i].syscall_no == syscall_no) {
			statistics->entries[i].call_count++;
			statistics->entries[i].total_time += time;
			return 0;
	}
	}
	
	// Add new entry
	if (statistics->count >= statistics->capacity) {
		size_t new_capacity = statistics->capacity * 2;
		t_statistics_entry *new_entries = realloc(statistics->entries, 
												 new_capacity * sizeof(t_statistics_entry));
		if (!new_entries) {
			return -1;
		}
		statistics->entries = new_entries;
		statistics->capacity = new_capacity;
	}
	
	statistics->entries[statistics->count].syscall_no = syscall_no;
	statistics->entries[statistics->count].call_count = 1;
	statistics->entries[statistics->count].total_time = time;
	statistics->entries[statistics->count].error_count = 0;
	statistics->count++;
	
	return 0;
}