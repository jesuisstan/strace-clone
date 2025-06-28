#include <statistics.h>
#include <stdio.h>
#include <stdlib.h>

void statistics_log(statistics_t *statistics)
{
	if (!statistics) {
		return;
	}
	
	printf("\n%15s %10s %15s %10s\n", "syscall", "calls", "total time", "errors");
	printf("%15s %10s %15s %10s\n", "------", "-----", "-----------", "------");
	
	for (size_t i = 0; i < statistics->count; i++) {
		statistics_entry_t *entry = &statistics->entries[i];
		printf("%15llu %10llu %15llu %10llu\n", 
			   entry->syscall_no, 
			   entry->count, 
			   entry->total_time, 
			   (unsigned long long)0); // errors not tracked in simplified version
	}
}