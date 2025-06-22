#ifndef OO_BTTN_H
#define OO_BTTN_H

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

// oo_button 
void
create_button(Display *display, Window *parent, int screen_num, 
              XFontStruct *font)
{
    int depth = DefaultDepth(display,screen_num);
    unsigned int class = InputOutput;
    Visual *visual = DefaultVisual(display,screen_num);
    unsigned long valuemask = 0;
    valuemask |= CWBackPixel;
    valuemask |= CWBorderPixel;
    valuemask |= CWEventMask;
    XSetWindowAttributes attributes = {
        .background_pixel = 0xbbbbbb,
        .border_pixel = 0x666666,
        .event_mask = KeyPressMask|EnterWindowMask|LeaveWindowMask,
    };

    Window subwin = XCreateWindow(display,*parent,0,0,100,20,2, depth,
                                  class,visual,valuemask,&attributes);
    XSetFont(display,DefaultGC(display,screen_num),font->fid);
    XDrawString(display,subwin,DefaultGC(display,screen_num),0,0,"Button",6);
    XMapWindow(display,subwin);
}
