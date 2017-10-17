NAME	=	infinite_integer_tester

CC		=	gcc

#Delete the second line for a release compile
FLAGS	=	-Wall -Wextra -Werror -Wno-stack-protector 						\
			-g -fprofile-arcs -ftest-coverage

INCDIR	=	includes/

SRCDIR	=	srcs/

#Delete the second line to keep the necessary files
SRCS	=	$(SRCDIR)utilities.c $(SRCDIR)allocations.c $(SRCDIR)cleans.c $(SRCDIR)comparisons.c $(SRCDIT)operations.c	\
			unit_tests.c main.c


OBJS	=	$(SRCS:.c=.o)

GCOV	=	$(SRCS:.c=.c.gcov) $(SRCS:.c=.gcda) $(SRCS:.c=.gcno)


all		:	$(NAME)

%.o		:	%.c
			@$(CC) $(FLAGS) -o $@ -c $< -lm -I $(INCDIR)

$(NAME):	$(OBJS)
			@$(CC) $(FLAGS) -o $(NAME) $(OBJS) -lm
			@echo "$(NAME) created"

clean	:
			@rm -f $(OBJS) $(GCOV)

fclean	:	clean
			@rm -f $(NAME)
			@echo "$(NAME) deleted"

re		:	fclean all


.PHONY	:	all clean fclean re
