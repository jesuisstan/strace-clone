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

# Тестовая программа с потоками
threads_test: threads_test.c
	$(CC) $(CFLAGS) -pthread threads_test.c -o threads_test

# Запуск теста
c: threads_test
	./threads_test

# Clean and fclean
clean:
	rm -rf $(OBJSDIR)
	rm -f threads_test
	rm -f thread_*.txt

fclean: clean
	rm -f ft_strace

re: fclean ft_strace

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re threads_test run_threads_test

# Компиляция и запуск теста одной командой
.PHONY: test_threads

test_threads:
	$(CC) $(CFLAGS) -pthread threads_test.c -o threads_test && ./ft_strace ./threads_test