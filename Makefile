NAME := ft_strace
CC := cc
CFLAGS := -Wall -Wextra -Werror
LIBS :=

SRCSDIR := srcs
OBJSDIR := .build
INCDIR := includes

# All source files
SRCS := main.c \
		config.c \
		utils/log_error.c \
		utils/initial_wait.c \
		utils/ft_strace_utils.c \
		execution/search_in_path.c \
		execution/exec_program.c \
		analysis/analysis_routine.c \
		syscall/syscall_get_description.c \
		syscall/syscall_handle.c \
		statistics/statistics_get.c \
		statistics/statistics_destroy.c \
		statistics/statistics_init.c \
		statistics/statistics_add_entry.c \
		statistics/statistics_log.c

OBJS := $(addprefix $(OBJSDIR)/,$(SRCS:.c=.o))
DEPS := $(addprefix $(OBJSDIR)/,$(SRCS:.c=.d))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

-include $(DEPS)
$(OBJSDIR)/%.o: $(SRCSDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCDIR) -MMD -MF $(@:.o=.d) -c $< -o $@

clean:
	rm -rf $(OBJSDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re