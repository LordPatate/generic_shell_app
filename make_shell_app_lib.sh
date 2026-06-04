gcc ${DEBUG_FLAGS} -c -I headers sources/standalones/*.c sources/shell_app/shell_app.c || exit 1
ar rcs binaries/libshell_app.a *.o  || exit 1
rm *.o
