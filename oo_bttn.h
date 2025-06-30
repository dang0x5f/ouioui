#ifndef OO_BTTN_H
#define OO_BTTN_H

static long def_valuemask = 
(
    CWBackPixel|CWBorderPixel|CWEventMask
);
static long def_eventmask = 
(
    KeyPressMask|
    ExposureMask|
    EnterWindowMask|
    LeaveWindowMask|
    ButtonPressMask|
    ButtonReleaseMask|
    SubstructureNotifyMask
);

typedef void (*Callback)(void *cbdata);

typedef struct {
    Window win;
    XftDraw *draw;
    XftColor foreground;
    XftColor fg;
    XftColor inverted_fg;
    XftFont *xftfont;
    int background;
    int border;
    int width;
    int height;
    int x;
    int y;
    char *label;
    size_t label_len;
    void *cbdata;
    Callback buttonRelease;
} oo_button;

void create_button(Display*,Window*,int,XFontStruct*,XContext,XftFont*,int,int,int,int,Colormap*,int,int,char*,char*,size_t,Callback);
void expose_button(oo_button*,XEvent*);
void config_button(oo_button*,XEvent*);
void enter_button(oo_button*,XEvent*);
void leave_button(oo_button*,XEvent*);

#endif // OO_BTTN_H

#ifdef OO_BTTN_IMPLEMENTATION

void die(void *cbdata)
{
    printf("die\n");
    exit(EXIT_SUCCESS);
}

char invert_hex_char(char c)
{
    int n = 15-((c>='A'?c-'A'+10:c-'0') & 0xF);
    return(n>=10?'A'+(n-10):'0'+n);
}

void invert_color(char *in, char *out)
{
    size_t sz = 7;
    out[0]='#';
    for(int i=1; i<sz; ++i)
        out[i] = invert_hex_char(in[i]);
}

// oo_button 
void
create_button(Display *display, Window *parent, int screen_num, 
              XFontStruct *font, XContext context, XftFont *xftfont, 
              int x, int y, int width, int height, Colormap *colormap, 
              int border, int background, char *foreground, char *label, 
              size_t label_len, Callback cb_func)
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

    XftDraw *draw = XftDrawCreate(display, subwin, visual, *colormap);

    oo_button *button = malloc(sizeof(oo_button)); 

    XftColor color;
    // TODO: validate foreground
    //  - upper vs lower 
    //  - ( (c >= A && c <= F) || (c >= 0 && c <= 9) )
    XftColorAllocName(display,visual,*colormap,foreground,&color);
    button->foreground = color;
    button->fg = color;

    char inverted[8]={'\0'};
    invert_color(foreground,inverted);
    XftColorAllocName(display,visual,*colormap,inverted,&color);
    button->inverted_fg = color;

    button->draw = draw;
    button->xftfont = xftfont;
    button->background = background;
    button->border = border;
    button->width = width;
    button->height = height;
    button->x = (width/2)-((xftfont->max_advance_width * label_len)/2);
    button->y = (xftfont->ascent+xftfont->descent);
    button->label_len = label_len;
    button->label = malloc(label_len+1);
    button->label = label;
    button->buttonRelease = (cb_func?cb_func:die);

    XSaveContext(display,subwin,context,(XPointer)button);
    XMapWindow(display,subwin);
}

void expose_button(oo_button *button, XEvent *event)
{
    // TODO: keep as backup
    // XDrawString(event->xany.display, event->xany.window,
    //             DefaultGC(event->xany.display, DefaultScreen(event->xany.display)),
    //             button->x, button->y, button->label, button->label_len);
    XftDrawStringUtf8(button->draw,&(button->foreground),button->xftfont,
                      button->x, button->y, (FcChar8*)button->label, button->label_len);
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
    (button->foreground) = (button->inverted_fg);
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
    (button->foreground) = (button->fg);
    XChangeWindowAttributes(event->xany.display, event->xany.window,
                            CWBackPixel|CWBorderPixel, &attributes);
    XClearArea(event->xany.display, event->xany.window, 0,0, button->width,
               button->height, True);
}

#endif // OO_BTTN_IMPLEMENTATION
