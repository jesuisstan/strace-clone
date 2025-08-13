# strace-clone

A custom implementation of the Linux `strace` utility, designed to trace system calls and signals of a running process. This project demonstrates advanced system programming concepts including process tracing, signal handling, and system call interception.

## 🎯 Project Overview

This project recreates the core functionality of the original `strace` utility, which is a powerful diagnostic, instructional, and debugging tool for Linux. It allows you to monitor the system calls used by a program and the signals received by it.

### Key Features

- **System Call Tracing**: Monitor all system calls made by a traced process
- **Real-time Output**: Display system calls as they occur with detailed parameter information
- **Symbolic Constants**: Show human-readable symbolic names for flags and constants
- **Binary Data Display**: Properly format binary data in system calls like `read`, `write`, `getrandom`
- **Error Handling**: Display meaningful error messages and codes
- **Process Management**: Handle process creation, execution, and termination
- **Signal Handling**: Intercept and display signal information

## 🚀 Features

### Core Functionality

- ✅ **Process Tracing**: Attach to running processes or trace new processes
- ✅ **System Call Interception**: Capture both entry and exit of system calls
- ✅ **Parameter Decoding**: Display system call parameters in human-readable format
- ✅ **Return Value Display**: Show system call return values with proper formatting
- ✅ **Error Reporting**: Display error codes and descriptions for failed system calls

### Advanced Features

- ✅ **Symbolic Constants**: Convert numeric flags to symbolic names (e.g., `O_RDONLY|O_CLOEXEC`)
- ✅ **String Truncation**: Intelligently truncate long strings with `...` notation
- ✅ **Binary Data Formatting**: Display binary data in hex format with escape sequences
- ✅ **Special Syscall Handling**: Custom formatting for complex system calls like `execve`, `mmap`, `ioctl`
- ✅ **Memory Safety**: Safe reading of process memory using `process_vm_readv`

### Supported System Calls

- **File Operations**: `open`, `openat`, `read`, `write`, `pread64`, `close`, `fstat`
- **Memory Management**: `mmap`, `munmap`, `mprotect`, `brk`
- **Process Control**: `execve`, `exit`, `exit_group`, `fork`, `clone`, `clone3`
- **System Information**: `getrandom`, `arch_prctl`, `set_tid_address`
- **File System**: `access`, `statfs`, `getdents64`
- **And many more...**

## 🛠️ Technical Implementation

### Architecture

- **Process Tracing**: Uses `ptrace(PTRACE_SEIZE)` for attaching to processes and `ptrace(PTRACE_INTERRUPT)` for starting tracing
- **System Call Interception**: Monitors syscall entry/exit using `PTRACE_SYSCALL`
- **Memory Reading**: Safely reads process memory using `process_vm_readv` system call
- **Signal Handling**: Blocks and handles signals to prevent interference

### Allowed PTRACE Options

This implementation strictly follows the subject requirements and uses only the following PTRACE options:

- `PTRACE_SYSCALL` - Monitor system call entry and exit
- `PTRACE_GETREGSET` - Get register sets from traced process
- `PTRACE_SETOPTIONS` - Set tracing options
- `PTRACE_GETSIGINFO` - Get signal information
- `PTRACE_SEIZE` - Attach to running process
- `PTRACE_INTERRUPT` - Interrupt traced process
- `PTRACE_LISTEN` - Listen for process events

### Key Components

- **Main Loop**: Coordinates process tracing and system call monitoring
- **Syscall Handler**: Decodes and formats system call information
- **Parameter Logger**: Handles different parameter types (strings, integers, structures)
- **Memory Reader**: Safely reads data from traced process memory using `process_vm_readv`
- **Error Handler**: Manages and displays error conditions

## 📦 Building and Installation

### Prerequisites

- Linux operating system (tested on Ubuntu 22.04+)
- GCC compiler with C99 support
- Make build system
- Standard C library headers

### Build Instructions

1. **Clone the repository**:

   ```bash
   git clone <repository-url>
   cd strace-clone
   ```

2. **Build the project**:

   ```bash
   make
   ```

3. **Clean build artifacts** (optional):
   ```bash
   make clean
   ```

### Build Options

- **Release build** (default): `make`
- **Debug build**: `make debug`
- **Clean build**: `make clean`
- **Reinstall**: `make re`
- **Test with threads**: `make test_threads`
- **Test kill cmd**: `make test_kill`
- **Test close cmd**: `make test_close`

## 🎮 Usage

### Basic Usage

