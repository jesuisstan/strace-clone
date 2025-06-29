#include "../param_log.h"

/**
 * @brief Log a value from a list of options
 *
 * @param value the value to log
 * @param options the list of options
 * @param options_size the size of the list of options
 * @param default_name the default name to use if the value is not found
 * @return int the number of bytes written
 */
int option_log(uint64_t value, const flag_str_t *options, size_t options_size,
			   const char *default_name)
{
	for (size_t i = 0; i < options_size; i++)
	{
		if (options[i].flag == value)
			return dprintf(STDERR_FILENO, "%s", options[i].str);
	}
	if (!default_name)
		return dprintf(STDERR_FILENO, "%#lx", value);
	return dprintf(STDERR_FILENO, "%#lx /* %s */", value, default_name);
}