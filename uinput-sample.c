/* Check below link to find more explaination:
 *     http://thiemonge.org/getting-started-with-uinput
 * The original code is from: 
 *     http://git.nosuchfile.org/?p=uinput-sample.git;a=blob_plain;f=uinput-sample.c;hb=HEAD
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)

int
main(void)
{
    int                    fd;
    struct uinput_user_dev uidev;
    struct input_event     ev;
    int                    dx, dy;
    int                    i;

    // /dev/uinput doesn't explicitly say key/mouse/touch
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("error: open");

    // says key
    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("error: ioctl EV_KEY");
    // says mouse
    if(ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        die("error: ioctl BTN_LEFT");

    if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
        die("error: ioctl EV_REL");
    if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
        die("error: ioctl REL_X");
    if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
        die("error: ioctl REL_Y");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
    uidev.id.bustype = BUS_USB; // BUS_ISA, BUS_PCI are tested OK
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        die("error: write");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: ioctl");

    sleep(1);

    srand(time(NULL));

    // test key press action
    while(0) {
        // BTN_LEFT pressed and releasd
        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_KEY;
        ev.code = BTN_LEFT;
        ev.value = 1;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write REL_ENTER");

        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write EV_SYN");

        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_KEY;
        ev.code = BTN_LEFT;
        ev.value = 0;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write REL_ENTER");

        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write EV_SYN");

        // KEY_A pressed and released
        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_KEY;
        ev.code = KEY_A;
        ev.value = 1;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write REL_ENTER");

        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write EV_SYN");

        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_KEY;
        ev.code = KEY_A;
        ev.value = 0;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write REL_ENTER");

        memset(&ev, 0, sizeof(struct input_event));
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        if(write(fd, &ev, sizeof(struct input_event)) < 0)
            die("error: write EV_SYN");
    }

    // test mouse movement
    while(1) {
        switch(rand() % 4) {
        case 0:
            dx = -10;
            dy = -1;
            break;
        case 1:
            dx = 10;
            dy = 1;
            break;
        case 2:
            dx = -1;
            dy = 10;
            break;
        case 3:
            dx = 1;
            dy = -10;
            break;
        }

        for(i = 0; i < 10; i++) {
            memset(&ev, 0, sizeof(struct input_event));
            ev.type = EV_REL;
            ev.code = REL_X;
            ev.value = dx;
            if(write(fd, &ev, sizeof(struct input_event)) < 0)
                die("error: write REL_X");

            memset(&ev, 0, sizeof(struct input_event));
            ev.type = EV_REL;
            ev.code = REL_Y;
            ev.value = dy;
            if(write(fd, &ev, sizeof(struct input_event)) < 0)
                die("error: write REL_Y");

            memset(&ev, 0, sizeof(struct input_event));
            ev.type = EV_SYN;
            ev.code = SYN_REPORT;
            ev.value = 0;
            if(write(fd, &ev, sizeof(struct input_event)) < 0)
                die("error: write EV_SYN");

            usleep(15000);
        }

        sleep(1);
    }

    sleep(1);

    if(ioctl(fd, UI_DEV_DESTROY) < 0)
        die("error: ioctl");

    close(fd);

    return 0;
}
