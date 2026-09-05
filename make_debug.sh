mkdir -p headers
ln -rsf sources/*/*.h headers
gcc -fsanitize=address -g -I headers sources/*/*.c examples/default_app/main.c -o debug_default_app  || exit 1
