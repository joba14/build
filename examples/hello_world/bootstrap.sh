
# !/bin/sh

rm -fr ./build
rm -fr ./build.bin
rm -fr ./build.bin.old
gcc -Wall -Wextra -Werror -o ./build.bin ./build.c
