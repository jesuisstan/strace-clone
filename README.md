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
- **Compile all tests**: `make compile_tests`
- **Compile individual tests**: `make test_1`, `make test_2`, etc.
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

## 🧪 Testing

The project includes comprehensive test programs to verify different aspects of the `ft_strace` implementation.

### Compiling Tests

```bash
# Compile all test programs
make compile_tests

# Compile individual tests
make test_1
make test_2
make test_3
make test_4
make test_5
make test_threads
make test_close
make test_kill
```

### Running Tests

After compilation, you can run the tests manually with `ft_strace`:

#### Basic Tests (test_1 - test_5)

**test_1** - Process Creation and Synchronization

```bash
./ft_strace ./test_1
```

**Description**: Tests `fork()`, `wait()`, and `write()` syscalls. Creates a child process that writes "child", parent waits for child completion, then writes "parent". Verifies process creation, synchronization, and basic I/O operations.

**test_2** - Signal Handling and Input/Output

```bash
echo "test input" | ./ft_strace ./test_2
```

**Description**: Tests signal handling (`signal(SIGINT, ...)`), input reading (`read()`), and output writing (`write()`). Sets up a signal handler for SIGINT, reads from stdin, and writes to stderr. The `echo "test input" |` is required because the program calls `read(0, buf, 1024)` which blocks waiting for stdin input - without this input, the program would hang indefinitely.

**test_3** - Memory Mapping and File Operations

```bash
./ft_strace ./test_3
```

**Description**: Tests `mmap()`, `fork()`, `open()`, and `close()` syscalls in a loop. Attempts to open `/tmp/delme`, map it to memory, and perform file operations. Demonstrates memory mapping, process creation, and file handling.

**test_4** - Signal Masking and Signal Raising

```bash
./ft_strace ./test_4
```

**Description**: Tests `sigprocmask()`, `signal()`, and `raise()` syscalls. Blocks specific signals (SIGCHLD, 33, 35, 63, 64), sets up a signal handler for SIGWINCH, and raises the signal. Verifies signal masking and signal delivery.

**test_5** - Multi-threading and Process Management

```bash
./ft_strace ./test_5
```

**Description**: Tests `pthread_create()`, `fork()`, `waitpid()`, `sleep()`, and `exit()` syscalls. Creates a thread that pauses, forks a child process that creates a thread and exits, then waits for the child. Demonstrates multi-threading, process creation, and synchronization.

#### Additional Tests

**test_close** - File Descriptor Operations

```bash
./ft_strace ./test_close
```

**Description**: Tests `close()` syscall with various file descriptors including standard ones (stdin, stdout, stderr) and invalid descriptors. Verifies proper handling of file descriptor operations and error conditions.

**test_kill** - Process Control and Signal Sending

```bash
./ft_strace ./test_kill
```

**Description**: Tests `kill()` syscall with various signals and process IDs. Sends signals to the current process and invalid PIDs, verifying signal delivery and error handling.

### Test Output Analysis

Each test produces specific system call patterns that can be analyzed:

- **Process Creation**: Look for `fork()`, `clone()`, `clone3()` syscalls
- **File Operations**: Check `open()`, `read()`, `write()`, `close()` syscalls
- **Memory Management**: Examine `mmap()`, `munmap()`, `brk()` syscalls
- **Signal Handling**: Verify `sigprocmask()`, `rt_sigaction()`, `kill()` syscalls
- **Threading**: Look for `pthread_create()`, `futex()` syscalls
- **Process Termination**: Check `exit()`, `exit_group()` syscalls

### Comparing with Original strace

To compare output with the original `strace`:

```bash
# Run with ft_strace
./ft_strace ./test_1

# Run with original strace
strace ./test_1

# Compare filtered outputs
./ft_strace ./test_1 | grep -E "(child|parent|write|exit_group)" > ft_strace_output.log
strace ./test_1 | grep -E "(child|parent|write|exit_group)" > original_strace_output.log
diff ft_strace_output.log original_strace_output.log
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
│   │   └── analysis_routine.c
│   ├── execution/           # Process execution
│   │   └── exec_program.c
│   ├── syscall/             # System call handling
│   │   ├── param_log/       # Parameter logging functions
│   │   │   ├── utils/       # Parameter logging utilities
│   │   │   └── *.c          # Individual parameter type handlers
│   │   ├── syscall_64.c     # 64-bit syscall table
│   │   ├── syscall_32.h     # 32-bit syscall table
│   │   ├── syscall_handle.c # Main syscall processing
│   │   └── syscall_get_description.c
│   ├── statistics/          # Statistics tracking
│   │   ├── statistics_init.c
│   │   ├── statistics_add_entry.c
│   │   ├── statistics_get.c
│   │   ├── statistics_log.c
│   │   └── statistics_destroy.c
│   ├── utils/               # Utility functions
│   │   └── ft_strace_utils.c
│   ├── config.c             # Configuration management
│   └── main.c               # Main entry point
├── tests/                   # Test programs
│   ├── test_1.c             # Process creation and synchronization
│   ├── test_2.c             # Signal handling and I/O
│   ├── test_3.c             # Memory mapping and file operations
│   ├── test_4.c             # Signal masking and raising
│   ├── test_5.c             # Multi-threading and process management
│   ├── test_close.c         # File descriptor operations
│   └── test_kill.c          # Process control and signal sending
├── .build/                  # Build artifacts (auto-generated)
├── Makefile                 # Build configuration
├── .gitignore              # Git ignore rules
└── README.md               # This file
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
