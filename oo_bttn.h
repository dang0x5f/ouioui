#ifndef OO_BTTN_H
#define OO_BTTN_H

static long def_valuemask = 
(
    CWBackPixel|CWBorderPixel|CWEventMask
);
static long def_eventmask = 
(
    KeyPressMask|EnterWindowMask|LeaveWindowMask
);

typedef struct {
    Window win;
    GC gc;
    int x;
    int y;
    char *label;
    size_t label_len;
} oo_button;

void create_button(Display*,Window*,int,XFontStruct*);
void draw_button(Display*,oo_button*);

#endif // OO_BTTN_H

#ifdef OO_BTTN_IMPLEMENTATION

// oo_button 
void
create_button(Display *display, Window *parent, int screen_num, 
              XFontStruct *font)
{
    int depth = DefaultDepth(display,screen_num);
    int class = InputOutput;
    Visual *visual = DefaultVisual(display,screen_num);
    int valuemask = def_valuemask;
    XSetWindowAttributes attributes = {
        .background_pixel = 0xbbbbbb,
        .border_pixel = 0x666666,
        .event_mask = def_eventmask,
    };

    Window subwin = XCreateWindow(display,*parent,0,0,100,20,2, depth,
                                  class,visual,valuemask,&attributes);
    XSetFont(display,DefaultGC(display,screen_num),font->fid);
    XDrawString(display,subwin,DefaultGC(display,screen_num),0,0,"Button",6);
    XMapWindow(display,subwin);
}

#endif // OO_BTTN_IMPLEMENTATION
