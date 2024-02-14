CC := clang
CFLAGS := -Wall -g -gdwarf-4
INCLUDES := src/message.c src/sockwrapper.c

srv:
	${CC} -o server src/server.c ${CFLAGS} ${INCLUDES}

cli:
	${CC} -o client src/client.c ${CFLAGS} ${INCLUDES}
