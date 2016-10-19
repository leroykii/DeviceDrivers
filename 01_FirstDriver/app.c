    
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MY_DEV_FD0 "/dev/bares0"
#define MY_DEV_FD1 "/dev/bares1"
#define N_DEVICES 2

int main(int argc, char *argv[])
{
   printf("Helloworld\n");

   int fd[N_DEVICES];

   fd[0] = open(MY_DEV_FD0, O_RDONLY); 
   if (fd[0] == -1) {
    printf("Couldn't open file %d (%d)\n", 0, fd[0]);
    exit(0);
}
fd[1] = open(MY_DEV_FD1, O_RDONLY);
if (fd[1] == -1) {
    printf("Couldn't open file %d (%d)\n", 1, fd[1]);
    exit(0);
}


close(fd[0]);
close(fd[1]);

return 0;
}