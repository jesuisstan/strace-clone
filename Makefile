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

# Pattern rule for object files
$(OBJSDIR)/%.o: $(SRCSDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCDIR) -MMD -MF $(@:.o=.d) -c $< -o $@

# Clean and fclean
clean:
	rm -rf $(OBJSDIR)

fclean: clean
	rm -f ft_strace

re: fclean ft_strace

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re