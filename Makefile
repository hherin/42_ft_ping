NAME=	ft_ping

SRC=	./src/main.c \
		./src/utils.c

OBJ=	${SRC:.c=.o}

CFLAG=	-g3 -fsanitize=address -Wall -Werror -Wextra

all: ${NAME}

%.o:%.c
	gcc ${CFLAG} -o $@ -c $<

${NAME}:	${OBJ}
			make -C lib
			gcc ${CFLAG} -o ${NAME} ${OBJ} lib/libftprintf.a

clean:
			@make clean -C lib
			@rm -rf src/*.o

fclean:
			@make fclean -C lib
			@rm -rf ${NAME}

re:			fclean all

.PHONY: all clean fclean re