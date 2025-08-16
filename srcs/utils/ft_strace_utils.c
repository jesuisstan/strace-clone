#include <ft_strace_utils.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Signal trap codes for SIGTRAP signal
 * These constants define the reason why a SIGTRAP signal was generated
 * TRAP_BRKPT: Breakpoint trap (debugger breakpoint)
 * TRAP_TRACE: Trace trap (single step execution)
 * TRAP_BRANCH: Branch trap (branch instruction)
 * TRAP_HWBKPT: Hardware breakpoint trap
 * TRAP_UNK: Unknown trap reason
 */
#ifndef TRAP_BRKPT
#define TRAP_BRKPT 1
#endif
#ifndef TRAP_TRACE
#define TRAP_TRACE 2
#endif
#ifndef TRAP_BRANCH
#define TRAP_BRANCH 3
#endif
#ifndef TRAP_HWBKPT
#define TRAP_HWBKPT 4
#endif
#ifndef TRAP_UNK
#define TRAP_UNK 5
#endif

// Signal name mapping
static const char *signal_names[] = {
    [SIGHUP] = "SIGHUP",
    [SIGINT] = "SIGINT", 
    [SIGQUIT] = "SIGQUIT",
    [SIGILL] = "SIGILL",
    [SIGTRAP] = "SIGTRAP",
    [SIGABRT] = "SIGABRT",
    [SIGBUS] = "SIGBUS",
    [SIGFPE] = "SIGFPE",
    [SIGKILL] = "SIGKILL",
    [SIGUSR1] = "SIGUSR1",
    [SIGSEGV] = "SIGSEGV",
    [SIGUSR2] = "SIGUSR2",
    [SIGPIPE] = "SIGPIPE",
    [SIGALRM] = "SIGALRM",
    [SIGTERM] = "SIGTERM",
    [SIGSTKFLT] = "SIGSTKFLT",
    [SIGCHLD] = "SIGCHLD",
    [SIGCONT] = "SIGCONT",
    [SIGSTOP] = "SIGSTOP",
    [SIGTSTP] = "SIGTSTP",
    [SIGTTIN] = "SIGTTIN",
    [SIGTTOU] = "SIGTTOU",
    [SIGURG] = "SIGURG",
    [SIGXCPU] = "SIGXCPU",
    [SIGXFSZ] = "SIGXFSZ",
    [SIGVTALRM] = "SIGVTALRM",
    [SIGPROF] = "SIGPROF",
    [SIGWINCH] = "SIGWINCH",
    [SIGIO] = "SIGIO",
    [SIGPWR] = "SIGPWR",
    [SIGSYS] = "SIGSYS",
    [35] = "SIGRT_3",  // RT_3
    [63] = "SIGRT_31",  // RT_31
    [64] = "SIGRT_32"   // RT_32
};

const char *ft_signalname(int sig)
{
    if (sig >= 0 && sig < (int)(sizeof(signal_names) / sizeof(signal_names[0])) && signal_names[sig])
        return signal_names[sig];
    
    static char buf[32];
    snprintf(buf, sizeof(buf), "SIG%d", sig);
    return buf;
}

