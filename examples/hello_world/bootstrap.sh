
# 
# @file bootstrap.sh
# 
# @copyright This file's a part of the "build" project and is distributed, and
# licensed under "MIT" license.
# 
# @author joba14
# 
# @date 2024-05-17
# 

rm -fr ./build
rm -fr ./build.bin
rm -fr ./build.bin.old
gcc -I./../../ -Wall -Wextra -Werror -o ./build.bin ./build.c
