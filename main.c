#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

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
    /* XrmInitialize(); */

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
        .event_mask = ExposureMask|SubstructureNotifyMask,
    };
    XContext context = XUniqueContext();

    Window window = XCreateWindow(display, root, 0, 0, 400, 300, 5, 
                        depth, class, visual, valuemask, &attributes);

    font_setup(display);

    create_button(display,&window,screen_num,font_info,context,0,0,100,20);

    XMapWindow(display,window);
    XSync(display,false);

    XEvent event;
    while(1){
        oo_button *button = NULL;
        XNextEvent(display,&event);
        XFindContext(event.xany.display, event.xany.window, 
                     context, (XPointer*)&button);
        switch(event.type){
            case ConfigureNotify:
                if(button) config_button(button,&event);
            case Expose:
                if(button) expose_button(button,&event);
                break;
            case EnterNotify:
                if(button) enter_button(button,&event);
        }
        if(event.xkey.keycode == 9) break;
    }

    return(0);
}