// Error name mapping
static const char *error_names[] = {
    [E2BIG] = "E2BIG",
    [EACCES] = "EACCES",
    [EADDRINUSE] = "EADDRINUSE",
    [EADDRNOTAVAIL] = "EADDRNOTAVAIL",
    [EAFNOSUPPORT] = "EAFNOSUPPORT",
    [EAGAIN] = "EAGAIN",
    [EALREADY] = "EALREADY",
    [EBADF] = "EBADF",
    [EBADMSG] = "EBADMSG",
    [EBUSY] = "EBUSY",
    [ECANCELED] = "ECANCELED",
    [ECHILD] = "ECHILD",
    [ECONNABORTED] = "ECONNABORTED",
    [ECONNREFUSED] = "ECONNREFUSED",
    [ECONNRESET] = "ECONNRESET",
    [EDEADLK] = "EDEADLK",
    [EDESTADDRREQ] = "EDESTADDRREQ",
    [EDOM] = "EDOM",
    [EDQUOT] = "EDQUOT",
    [EEXIST] = "EEXIST",
    [EFAULT] = "EFAULT",
    [EFBIG] = "EFBIG",
    [EHOSTUNREACH] = "EHOSTUNREACH",
    [EIDRM] = "EIDRM",
    [EILSEQ] = "EILSEQ",
    [EINPROGRESS] = "EINPROGRESS",
    [EINTR] = "EINTR",
    [EINVAL] = "EINVAL",
    [EIO] = "EIO",
    [EISCONN] = "EISCONN",
    [EISDIR] = "EISDIR",
    [ELOOP] = "ELOOP",
    [EMFILE] = "EMFILE",
    [EMLINK] = "EMLINK",
    [EMSGSIZE] = "EMSGSIZE",
    [EMULTIHOP] = "EMULTIHOP",
    [ENAMETOOLONG] = "ENAMETOOLONG",
    [ENETDOWN] = "ENETDOWN",
    [ENETRESET] = "ENETRESET",
    [ENETUNREACH] = "ENETUNREACH",
    [ENFILE] = "ENFILE",
    [ENOBUFS] = "ENOBUFS",
    [ENODATA] = "ENODATA",
    [ENODEV] = "ENODEV",
    [ENOENT] = "ENOENT",
    [ENOEXEC] = "ENOEXEC",
    [ENOLCK] = "ENOLCK",
    [ENOLINK] = "ENOLINK",
    [ENOMEM] = "ENOMEM",
    [ENOMSG] = "ENOMSG",
    [ENOPROTOOPT] = "ENOPROTOOPT",
    [ENOSPC] = "ENOSPC",
    [ENOSR] = "ENOSR",
    [ENOSTR] = "ENOSTR",
    [ENOSYS] = "ENOSYS",
    [ENOTCONN] = "ENOTCONN",
    [ENOTDIR] = "ENOTDIR",
    [ENOTEMPTY] = "ENOTEMPTY",
    [ENOTSOCK] = "ENOTSOCK",
    [ENOTSUP] = "ENOTSUP",
    [ENOTTY] = "ENOTTY",
    [ENXIO] = "ENXIO",
    [EOVERFLOW] = "EOVERFLOW",
    [EPERM] = "EPERM",
    [EPIPE] = "EPIPE",
    [EPROTO] = "EPROTO",
    [EPROTONOSUPPORT] = "EPROTONOSUPPORT",
    [EPROTOTYPE] = "EPROTOTYPE",
    [ERANGE] = "ERANGE",
    [EROFS] = "EROFS",
    [ESPIPE] = "ESPIPE",
    [ESRCH] = "ESRCH",
    [ESTALE] = "ESTALE",
    [ETIME] = "ETIME",
    [ETIMEDOUT] = "ETIMEDOUT",
    [ETXTBSY] = "ETXTBSY",
    [EXDEV] = "EXDEV",
    [516] = "ERESTART_RESTARTBLOCK"
};

const char *ft_errnoname(int errnum)
{
    if (errnum >= 0 && errnum < (int)(sizeof(error_names) / sizeof(error_names[0])) && error_names[errnum])
        return error_names[errnum];
    
    static char buf[32];
    snprintf(buf, sizeof(buf), "E%d", errnum);
    return buf;
}

const char *ft_strerror(int errnum)
{
	if (errnum == 516) {
		return "Interrupted by signal";
	}
	return strerror(errnum);
}

