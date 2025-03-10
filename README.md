# tinyxcb
TinyWM ported to xcb. I made this project to learn the differences between Xlib
and xcb and to decide on which one I should use for future projects.

Why xcb instead of Xlib?  
 - xcb is fully asynchronous (faster than xlib)
 - xcb is newer and has a much smaller codebase
 - lower memory usage
 - lots more (read here: https://www.x.org/wiki/guide/xlib-and-xcb/)

Why Xlib instead of xcb?
 - For some use cases the performance advantages of xcb over xlib are minimal
 - Xlib is a lot easier to learn (in my opinion), and actually has documentation.
 - Xlib has a lot of high level data structures/functions to make it easier,
 unlike xcb which is a lot more low level.

xcb might seem to be an obvious choice for your next project, however it isn't
exactly easy to use. xcb is extremely verbose, lower level, and poorly documented.
Xlib's documentation is not too great, but when compared to xcb it's infinitely better
(because it actually exists).

## example of xcb verbosity
In Xlib lots of the types and functions have simple and obvious names.
For example the event struct is called `XEvent`. To get a button event you do this:
```C
XEvent event;
XButtonEvent button_event = event.xbutton;
```

In xcb the types and functions are extremely long. In xcb here is how you get a 
button event:
```C
xcb_generic_event_t* event;
xcb_button_press_event_t* button_event = (xcb_button_press_event_t*) event;
```
Compare the source code of tinyxcb with TinyWM (https://github.com/mackstann/tinywm)
to guage the difference between the verbosity of the two libraries.
Note: because of the verbosity of xcb, I broke down a lot of the functionality into
different functions. Otherwise it would've been difficult to read.


Although I am not personally a fan of the verbosity of xcb, I will say I like that
the functions and types are prefaced by `xcb`. In Xlib you have almost everything
prefaced by `X` which is fine... except some things aren't (like Window, and Display).
In my opinion this makes things confusing when reading other peoples source code because
you get their custom types mixed in with Xlib's types. For example in dwm you might see
these types used in a function:
```
Monitor
Window
Display
```
This is confusing and would be a lot nicer if these typedefs were prefixed by X
```
Monitor  /* custom type */
XWindow  /* Xlib type */
XDisplay /* Xlib type */
```
Why Xlib doesn't do this is beyond me.

## SLOC (Source Lines of Code) Comparison:
Source Lines of Code is the lines of code not including line breaks and comments.
Tinyxcb SLOC count could be reduced if everything was in main() with no extra data
structures like tinywm. However, the SLOC would probably still be around 3x more than
tinywm.
```
tinyxcb - 150 SLOC
tinywm  - 40  SLOC
```

## Memory Usage of tinyxcb vs tinyWM
This is not a definite measurement, as memory usage is sort of hard to accurately
measure. For this I used the tool called `smem`. As you can see below, even with
this minimal example, xcb still uses less memory.
```
tinyxcb - 222.0k
tinywm  - 308.0k
```

## Compilation
To compile you need the following libraries (apt):
```
libxcb-keysyms1-dev
libxcb1-dev
libxcb-util0-dev
```

Build and run (inside of Xephyr):
```
make
./run_sandbox
```
