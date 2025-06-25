#ifndef OO_BTTN_H
#define OO_BTTN_H

static long def_valuemask = 
(
    CWBackPixel|CWBorderPixel|CWEventMask
);
static long def_eventmask = 
(
    KeyPressMask|EnterWindowMask|LeaveWindowMask|ExposureMask|SubstructureNotifyMask
);

typedef struct {
    Window win;
    GC gc;
    int background;
    int border;
    int width;
    int height;
    int x;
    int y;
    char *label;
    size_t label_len;
} oo_button;

void create_button(Display*,Window*,int,XFontStruct*,XContext,int,int,int,int,int,int,char*,size_t);
void expose_button(oo_button*,XEvent*);
void config_button(oo_button*,XEvent*);
void enter_button(oo_button*,XEvent*);
void leave_button(oo_button*,XEvent*);

#endif // OO_BTTN_H

#ifdef OO_BTTN_IMPLEMENTATION

// oo_button 
void
create_button(Display *display, Window *parent, int screen_num, 
              XFontStruct *font, XContext context, int x, int y,
              int width, int height, int border, int background,
              char *label, size_t label_len)
{
    int depth = DefaultDepth(display,screen_num);
    int class = InputOutput;
    Visual *visual = DefaultVisual(display,screen_num);
    int valuemask = def_valuemask;
    XSetWindowAttributes attributes = {
        .background_pixel = background,
        .border_pixel = border,
        .event_mask = def_eventmask,
    };

    Window subwin = XCreateWindow(display,*parent,x,y,width,height,2, depth,
                                  class,visual,valuemask,&attributes);

    // XSetFont(display,DefaultGC(display,screen_num),font->fid);

    oo_button *button = malloc(sizeof(oo_button)); 

    button->background = background;
    button->border = border;
    button->width = width;
    button->height = height;
    button->x = (width/2)-((font->per_char->width * label_len)/2);
    button->y = (font->ascent+font->descent);
    button->label_len = label_len;
    button->label = malloc(label_len+1);
    button->label = label;

    XSaveContext(display,subwin,context,(XPointer)button);
    XMapWindow(display,subwin);
}

void expose_button(oo_button *button, XEvent *event)
{
    XDrawString(event->xany.display, event->xany.window,
                DefaultGC(event->xany.display, DefaultScreen(event->xany.display)),
                button->x, button->y, button->label, button->label_len);
}

void config_button(oo_button *button, XEvent *event)
{
    XClearWindow(event->xany.display, event->xany.window);
}

void enter_button(oo_button *button, XEvent *event)
{
    XSetWindowAttributes attributes;
    attributes.background_pixel = button->border;
    attributes.border_pixel = button->background;
    XChangeWindowAttributes(event->xany.display, event->xany.window,
                            CWBackPixel|CWBorderPixel, &attributes);
    XClearArea(event->xany.display, event->xany.window, 0,0, button->width,
               button->height, True);
}

void leave_button(oo_button *button, XEvent *event)
{
    XSetWindowAttributes attributes;
    attributes.background_pixel = button->background;
    attributes.border_pixel = button->border;
    XChangeWindowAttributes(event->xany.display, event->xany.window,
                            CWBackPixel|CWBorderPixel, &attributes);
    XClearArea(event->xany.display, event->xany.window, 0,0, button->width,
               button->height, True);
}

#endif // OO_BTTN_IMPLEMENTATION
