gcc -g -I headers sources/shell_app/shell_app.c sources/standalones/*.c examples/default_app.c  || exit 1
gdb a.out
