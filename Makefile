# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = ft_strace

# Directories
SRCSDIR = srcs
OBJSDIR = .build
INCDIR = includes

# Find all .c files in srcs/
SRCS = $(shell find $(SRCSDIR) -name '*.c')
OBJS = $(SRCS:$(SRCSDIR)/%.c=$(OBJSDIR)/%.o)

# Main target
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "✅ strace-clone successfully compiled!"

# Pattern rule for object files
$(OBJSDIR)/%.o: $(SRCSDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCDIR) -MMD -MF $(@:.o=.d) -c $< -o $@

# Clean and fclean
clean:
	rm -rf $(OBJSDIR)
	rm -f test_close
	rm -f test_kill
	rm -f test_1 test_2 test_3 test_4 test_5

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

test_close: $(NAME) tests/test_close.c
	$(CC) $(CFLAGS) tests/test_close.c -o test_close && ./$(NAME) ./test_close

test_kill: $(NAME) tests/test_kill.c
	$(CC) $(CFLAGS) tests/test_kill.c -o test_kill && ./$(NAME) ./test_kill

test_1: $(NAME) tests/test_1.c
	$(CC) tests/test_1.c -o test_1

test_2: $(NAME) tests/test_2.c
	$(CC) tests/test_2.c -o test_2

test_3: $(NAME) tests/test_3.c
	$(CC) tests/test_3.c -o test_3

test_4: $(NAME) tests/test_4.c
	$(CC) tests/test_4.c -o test_4

test_5: $(NAME) tests/test_5.c
	$(CC) -pthread tests/test_5.c -o test_5

# Compile all basic tests at once
compile_tests: test_1 test_2 test_3 test_4 test_5
	@echo "✅ All basic tests compiled!"

# Include dependency files
-include $(OBJS:.o=.d)

.PHONY: all clean fclean re test_close test_kill test_1 test_2 test_3 test_4 test_5 compile_tests
