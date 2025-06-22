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

#endif // OO_BTTN_H
