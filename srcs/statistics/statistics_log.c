#include "statistics.h"
#include "syscall_strace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void statistics_log(t_statistics *statistics) {
	if (!statistics || !statistics->entries) {
		return;
	}
	
	printf("\n%c %-20s %10s %10s %10s %10s\n", '%', "time", "seconds", "usecs/call", "calls", "errors");
	printf("%c %-20s %10s %10s %10s %10s\n", '%', "syscall", "", "", "", "");
	printf("%c %-20s %10s %10s %10s %10s\n", '%', "------", "-------", "----------", "-----", "------");
	
	// Calculate total time and calls
	unsigned long long total_time = 0;
	unsigned long long total_calls = 0;
	
	for (size_t i = 0; i < statistics->count; i++) {
		t_statistics_entry *entry = &statistics->entries[i];
		total_time += entry->total_time;
		total_calls += entry->call_count;
}

	// Sort entries by time (descending)
	for (size_t i = 0; i < statistics->count - 1; i++) {
		for (size_t j = i + 1; j < statistics->count; j++) {
			if (statistics->entries[i].total_time < statistics->entries[j].total_time) {
				t_statistics_entry temp = statistics->entries[i];
				statistics->entries[i] = statistics->entries[j];
				statistics->entries[j] = temp;
			}
		}
	}
	
	// Print entries
	for (size_t i = 0; i < statistics->count; i++) {
		t_statistics_entry *entry = &statistics->entries[i];
		
		if (entry->call_count == 0) continue;
		
		double seconds = (double)entry->total_time / 1000000.0;
		double usecs_per_call = (double)entry->total_time / (double)entry->call_count;
		
		// Get syscall name
		const char *syscall_name = syscall_get_description(entry->syscall_no);
		if (!syscall_name) {
			syscall_name = "unknown";
		}
		
		printf("%-20s %10.6f %10.2f %10llu %10d\n", 
			syscall_name, seconds, usecs_per_call, entry->call_count, entry->error_count);
}

	// Print totals
	printf("%c %-20s %10s %10s %10s %10s\n", '%', "------", "-------", "----------", "-----", "------");
	printf("%-20s %10.6f %10.2f %10llu %10d\n", 
		"total", (double)total_time / 1000000.0, 
		total_calls > 0 ? (double)total_time / (double)total_calls : 0.0,
		total_calls, 0);
}