#ifndef APP_H
#define APP_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define MY_DEV_FD0 "/dev/bares0"
#define MY_DEV_FD1 "/dev/bares1"
#define N_DEVICES 2



/* IOCTL macros definition */
#define IOC_GET_BUFFERSIZE 1
#define IOC_GET_MAJOR 4
#define IOC_GET_MINOR 3
#define IOC_SET_MEM_SIZE 5
#define IOC_GET_MEM_SIZE 6




 #endif