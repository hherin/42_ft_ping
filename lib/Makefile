CC	= gcc

HDRS =	inc/libft.h 

RM	= rm -f

NAME	= libft.a

SRCS = 	./lib/alloc_string.c \
		./lib/converter.c \
		./lib/ft_put_fd.c \
		./lib/is_type.c\
		./lib/list.c\
		./lib/memstr.c\
		./lib/stringh.c\
		./lib/strsearch.c

OBJS	= ${SRCS:.c=.o}

CFLAGS	= -Wall -Wextra -Werror -g


${NAME}:	${OBJS}
			ar rc ${NAME} ${OBJS}
			ranlib ${NAME}


all: ${NAME}

%.o:%.c 
	${CC} ${CFLAGS} -o $@ -c $<


clean:
	${RM} ${BONUS_OBJS} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all bonus

.PHONY: all clean fclean re