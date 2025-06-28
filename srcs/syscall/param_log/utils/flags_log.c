#include "../param_log.h"
#include <ft_printf.h>

/**
 * @brief Log a set of flags
 *
 * @param flags the flags to log
 * @param flag_strs the flags strings
 * @param flag_strs_size the size of the flag strings array
 * @return int the number of bytes written
 */
int flags_log(uint64_t flags, const flag_str_t *flag_strs, size_t flag_strs_size)
{
	if (flags == 0)
		return ft_dprintf(STDERR_FILENO, "0");
	int size_written = 0;
	bool_t first = true;
	for (size_t i = 0; i < flag_strs_size; i++)
	{
		if (flags & flag_strs[i].flag)
		{
			if (!first)
				size_written += ft_dprintf(STDERR_FILENO, "|");
			size_written += ft_dprintf(STDERR_FILENO, "%s", flag_strs[i].str);
			first = false;
			flags &= ~flag_strs[i].flag;
		}
	}
	if (flags != 0)
	{
		if (!first)
			size_written += ft_dprintf(STDERR_FILENO, "|");
		size_written += ft_dprintf(STDERR_FILENO, "%#llx", flags);
	}
	return size_written;
}