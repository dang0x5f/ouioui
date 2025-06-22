#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <X11/Xlib.h>

#define OO_BTTN_IMPLEMENTATION
#include "oo_bttn.h"

// TODO: organize font setup
char *font_name = "fixed";
XFontStruct *font_info;
void font_setup(Display *display)
{
    font_info = XLoadQueryFont(display,font_name);
    if(!font_info) 
        exit(-1);
}

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

    font_setup(display);

    create_button(display,&window,screen_num,font_info);

    XMapWindow(display,window);
    XSync(display,false);
    XEvent event;
    while(1){
        /* oo_button *button = NULL; */
        XNextEvent(display,&event);
        switch(event.type){
            /* case Expose: */
            /*     if(button) button_expose(button,&event); */
            /*     break; */
        }
        if(event.xkey.keycode == 9) break;
    }

    return(0);
}
