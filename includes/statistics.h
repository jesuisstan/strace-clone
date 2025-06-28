#ifndef STATISTICS_H
# define STATISTICS_H

#include <sys/types.h>
#include <stdbool.h>
#include <types.h>

typedef struct s_statistics_entry {
	unsigned long long syscall_no;
	unsigned long long count;
	unsigned long long total_time;
} statistics_entry_t;

typedef struct s_statistics {
	statistics_entry_t *entries;
	size_t count;
	size_t capacity;
} statistics_t;

/**
 * @brief Initialize statistics structure
 *
 * @return statistics_t* the statistics structure or NULL if an error occurred
 */
statistics_t *statistics_init(void);

/**
 * @brief Destroy statistics structure
 *
 * @param statistics the statistics structure to destroy
 */
void statistics_destroy(statistics_t *statistics);

/**
 * @brief Add an entry to statistics
 *
 * @param statistics the statistics structure
 * @param type the architecture type
 * @param syscall_no the syscall number
 * @param time the time taken by the syscall
 * @return int 0 if the call succeeded, -1 otherwise
 */
int statistics_add_entry(statistics_t *statistics, architecture_t type,
						unsigned long long syscall_no, unsigned long long time);

/**
 * @brief Get statistics for a specific syscall
 *
 * @param statistics the statistics structure
 * @param type the architecture type
 * @param syscall_no the syscall number
 * @return statistics_entry_t* the statistics entry or NULL if not found
 */
statistics_entry_t *statistics_get(statistics_t *statistics, architecture_t type,
								 unsigned long long syscall_no);

/**
 * @brief Log statistics to stderr
 *
 * @param statistics the statistics structure
 */
void statistics_log(statistics_t *statistics);

#endif