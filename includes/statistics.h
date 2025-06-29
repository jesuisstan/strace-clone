#ifndef STATISTICS_H
# define STATISTICS_H

#include <sys/types.h>
#include <stdbool.h>
#include <types.h>

typedef struct s_statistics_entry {
	unsigned long long syscall_no;
	unsigned long long call_count;
	unsigned long long total_time;
	int error_count;
} t_statistics_entry;

typedef struct s_statistics {
	t_statistics_entry *entries;
	size_t count;
	size_t capacity;
} t_statistics;

/**
 * @brief Initialize statistics structure
 *
 * @return t_statistics* the statistics structure or NULL if an error occurred
 */
t_statistics *statistics_init(void);

/**
 * @brief Destroy statistics structure
 *
 * @param statistics the statistics structure to destroy
 */
void statistics_destroy(t_statistics *statistics);

/**
 * @brief Add an entry to statistics
 *
 * @param statistics the statistics structure
 * @param type the architecture type
 * @param syscall_no the syscall number
 * @param time the time taken by the syscall
 * @return int 0 if the call succeeded, -1 otherwise
 */
int statistics_add_entry(t_statistics *statistics, architecture_t type,
						unsigned long long syscall_no, unsigned long long time);

/**
 * @brief Get statistics for a specific syscall
 *
 * @param statistics the statistics structure
 * @param type the architecture type
 * @param syscall_no the syscall number
 * @return t_statistics_entry* the statistics entry or NULL if not found
 */
t_statistics_entry *statistics_get(t_statistics *statistics, architecture_t type,
								 unsigned long long syscall_no);

/**
 * @brief Log statistics to stderr
 *
 * @param statistics the statistics structure
 */
void statistics_log(t_statistics *statistics);

#endif