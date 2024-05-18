
/**
 * @file build.c
 * 
 * @copyright This file's a part of the "build" project and is distributed, and
 * licensed under "MIT" license.
 * 
 * @author joba14
 * 
 * @date 2024-05-17
 */

#define build_implementation
#include "build.h"

#define PROJECT_NAME "rl78emu"
#define INCLUDE_DIR  "./include"
#define SOURCE_DIR   "./source"
#define BUILD_DIR    "./build"
#define SOURCES    SOURCE_DIR"/main.c", SOURCE_DIR"/factorial.c"

#define pause (void)getchar()

#define COMPILER "cc"
#define CFLAGS "-std=gnu11", "-Wall", "-Wextra", "-Werror", "-I", INCLUDE_DIR
#define DEBUG_FLAGS "-g", "-O0", "-o", BUILD_DIR"/debug/"PROJECT_NAME
#define RELEASE_FLAGS "-O3", "-o", BUILD_DIR"/release/"PROJECT_NAME

build_target(clean, "remove the build directory with all its artefacts.")
{
	build_command_s command = {0};
	build_command_append(&command, "rm", "-fr", BUILD_DIR);
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

build_target(prep, "create build directory.")
{
	build_command_s command = {0};
	build_command_append(&command, "mkdir", BUILD_DIR, BUILD_DIR"/debug", BUILD_DIR"/release");
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

build_target(debug, "build the project in debug build configuration.")
{
	build_command_s command = {0};
	build_command_append(&command, COMPILER, CFLAGS, DEBUG_FLAGS, SOURCES);
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

build_target(release, "build the project in release build configuration.")
{
	build_command_s command = {0};
	build_command_append(&command, COMPILER, CFLAGS, RELEASE_FLAGS, SOURCES);
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

build_target(run, "run the debug build.")
{
	build_command_s command = {0};
	build_command_append(&command, BUILD_DIR"/debug/"PROJECT_NAME);
	const bool_t status = build_proc_run_sync(&command);
	build_vector_drop(&command);
	return status;
}

build_target(all, "run all targets one after the other.")
{
	return clean() && prep() && debug() && release() && run();
}

build_targets(
	bind_target(clean  ),
	bind_target(prep   ),
	bind_target(debug  ),
	bind_target(release),
	bind_target(run    ),
	bind_target(all    ),
);
