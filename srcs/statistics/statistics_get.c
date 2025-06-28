#include <statistics.h>
#include <stddef.h>

/**
 * @brief Get a specific statistics entry by syscall number and architecture type.
 *
 * @param statistics Pointer to the statistics structure
 * @param type Architecture type (x86_64 or i386)
 * @param syscall_no System call number
 * @return statistics_entry_t* Pointer to the found entry or NULL if not found
 */
statistics_entry_t *statistics_get(statistics_t *statistics, architecture_t type,
                                 unsigned long long syscall_no)
{
    (void)type; // Currently not used, but kept for future extensibility
    
    if (!statistics)
        return NULL;
    
    for (size_t i = 0; i < statistics->count; i++) {
        if (statistics->entries[i].syscall_no == syscall_no) {
            return &statistics->entries[i];
        }
    }
    
    return NULL;
}