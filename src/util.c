#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/* system call error, which focus on the condition: errno == -1 */
void serr(int err, const char *msg){
    if (err == -1){
	perror(msg);
	exit(EXIT_FAILURE);
    }
}

/* add O_NONBLOCK to specific fd */
void setNonBlocking (int fd) {
    int flags = fcntl(fd, F_GETFL);
    serr(flags, "fcntl() GET");
    flags = flags | O_NONBLOCK;
    serr(fcntl(fd, F_SETFL, flags), "fcntl() SET");
}
