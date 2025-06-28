#include "param_log.h"
#include <ft_printf.h>
#include <sys/types.h>
#include <unistd.h>

struct ustat
{
	long f_tfree;
	long f_tinode;
	char f_fname[6];
	char f_fpack[6];
};

int log_USTAT_STRUCT(uint64_t value, syscall_log_param_t *context)
{
	STRUCT_HANDLE(struct ustat, ustat);
	return ft_dprintf(STDERR_FILENO, "{f_tfree=%ld, f_tinode=%ld, f_fname=\"%s\", f_fpack=\"%s\"}",
					  ustat.f_tfree, ustat.f_tinode, ustat.f_fname, ustat.f_fpack);
}