#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h>  
#include <sys/ioctl.h>

#define TEST_DEV_MAJOR 200

#define CH_WRITE_BUFFER_SIZE _IOW(TEST_DEV_MAJOR,1,int)
#define CH_READ_BUFFER_SIZE _IOW(TEST_DEV_MAJOR,2,int)

int main(int argc, char* argv[])
{

    int device;
    int size = atoi(argv[2]);
    int type = atoi(argv[1]);
    device = open("/dev/VirtualDisk", O_RDWR | O_NDELAY);
    printf("argc %d\n",argc);
    printf("argv %c\n",*argv[1]);
    printf("argv2 %d\n",size);
    
    if (type == 1)
    {
        ioctl(device, CH_WRITE_BUFFER_SIZE, &size);
        printf("buffer wr changed %d\n", size);
    }
    if (type == 2)
    {
        ioctl(device, CH_READ_BUFFER_SIZE, &size);
        printf("buffer rd changed %d\n", size);
    }
    printf("changed\n");

    close(device);

    return 0;
}
