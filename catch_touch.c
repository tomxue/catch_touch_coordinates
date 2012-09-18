#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

void handler (int sig)
{
    printf ("\nexiting...(%d)\n", sig);
    exit (0);
}

void perror_exit (char *error)
{
    perror (error);
    handler (9);
}

int main (int argc, char *argv[])
{
    struct input_event ev[64];
    int fd, rd, value, size = sizeof (struct input_event);
    char name[256] = "Unknown";
    char *device = NULL;
    
    //Setup check
    if (argv[1] == NULL){
        printf("Please input like this: sudo ./a.out /dev/input/event2\n");
        exit (0);
    }
    
    
    if ((getuid ()) != 0)
        printf ("You are not root! This may not work...\n");
    
    if (argc > 1)
        device = argv[1];
    
    //Open Device
    if ((fd = open (device, O_RDONLY)) == -1)
        printf ("%s is not a vaild device.\n", device);
    
    //Print Device Name
    ioctl (fd, EVIOCGNAME (sizeof (name)), name);
    printf ("Reading From : %s (%s)\n", device, name);
    
    unsigned int x ,y, z;
    
    while (1){
        if ((rd = read (fd, ev, size * 64)) < size)
            perror_exit ("read()");   
        
        if(ev[0].type == EV_ABS)    //Absolute axes，侦测触摸事件的绝对坐标值
        {
            if(ev[0].code == ABS_X)        //坐标原点位于bottom-left corner
                x = ev[0].value;
            if(ev[0].code == ABS_Y)
                y = ev[0].value;
            if(ev[0].code == ABS_Z)
                z = ev[0].value;
        }
        printf("x = %d\n", x);	
        printf("y = %d\n", y);
	printf("z = %d\n", z);	
        //return 0;
    }
}
