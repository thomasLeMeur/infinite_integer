NAME	=	prog

CC		=	gcc

FLAGS	=	-Wall -Wextra -Werror -Wno-stack-protector

SRCS	=	utilities.c allocations.c cleans.c comparisons.c operations.c

#Uncomment these two following FLAGS SRCS to compile with debug and code coverage
FLAGS	=	-Wall -Wextra -Werror -Wno-stack-protector -g -fprofile-arcs -ftest-coverage
SRCS	=	utilities.c allocations.c cleans.c comparisons.c operations.c	\
			unit_tests.c main.c


OBJS	=	$(SRCS:.c=.o)

GCOV	=	$(SRCS:.c=.c.gcov) $(SRCS:.c=.gcda) $(SRCS:.c=.gcno)


all		:	$(NAME)

%.o		:	%.c
			@$(CC) $(FLAGS) -o $@ -c $<

$(NAME):	$(OBJS)
			@$(CC) $(FLAGS) -o $(NAME) $(OBJS)
			@echo "$(NAME) created"

clean	:
			@rm -f $(OBJS) $(GCOV) $(GCOV)

fclean	:	clean
			@rm -f $(NAME)
			@echo "$(NAME) deleted"

re		:	fclean all


.PHONY	:	all clean fclean re
