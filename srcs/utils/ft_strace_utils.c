#include <ft_strace_utils.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    [SIGSYS] = "SIGSYS"
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
    [EXDEV] = "EXDEV"
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
    return strerror(errnum);
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