```bash
# Trace a new process
./ft_strace <command>

# Examples
./ft_strace ls
./ft_strace echo "Hello, World!"
./ft_strace cat /etc/passwd
```

### Advanced Usage

```bash
# Test with multi-threaded program
make test_threads

# Trace with specific system calls
./ft_strace -c echo "Hello, World!"
```

### Output Format

The output follows the standard `strace` format:

```
syscall_name(parameters) = return_value
```

**Examples**:

```
execve("/usr/bin/ls", ...) = 0
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC, 0) = 3
read(3, "\177ELF\002\001\001\000\000\000\000\000\000\000\000\000\003\000>\000\001\000\000\000\000\000\000\000\000\000"..., 832) = 832
write(1, "file1  file2  file3"..., 47) = 47
```

## 🔍 Features in Detail

### Symbolic Constants

The tool automatically converts numeric flags to symbolic names:

- `4` → `R_OK`
- `-100` → `AT_FDCWD`
- `0x8000` → `O_RDONLY|O_CLOEXEC`

### Binary Data Display

Binary data is displayed in a readable format:

- Printable ASCII characters shown as-is
- Non-printable characters shown as escape sequences (`\t`, `\n`, `\r`)
- Other bytes shown in octal format (`\177`)

### String Truncation

Long strings are intelligently truncated:

```
write(1, "This is a very long string that will be truncated..."..., 100) = 100
```

### Error Handling

Failed system calls show detailed error information:

```
openat(AT_FDCWD, "/nonexistent/file", O_RDONLY|O_CLOEXEC, 0) = -1 ENOENT (No such file or directory)
```

## 🎯 Educational Value

This project demonstrates several important system programming concepts:

### Process Management

- Process creation and execution
- Parent-child process relationships
- Process state monitoring

### System Call Interception

- Using `ptrace` for process tracing
- System call entry/exit monitoring
- Parameter and return value extraction

### Memory Management

- Safe reading of process memory using `process_vm_readv`
- Handling different data types
- Memory layout understanding

### Signal Handling

- Signal blocking and unblocking
- Signal delivery mechanisms
- Process control flow

## 🔗 References

### Original strace Documentation

- **Official strace**: [https://strace.io/](https://strace.io/)
- **Linux man page**: `man strace`
- **GitHub repository**: [https://github.com/strace/strace](https://github.com/strace/strace)

### Related Documentation

- **ptrace(2)**: `man 2 ptrace`
- **syscalls(2)**: `man 2 syscalls`
- **signal(7)**: `man 7 signal`
- **process_vm_readv(2)**: `man 2 process_vm_readv`

### Learning Resources

- **Linux System Programming**: Understanding system calls and process management
- **Advanced Programming in the UNIX Environment**: Process control and interprocess communication
- **The Linux Programming Interface**: Comprehensive guide to Linux system programming

## 🏗️ Project Structure

```
strace-clone/
├── includes/                # Header files
│   ├── analysis.h           # Analysis routine declarations
│   ├── config.h             # Configuration structures
│   ├── execution.h          # Process execution functions
│   ├── ft_strace_utils.h    # Utility functions
│   ├── statistics.h         # Statistics tracking
│   ├── syscall_strace.h     # System call handling
│   └── types.h              # Common type definitions
├── srcs/                    # Source code
│   ├── analysis/            # System call analysis
│   ├── execution/           # Process execution
│   ├── syscall/             # System call handling
│   │   ├── param_log/       # Parameter logging
│   │   └── custom_handlers/ # Custom syscall handlers
│   └── utils/               # Utility functions
├── Makefile                 # Build configuration
├── threads_test.c           # Multi-threaded test program
└── README.md                # This file
```

## 🐛 Known Limitations

- **32-bit Support**: Currently optimized for 64-bit systems
- **Complex Structures**: Some complex data structures (like `stat`) are shown as addresses
- **Signal Handling**: Limited signal tracing compared to original strace
- **Performance**: May have higher overhead than the original strace

## 🤝 Contributing

This is an educational project, but contributions are welcome! Areas for improvement:

- Enhanced signal handling
- Support for more system calls
- Better structure decoding
- Performance optimizations
- Additional formatting options

## 📄 License

This project is created for educational purposes. The original `strace` utility is licensed under the LGPL-2.1+ license.

## 🙏 Acknowledgments

- **Original strace developers**: For creating the excellent reference implementation
- **Linux kernel developers**: For the comprehensive system call interface
- **System programming community**: For documentation and examples

---

**Note**: This is an educational implementation and should not be used in production environments. For production use, please use the official `strace` utility.
