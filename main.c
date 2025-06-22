#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <X11/Xlib.h>

#define OO_BTTN_IMPLEMENTATION
#include "oo_bttn.h"

int main(void)
{
    Display *display = XOpenDisplay(NULL);
    int screen_num = DefaultScreen(display);
    Window root = DefaultRootWindow(display);
    int depth = DefaultDepth(display,screen_num);
    unsigned int class = InputOutput;
    Visual *visual = DefaultVisual(display,screen_num);
    unsigned long valuemask = 0;
    valuemask |= CWBackPixel;
    valuemask |= CWEventMask;
    XSetWindowAttributes attributes = {
        .background_pixel = 0xccccdd,
        .event_mask = KeyPressMask|EnterWindowMask|LeaveWindowMask,
    };

    Window window = XCreateWindow(display, root, 0, 0, 400, 300, 5, 
                        depth, class, visual, valuemask, &attributes);

    XMapWindow(display,window);
    XSync(display,false);
    XEvent event;
    while(1){
        XNextEvent(display,&event);
        if(event.xkey.keycode == 9) break;
    }

    return(0);
}
