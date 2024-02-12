SRC := src
CC := gcc
BUILD := build
CFLAGS := -c

tcp_server: ${BUILD}/server.o ${BUILD}/util.o
	${CC} $^ -o ${BUILD}/$@
	${BUILD}/$@

tcp_client: ${BUILD}/client.o ${BUILD}/util.o
	${CC} $^ -o ${BUILD}/$@
	${BUILD}/$@

${BUILD}/%.o: ${SRC}/%.c
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} $< -o $@

clean:
	rm -rf ${BUILD}