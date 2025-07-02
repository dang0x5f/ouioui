#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <X11/Xlib.h>
/* #include <X11/Xutil.h> */
#include <X11/Xresource.h>
#include <X11/Xft/Xft.h>

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

char *font_pattern = "Deja Vu Sans Mono:weight=bold:size=8";
XftFont *xftfont;
void xftfont_setup(Display *display, int screen_num)
{
    xftfont = XftFontOpenName(display,screen_num,font_pattern);
    if(!xftfont){
        perror("XftFontOpenName() error\n");
        exit(EXIT_FAILURE);
    }
}

void prt_hello(void *cbdata)
{
    printf("Hello\n");
}

int main(void)
{
    /* XrmInitialize(); */
    XContext context = XUniqueContext();

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
        .event_mask = KeyPressMask|ExposureMask|SubstructureNotifyMask,
    };
    Colormap colormap = DefaultColormap(display, screen_num);

    Window window = XCreateWindow(display, root, 0, 0, 400, 300, 5, 
                        depth, class, visual, valuemask, &attributes);

    /* font_setup(display); */
    xftfont_setup(display,screen_num);
    char *l = "SAy ZON ZEE";
    create_button(display,&window,screen_num,font_info,context,xftfont,0,0,100,&colormap,  0x888888,0xbbbbbb,"#000000",l,strlen(l),prt_hello);
    create_button(display,&window,screen_num,font_info,context,xftfont,0,100,100,&colormap, 0x888888,0xbbbbbb,"#000000",l,strlen(l),NULL);
    create_button(display,&window,screen_num,font_info,context,xftfont,0,200,100,&colormap, 0x888888,0xbbbbbb,"#000000",l,strlen(l),NULL);
    create_button(display,&window,screen_num,font_info,context,xftfont,0,300,100,&colormap, 0x888888,0xbbbbbb,"#000000",l,strlen(l),NULL);
    create_button(display,&window,screen_num,font_info,context,xftfont,0,400,100,&colormap,0x888888,0xbbbbbb,"#000000",l,strlen(l),NULL);
    create_button(display,&window,screen_num,font_info,context,xftfont,0,500,100,&colormap,0x888888,0xbbbbbb,"#000000",l,strlen(l),NULL);

    printf("ascent %d : descent %d\nheight %d\n",
           xftfont->ascent, xftfont->descent, xftfont->height);

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
                break;
            case Expose:
                if(button) expose_button(button,&event);
                break;
            case EnterNotify:
                if(button) enter_button(button,&event);
                break;
            case LeaveNotify:
                if(button) leave_button(button,&event);
                break;
            case ButtonRelease:
                if(button) button->buttonRelease(button->cbdata);
                break;
        }
        if(event.xkey.keycode == 9) break;
    }

    return(EXIT_SUCCESS);
}