const char *ft_sicodename(int sig, int si_code)
{
	switch (sig) {
	case SIGILL:
		switch (si_code) {
		case ILL_ILLOPC: return "ILL_ILLOPC";
		case ILL_ILLOPN: return "ILL_ILLOPN";
		case ILL_ILLADR: return "ILL_ILLADR";
		case ILL_ILLTRP: return "ILL_ILLTRP";
		case ILL_PRVOPC: return "ILL_PRVOPC";
		case ILL_PRVREG: return "ILL_PRVREG";
		case ILL_COPROC: return "ILL_COPROC";
		case ILL_BADSTK: return "ILL_BADSTK";
		}
		break;
	case SIGFPE:
		switch (si_code) {
		case FPE_INTDIV: return "FPE_INTDIV";
		case FPE_INTOVF: return "FPE_INTOVF";
		case FPE_FLTDIV: return "FPE_FLTDIV";
		case FPE_FLTOVF: return "FPE_FLTOVF";
		case FPE_FLTUND: return "FPE_FLTUND";
		case FPE_FLTRES: return "FPE_FLTRES";
		case FPE_FLTINV: return "FPE_FLTINV";
		case FPE_FLTSUB: return "FPE_FLTSUB";
		}
		break;
	case SIGSEGV:
		switch (si_code) {
		case SEGV_MAPERR: return "SEGV_MAPERR";
		case SEGV_ACCERR: return "SEGV_ACCERR";
		case SEGV_BNDERR: return "SEGV_BNDERR";
		case SEGV_PKUERR: return "SEGV_PKUERR";
		}
		break;
	case SIGBUS:
		switch (si_code) {
		case BUS_ADRALN: return "BUS_ADRALN";
		case BUS_ADRERR: return "BUS_ADRERR";
		case BUS_OBJERR: return "BUS_OBJERR";
		case BUS_MCEERR_AR: return "BUS_MCEERR_AR";
		case BUS_MCEERR_AO: return "BUS_MCEERR_AO";
		}
		break;
	case SIGTRAP:
		switch (si_code) {
		case TRAP_BRKPT: return "TRAP_BRKPT";
		case TRAP_TRACE: return "TRAP_TRACE";
		case TRAP_BRANCH: return "TRAP_BRANCH";
		case TRAP_HWBKPT: return "TRAP_HWBKPT";
		case TRAP_UNK: return "TRAP_UNK";
		}
		break;
	case SIGCHLD:
		switch (si_code) {
		case CLD_EXITED: return "CLD_EXITED";
		case CLD_KILLED: return "CLD_KILLED";
		case CLD_DUMPED: return "CLD_DUMPED";
		case CLD_TRAPPED: return "CLD_TRAPPED";
		case CLD_STOPPED: return "CLD_STOPPED";
		case CLD_CONTINUED: return "CLD_CONTINUED";
		}
		break;
	case SIGPOLL:
		switch (si_code) {
		case POLL_IN: return "POLL_IN";
		case POLL_OUT: return "POLL_OUT";
		case POLL_MSG: return "POLL_MSG";
		case POLL_ERR: return "POLL_ERR";
		case POLL_PRI: return "POLL_PRI";
		case POLL_HUP: return "POLL_HUP";
		}
		break;
	}
	
	// Generic si_code values
	switch (si_code) {
	case SI_USER: return "SI_USER";
	case SI_KERNEL: return "SI_KERNEL";
	case SI_QUEUE: return "SI_QUEUE";
	case SI_TIMER: return "SI_TIMER";
	case SI_MESGQ: return "SI_MESGQ";
	case SI_ASYNCIO: return "SI_ASYNCIO";
	case SI_SIGIO: return "SI_SIGIO";
	case SI_TKILL: return "SI_TKILL";
	}
	
	static char buf[64];
	snprintf(buf, sizeof(buf), "UNKNOWN (%d)", si_code);
	return buf;
}

char *ft_escape(const char *str, size_t size)
{
    if (!str)
        return NULL;
    
    // Calculate needed size
    size_t needed = 1; // null terminator
    for (size_t i = 0; i < size && str[i]; i++) {
        if (str[i] < 32 || str[i] > 126 || str[i] == '\\' || str[i] == '"')
            needed += 4; // \xXX format
        else
            needed += 1;
    }
    
    char *result = malloc(needed);
    if (!result)
        return NULL;
    
    size_t j = 0;
    for (size_t i = 0; i < size && str[i]; i++) {
        if (str[i] < 32 || str[i] > 126 || str[i] == '\\' || str[i] == '"') {
            snprintf(result + j, 5, "\\x%02x", (unsigned char)str[i]);
            j += 4;
        } else {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';
    
    return result;
} 