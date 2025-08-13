# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
SRCSDIR = srcs
OBJSDIR = .build
INCDIR = includes

# Find all .c files in srcs/
SRCS = $(shell find $(SRCSDIR) -name '*.c')
OBJS = $(SRCS:$(SRCSDIR)/%.c=$(OBJSDIR)/%.o)

# Main target
ft_strace: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "✅ strace-clone successfully compiled!"

# Pattern rule for object files
$(OBJSDIR)/%.o: $(SRCSDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCDIR) -MMD -MF $(@:.o=.d) -c $< -o $@

# Clean and fclean
clean:
	rm -rf $(OBJSDIR)
	rm -f test_threads
	rm -f test_close
	rm -f test_kill

fclean: clean
	rm -f ft_strace

re: fclean ft_strace

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re threads_test run_threads_test test

.PHONY: test_threads test_close test_kill

test_threads: ft_strace
	$(CC) $(CFLAGS) -pthread tests/test_threads.c -o test_threads && ./ft_strace ./test_threads

test_close: ft_strace
	$(CC) $(CFLAGS) tests/test_close.c -o test_close && ./ft_strace ./test_close

test_kill: ft_strace
	$(CC) $(CFLAGS) tests/test_kill.c -o test_kill && ./ft_strace ./test_kill

# Test ft_strace with basic Linux commands
test: ft_strace
	@echo "🧪 Running ft_strace tests..."
	./test_ft_strace.sh