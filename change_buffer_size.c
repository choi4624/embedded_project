#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h>  
#include <sys/ioctl.h>

#define TEST_DEV_MAJOR 200

#define CH_WRITE_BUFFER_SIZE _IOW(TEST_DEV_MAJOR,1,int)
#define CH_READ_BUFFER_SIZE _IOW(TEST_DEV_MAJOR,2,int)

int main(int argc, char **argv)
{

    int device;
    device = open("/dev/VirtualDisk", O_RDWR | O_NDELAY);
    int size = atoi(argv[1]);
    if (argc == 1)
    {
        ioctl(device, CH_WRITE_BUFFER_SIZE, &size);
    }
    if (argc == 2)
    {
        ioctl(device, CH_READ_BUFFER_SIZE, &size);
    }
    printf("buffer changed\n");

    close(device);


    return 0;
}
