# tinyxcb
TinyWM ported to XCB. I made this project to compare the usage (specifically for
window managers) of xcb vs Xlib. Compare the source code of TinyWM with the source code
of this project to gauge the difference between Xlib and xcb.

TinyWM: https://github.com/mackstann/tinywm

Usage is the same as TinyWM:
```
Focus follows pointer.
Alt + Left Mouse, drag: interactive window move
Alt + Right Mouse, drag: interactive window resize
Alt + F1: raise focused window
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

## Comparison (Xlib vs XCB)
xcb uses less memory and is faster than Xlib. However, xcb's documentation is basically
non-existant and the API is harder to learn (more verbose and low level). Xlib is slower 
and older, but easier to use and has more documentation. 

### SLOC (Source Lines of Code) Comparison:
Source Lines of Code is the lines of code not including line breaks and comments.
```
tinyxcb - 139 SLOC
tinywm  - 40  SLOC
```
Even if tinyxcb were written like tinywm (everything in main()), it would still be about 3x larger.

### Memory Usage of tinyxcb vs tinyWM
Measured using `smem` (PSS):
```
tinyxcb - 222.0k
tinywm  - 308.0k
```
