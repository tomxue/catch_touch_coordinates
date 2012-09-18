// the original code below is from internet, the link is ?
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

static int event_fd = -1;
struct input_event ev0[64];

// handling event0/1/2, ?/key/mouse - Tom's Ubuntu
// handling event1, mouse - BB Ubuntu
static int handle_event_mouse()
{
    int button = 0, realx = 0, realy = 0, i, rd;

    printf("The sizeof struct input_event is: %d\n", sizeof(struct input_event));
    rd = read(event_fd, ev0, sizeof(struct input_event)*64);
    if (rd < sizeof(struct input_event))
        return 0;

    for (i = 0; i < rd / sizeof(struct input_event); i++) {
        //printf("", ev0[i].type, ev0[i].code, ev0[i].value);
        if (ev0[i].type == 3 && ev0[i].code == 0)
            realx = ev0[i].value;
        else if (ev0[i].type == 3 && ev0[i].code == 1)
            realy = ev0[i].value;
        else if (ev0[i].type == 1) {
            if (ev0[i].code == 158) {
                //if key esc then exit
                return 0;
            }
        } else if (ev0[i].type == 0 && ev0[i].code == 0 && ev0[i].value == 0) {
            realx = 0, realy = 0;
        }

        printf("event(%d): type: %d; code: %3d; value: %3d; realx: %3d; realy: %3d\n", i,
               ev0[i].type, ev0[i].code, ev0[i].value, realx, realy);
    }

    return 1;
}

int main(void)
{
    int done = 1;
    printf("sizeof(struct input_event) = %d\n", sizeof(struct input_event));

    // event1 - keyboard
    // event2 - mouse
    event_fd = open("/dev/input/event2", O_RDWR);

    if ( event_fd < 0 ) {
        printf("event_fd < 0\n");;
        return -1;
    }

    while ( done ) {
        printf("begin handel_event0...\n");
        done = handle_event_mouse();
        printf("end handel_event0...\n");
    }

    if ( event_fd > 0 ) {
        close(event_fd);
        event_fd = -1;
    }

    return 0;
}
