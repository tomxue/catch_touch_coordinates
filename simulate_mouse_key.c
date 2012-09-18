#include <stdio.h>
#include <linux/input.h>	//event
#include <sys/time.h>	    //gettimeofday
#include <unistd.h>	        //write close
#include <fcntl.h>	        //open
#include <string.h>

int simulate_key(int fd,int code,int value)
{
    struct input_event event;
    memset(&event,0,sizeof(event));
    event.type = EV_KEY;
    event.code = code; //simulate what key
    event.value = value;
    if(write(fd,&event,sizeof(event)) < 0)
    {
        printf("Can not simulate key input\n");
        return -1;
    }

    event.type = EV_SYN;
    event.value = 0;
    event.code = SYN_REPORT;
    if(write(fd,&event,sizeof(event)) != sizeof(event))
    {
        printf("Can not simulate_mouse\n");
        return-1;
    }
    return 0;
}
int simulate_mouse(int fd,int buff[4])
{
    int rel_x,rel_y;
    static struct input_event event,ev;
    memset(&event,0,sizeof(event));
    printf("Mouse touch:x1=%d,y1=%d,x2=%d,y2=%d\n",buff[0],buff[1],buff[2],buff[3]);
    rel_x = (buff[0] + buff[2]) / 2;
    rel_y = -(buff[1] + buff[3]) / 2;
    event.type = EV_REL;
    event.code = REL_X;
    //event.value = rel_x;
    event.value = 100;
    gettimeofday(&event.time,NULL);
    if(write(fd,&event,sizeof(event)) != sizeof(event))
    {
        printf("Can not simulate_mouse\n");
        return -1;
    }
    event.type = EV_REL;
    event.code = REL_Y;
    //event.value = rel_y;
    event.value = 100;
    gettimeofday(&event.time,NULL);
    if(write(fd,&event,sizeof(event)) != sizeof(event))
    {
        printf("Can not simulate_mouse\n");
        return-1;
    }
    event.type = EV_SYN;
    event.value = 0;
    event.code = SYN_REPORT;
    if(write(fd,&event,sizeof(event)) != sizeof(event))
    {
        printf("Can not simulate_mouse\n");
        return-1;
    }
    //write(fd,&ev,sizeof(ev));	//must write other event to fresh
    return 0;
}

int main(int argc,char* argv[])
{
    int fd_key = 0;
    int fd_mouse = 0;
    int buff[4] = {100,100,500,500};

    if((fd_key = open("/dev/input/event1",O_RDWR)) == -1)
    {
        printf("Can not open file fd_key is -1\n");
    }

    if((fd_mouse = open("/dev/input/event2",O_RDWR)) == -1)
    {
        printf("Can not open file fd_mouse is -1\n");
    }

    simulate_key(fd_key,KEY_C,1);
    simulate_key(fd_key,KEY_C,0);
//	simulate_mouse(fd_mouse,buff);

    close(fd_key);
    close(fd_mouse);

    return 0;
}
