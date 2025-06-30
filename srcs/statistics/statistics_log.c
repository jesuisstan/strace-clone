#include "statistics.h"
#include "syscall_strace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

void statistics_log(t_statistics *statistics) {
	if (!statistics || !statistics->entries) {
		return;
	}
	
	setlocale(LC_NUMERIC, "en_US.UTF-8");
	
	// Calculate total time and calls
	unsigned long long total_time = 0;
	unsigned long long total_calls = 0;
	unsigned long long total_errors = 0;
	
	for (size_t i = 0; i < statistics->count; i++) {
		t_statistics_entry *entry = &statistics->entries[i];
		total_time += entry->total_time;
		total_calls += entry->call_count;
		total_errors += entry->error_count;
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
	
	printf("%% time     seconds  usecs/call     calls    errors syscall\n");
	printf("------ ----------- ----------- --------- --------- ----------------\n");
	
	// Print entries
	for (size_t i = 0; i < statistics->count; i++) {
		t_statistics_entry *entry = &statistics->entries[i];
		
		if (entry->call_count == 0) continue;
		
		double time_percent = total_time > 0 ? ((double)entry->total_time / total_time) * 100.0 : 0.0;
		double seconds = (double)entry->total_time / 1000000.0;
		double usecs_per_call = (double)entry->total_time / (double)entry->call_count;
		
		const char *syscall_name = syscall_get_description(entry->syscall_no);
		if (!syscall_name) {
			syscall_name = "unknown";
		}
		
		char buffer[256];
		int len = snprintf(buffer, sizeof(buffer), "%5.2f    %f     %lu         %llu",
				 time_percent, seconds, (unsigned long)usecs_per_call, entry->call_count);
		
		for (int i = 0; i < len; ++i) {
			if (buffer[i] == '.') {
				buffer[i] = ',';
			}
		}
		
		printf("%s", buffer);
		if (entry->error_count > 0) {
			printf(" %9d", entry->error_count);
		} else {
			printf("          ");
		}
		printf(" %s\n", syscall_name);
	}
	
	// Print totals
	printf("------ ----------- ----------- --------- --------- ----------------\n");
	char total_buffer[256];
	double total_seconds = (double)total_time / 1000000.0;
	double avg_usecs_per_call = total_calls > 0 ? (double)total_time / total_calls : 0.0;
	
	int total_len = snprintf(total_buffer, sizeof(total_buffer), "100,00    %f     %lu         %llu",
				 total_seconds, (unsigned long)avg_usecs_per_call, total_calls);
	
	for (int i = 0; i < total_len; ++i) {
		if (total_buffer[i] == '.') {
			total_buffer[i] = ',';
		}
	}
	
	printf("%s", total_buffer);
	if (total_errors > 0) {
		printf(" %9llu", total_errors);
	} else {
		printf("          ");
	}
	printf(" total\n");
}