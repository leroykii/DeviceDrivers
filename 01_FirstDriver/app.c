#include "app.h"


#define BUFFER_SIZE 128





int main(int argc, char *argv[]) {
   printf("Helloworld\n");

   int fd[N_DEVICES];
   unsigned char buffer0[BUFFER_SIZE] = {0};
   unsigned char buffer1[BUFFER_SIZE] = {0};
   int i;
   int nbytes = 0;

   int buffersize = 0;
   int io_arg = 0;



   fd[0] = open(MY_DEV_FD0, O_RDWR); 
   if (fd[0] == -1) {
    printf("Couldn't open file %d (%d)\n", 0, fd[0]);
    exit(0);
}

fd[1] = open(MY_DEV_FD1, O_RDWR);
if (fd[1] == -1) {
    printf("Couldn't open file %d (%d)\n", 1, fd[1]);
    exit(0);
}



return 0;

if (ioctl(fd[0], IOC_GET_MAJOR, &buffersize) < 0) printf("error\n");
printf("buffersize: %d\n", buffersize);



if (ioctl(fd[0], IOC_GET_BUFFERSIZE, &buffersize) < 0 ) printf("error\n");
printf("buffersize: %d\n", buffersize);

ioctl(fd[1], IOC_GET_MINOR, &buffersize);
printf("buffersize: %d\n", buffersize);

ioctl(fd[0], IOC_GET_MINOR, &buffersize);
printf("buffersize: %d\n", buffersize);

return 0;

/*
*/

unsigned char init_value = 0xAA;
for (i = 0; i < 5; ++i)
{
    memset(buffer0, init_value++, BUFFER_SIZE);
    nbytes = write(fd[0], buffer0, BUFFER_SIZE);
    printf("Written: %d\n", nbytes);
}
memset(buffer0, init_value++, BUFFER_SIZE);
nbytes = write(fd[1], buffer0, BUFFER_SIZE);

nbytes = pread(fd[1], buffer1, BUFFER_SIZE, 128);
printf("Read: %d\n", nbytes);
for (i = 0; i < BUFFER_SIZE; ++i)
{
    printf("%02x ", buffer1[i]);
}
printf("\n");

nbytes = pread(fd[1], buffer1, BUFFER_SIZE, 768);
printf("Read: %d\n", nbytes);
for (i = 0; i < BUFFER_SIZE; ++i)
{
    printf("%02x ", buffer1[i]);
}
printf("\n");


nbytes = lseek(fd[1], 384, SEEK_SET);
nbytes = lseek(fd[0], 384, SEEK_SET);
printf("Result offset is: %d\n", nbytes);


nbytes = write(fd[0], buffer0, BUFFER_SIZE);
nbytes = write(fd[0], buffer0, BUFFER_SIZE);

read(fd[1], buffer1, BUFFER_SIZE);

close(fd[0]);
close(fd[1]);

return 0;
}