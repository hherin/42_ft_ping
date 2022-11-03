NAME=	ft_ping

SRC=	./src/main.c \
		./src/utils.c \
		./src/ping_loop.c \
		./src/recv.c \
		./src/send.c \
		./src/stats.c

OBJ=	${SRC:.c=.o}

HDRS=	./inc/ft_ping.h

CFLAG=	-g3 -fsanitize=address -Wall -Werror -Wextra

all: ${NAME}

${OBJ} : ${HDRS}

%.o:%.c
	gcc ${CFLAG} -o $@ -c $<

${NAME}:	${OBJ}
			make -C lib
			gcc ${CFLAG} -o ${NAME} ${OBJ} lib/libft.a

clean:
			@make clean -C lib
			@rm -rf src/*.o

fclean:		clean
			@make fclean -C lib
			@rm -rf ${NAME}

re:			fclean all

.PHONY: all clean fclean re