CC := clang
CFLAGS := -Wall
INCLUDES := src/message.c src/sockwrapper.c

server:
	${CC} -o server src/server.c ${CFLAGS} ${INCLUDES}

client:
	${CC} -o client src/client.c ${CFLAGS} ${INCLUDES}
