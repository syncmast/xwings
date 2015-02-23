cd /home/user/Projects/xwings/examples/offscreen/
gcc -DXK_MISCELLANY -std=c99 -g -Wall -Werror -lX11 -lXrender -lXcomposite \
x11-offscreen-redirection.c -o x11-offscreen-redirection
