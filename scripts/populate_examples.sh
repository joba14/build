
# 
# @file populate_examples.sh
# 
# @copyright This file's a part of the "build" project and is distributed, and
# licensed under "MIT" license.
# 
# @author joba14
# 
# @date 2024-05-17
# 

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$SCRIPT_DIR/.."
cd $PROJECT_DIR

EXAMPLES_DIR="$PROJECT_DIR/examples"

BOOTSTRAP_FILE="bootstrap.sh"
BUILD_H_FILE="build.h"
BUILD_BIN_FILE="build.bin"
BUILD_BIN_OLD_FILE="build.bin.old"

BOOTSTRAP_FILE_PATH="$PROJECT_DIR/$BOOTSTRAP_FILE"
BUILD_H_FILE_PATH="$PROJECT_DIR/$BUILD_H_FILE"

for SUBDIR in "$EXAMPLES_DIR"/*; do
	if [ -d "$SUBDIR" ]; then
		if [ -f "$SUBDIR/$BUILD_H_FILE" ]; then
			rm "$SUBDIR/$BUILD_H_FILE"
			echo "removed $SUBDIR/$BUILD_H_FILE"
		fi

		if [ -f "$SUBDIR/$BOOTSTRAP_FILE" ]; then
			rm "$SUBDIR/$BOOTSTRAP_FILE"
			echo "removed $SUBDIR/$BOOTSTRAP_FILE"
		fi

		if [ -f "$SUBDIR/$BUILD_BIN_FILE" ]; then
			rm "$SUBDIR/$BUILD_BIN_FILE"
			echo "removed $SUBDIR/$BUILD_BIN_FILE"
		fi

		if [ -f "$SUBDIR/$BUILD_BIN_OLD_FILE" ]; then
			rm "$SUBDIR/$BUILD_BIN_OLD_FILE"
			echo "removed $SUBDIR/$BUILD_BIN_OLD_FILE"
		fi
	fi
done

for SUBDIR in "$EXAMPLES_DIR"/*; do
	if [ -d "$SUBDIR" ]; then
		cp $BOOTSTRAP_FILE_PATH $SUBDIR
		echo "copied $BOOTSTRAP_FILE_PATH to $SUBDIR"
		cp $BUILD_H_FILE_PATH $SUBDIR
		echo "copied $BUILD_H_FILE_PATH to $SUBDIR"
		chmod +x $SUBDIR/$BOOTSTRAP_FILE
	fi
done

for SUBDIR in "$EXAMPLES_DIR"/*; do
	if [ -d "$SUBDIR" ]; then
		cd $SUBDIR
		./$BOOTSTRAP_FILE
		./$BUILD_BIN_FILE -v
		./$BUILD_BIN_FILE all
	fi
done
