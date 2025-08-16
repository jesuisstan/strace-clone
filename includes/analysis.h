#ifndef ANALYSIS_H
# define ANALYSIS_H

#include <types.h>
#include <statistics.h>
#include <sys/types.h>

#define ROUTINE_ERROR -1

/**
 * @brief Analysis routine of the tracer
 *
 * @param pid the pid of the tracee
 * @param statistics optional statistics structure for collecting data
 * @param quiet_mode if true, suppress exit messages for statistics mode
 * @return status code of tracee or ROUTINE_ERROR if an error occurred
 */
int analysis_routine(pid_t pid, t_statistics *statistics, bool quiet_mode);

#endif