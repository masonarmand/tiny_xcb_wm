# tinyxcb
TinyWM ported to XCB. This project compares Xlib and XCB to help decide which is better for future projects.

Why xcb?  
 - Asynchronous API (faster than xlib)
 - xcb is newer and has a much smaller codebase
 - lower memory usage
 - lots more (read here: https://www.x.org/wiki/guide/xlib-and-xcb/)

Why Xlib?
 - higher level API (more built in helper functions/types)
 - Easier to learn and has better documentation

## example of xcb verbosity
Xlib has simpler, more readable types. Example:
```C
XEvent event;
XButtonEvent button_event = event.xbutton;
```

xcb on the other hand has long function/type names and requires casting events.
```C
xcb_generic_event_t* event;
xcb_button_press_event_t* button_event = (xcb_button_press_event_t*) event;
```
Compare the source code of tinyxcb with TinyWM (https://github.com/mackstann/tinywm)
to guage the difference between the verbosity of the two libraries.

## Naming Consistency
xbc prefixes all types/functions with `xcb_`, making it easier to distinguish from user-defined types.
Xlib, however, uses `X` inconsistently (Window and Display are not prefixed with X), leading to confusion in projects like dwm (where you have custom types like Monitor mixed with Xlibs types).

## SLOC (Source Lines of Code) Comparison:
Source Lines of Code is the lines of code not including line breaks and comments.
```
tinyxcb - 150 SLOC
tinywm  - 40  SLOC
```
Even if tinyxcb were written like tinywm (everything in main()), it would still be about 3x larger.

## Memory Usage of tinyxcb vs tinyWM
Measured using `smem` (PSS):
```
tinyxcb - 222.0k
tinywm  - 308.0k
```

## Compilation
Dependencies (Ubuntu / Debian / Mint)
```
libxcb-keysyms1-dev
libxcb1-dev
libxcb-util0-dev
```

Build and run inside of Xephyr:
```
make
./run_sandbox
```
