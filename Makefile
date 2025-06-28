NAME := ft_strace
CC := cc
CFLAGS := -Wall -Wextra -Werror
LIBS :=
FORMAT_COMMAND := clang-format -i -style=file

ifneq ($(filter debug redebug,$(MAKECMDGOALS)),)
	CFLAGS += -g
endif

SRCSDIR := srcs
OBJSDIR_RELEASE := .build/release
OBJSDIR_DEBUG := .build/debug
INCDIR := includes

SRCS_MAIN := main.c

# config srcs

SRCS += config.c

# utils srcs

SRCS += utils/log_error.c \
		utils/initial_wait.c \
		utils/ft_strace_utils.c

# execution srcs

SRCS += execution/search_in_path.c \
		execution/exec_program.c

# analysis srcs

SRCS += analysis/analysis_routine.c

# syscall srcs

SRCS += syscall/syscall_get_description.c \
		syscall/syscall_handle.c

# statistics srcs

SRCS += statistics/statistics_get.c \
		statistics/statistics_destroy.c \
		statistics/statistics_init.c \
		statistics/statistics_add_entry.c \
		statistics/statistics_log.c

OBJS_MAIN_RELEASE := $(addprefix $(OBJSDIR_RELEASE)/,$(SRCS_MAIN:.c=.o))
OBJS_MAIN_DEBUG := $(addprefix $(OBJSDIR_DEBUG)/,$(SRCS_MAIN:.c=.o))
OBJS_RELEASE := $(addprefix $(OBJSDIR_RELEASE)/,$(SRCS:.c=.o))
OBJS_DEBUG := $(addprefix $(OBJSDIR_DEBUG)/,$(SRCS:.c=.o))

ALL_SRCS := $(SRCS) $(SRCS_MAIN)

DEPS_RELEASE := $(addprefix $(OBJSDIR_RELEASE)/,$(ALL_SRCS:.c=.d))
DEPS_DEBUG := $(addprefix $(OBJSDIR_DEBUG)/,$(ALL_SRCS:.c=.d))

DFLAGS	= -MMD -MF $(@:.o=.d)

NAME_DEBUG := $(NAME)_debug

all: $(NAME)

debug: $(NAME_DEBUG)

$(NAME): $(OBJS_RELEASE) $(OBJS_MAIN_RELEASE)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS_RELEASE) $(OBJS_MAIN_RELEASE) $(LIBS)

$(NAME_DEBUG): $(OBJS_DEBUG) $(OBJS_MAIN_DEBUG)
	$(CC) $(CFLAGS) -o $(NAME_DEBUG) $(OBJS_DEBUG) $(OBJS_MAIN_DEBUG) $(LIBS)

-include $(DEPS_RELEASE)
$(OBJSDIR_RELEASE)/%.o: $(SRCSDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCDIR) $(DFLAGS) -c $< -o $@

-include $(DEPS_DEBUG)
$(OBJSDIR_DEBUG)/%.o: $(SRCSDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCDIR) $(DFLAGS) -c $< -o $@

clean:
	rm -rf .build

fclean: clean
	rm -f $(NAME) $(NAME_DEBUG)

re: fclean all

redebug: fclean debug

format:
	find . -regex '.*\.\(c\|h\)' -exec $(FORMAT_COMMAND) {} \;

.PHONY: all clean fclean re debug redebug