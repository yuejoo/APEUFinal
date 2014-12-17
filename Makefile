PROG=	sish
OBJS=	main.o utils.o stack.o parse.o
 
CFLAGS=	-Wall -g

all: ${PROG}

${PROG}: ${OBJS}
	@echo $@ depends on $?
	${CC} ${LDFLAGS} ${OBJS} -o ${PROG} -D READLINE_ON -I /usr/include -lreadline -Itermcap 

clean:
	rm -f sish *.o
