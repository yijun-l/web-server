#include <stdio.h>
#include <stdlib.h>

/* system call error, which focus on the condition: errno == -1 */
void serr(int err, const char *msg){
    if (err == -1){
	perror(msg);
	exit(EXIT_FAILURE);
    }
}