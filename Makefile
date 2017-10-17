NAME    =   infinite_integer_tester

CC      =   gcc

FLAGS   =
ifeq ($(INFINITE_INTEGER_DEBUG), 1664)
    FLAGS   =   -Wall -Wextra -Werror -Wno-stack-protector                  \
                -g -fprofile-arcs -ftest-coverage
else
    FLAGS   =   -Wall -Wextra -Werror -Wno-stack-protector
endif

INCDIR  =   includes/

SRCDIR  =   srcs/

SRCS    =   $(SRCDIR)utilities.c $(SRCDIR)allocations.c $(SRCDIR)cleans.c $(SRCDIR)comparisons.c $(SRCDIR)operations.c  \
            unit_tests.c main.c

OBJS    =   $(SRCS:.c=.o)

GCOV    =   $(SRCS:.c=.c.gcov) $(SRCS:.c=.gcda) $(SRCS:.c=.gcno)


all     :   $(NAME)

%.o     :   %.c
			@$(CC) $(FLAGS) -o $@ -c $< -lm -I $(INCDIR)

$(NAME) :   $(OBJS)
			@$(CC) $(FLAGS) -o $(NAME) $(OBJS) -lm
			@echo "$(NAME) created"

clean   :
			@rm -f $(OBJS) $(GCOV)

fclean  :   clean
			@rm -f $(NAME)
			@echo "$(NAME) deleted"

re      :   fclean all


.PHONY  :   all clean fclean re
