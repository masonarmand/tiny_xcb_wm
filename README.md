# tinyxcb
TinyWM ported to xcb. I made this project to learn the differences between Xlib
and xcb and to decide on which one I should use for future projects.

Why xcb instead of Xlib?  
 - xcb is fully asynchronous (faster than xlib)
 - xcb is newer and has a much smaller codebase
 - lower memory usage
 - lots more (read here: https://www.x.org/wiki/guide/xlib-and-xcb/)

xcb might seem to be an obvious choice for your next project, however it isn't
exactly easy to use. xcb is extremely verbose, lower level, and poorly documented.
Xlib's documentation is not too great, but when compared to xcb it's infinitely better
(because it actually exists).

## example of xcb verbosity
In Xlib lots of the types and functions have simple and obvious names.
For example the event struct is called `XEvent`. To get a button event you do this:
```C
XEvent e;
XButtonEvent e = e.xbutton;
```

In xcb the types and functions are extremely long. In xcb here is how you get a 
button event:
```C
xcb_generic_event_t* e;
xcb_button_press_event_t* ev = (xcb_button_press_event_t*) event->ev;
```

Here's what getting the attributes of a window looks like in Xlib:
```C
XWindowAttributes attrs;
if (XGetWindowAttributes(display, root, &attrs)) {
    printf("Window position: (%d, %d)\n", attrs.x, attrs.y);
    printf("Size: %dx%d\n", attrs.width, attrs.height);
}
```

And here's what it looks like in xcb...
```C
xcb_get_geometry_cookie_t geom_cookie = xcb_get_geometry(conn, root);
xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply(conn, geom_cookie, NULL);

if (geom) {
    printf("Window position: (%d, %d)\n", geom->x, geom->y);
    printf("Size: %dx%d\n", geom->width, geom->height);
    free(geom)
}
```



```
  PID User     Command                         Swap      USS      PSS      RSS
27485 mason    ./tinyxcb                          0      204      222     1872
```
```
  PID User     Command                         Swap      USS      PSS      RSS
27666 mason    ./tinywm                           0      264      308     2508
```

```
libxcb-keysyms1-dev
```
