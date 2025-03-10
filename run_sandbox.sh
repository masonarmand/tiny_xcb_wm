set -e
make clean
make
XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')
xinit ./xinitrc -- \
    "$XEPHYR" \
        :100 \
        -ac \
        -noreset \
        -screen 1280x720 \
        -host-cursor
