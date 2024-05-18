
/**
 * @file build.h
 * 
 * @copyright This file's a part of the "build" project and is distributed, and
 * licensed under "MIT" license.
 * 
 * @author joba14
 * 
 * @date 2024-05-17
 */

#ifndef __build_h__
#define __build_h__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef char char_t;
typedef bool bool_t;

#define build_version_major ((uint64_t)1)
#define build_version_minor ((uint64_t)1)
#define build_version_patch ((uint64_t)0)

#define build_logger_log(_stream, _tag, _format, ...)                          \
	do                                                                         \
	{                                                                          \
		assert((_format) != NULL);                                             \
		                                                                       \
		if ((_tag) != NULL)                                                    \
		{                                                                      \
			(void)fprintf((_stream), "%s", (_tag));                            \
		}                                                                      \
		                                                                       \
		(void)fprintf((_stream), (_format), ## __VA_ARGS__);                   \
		(void)fprintf((_stream), "\n");                                        \
	} while (0)

#define build_logger_info(_format, ...)                                        \
	do                                                                         \
	{                                                                          \
		build_logger_log(stdout, "info : ", (_format), ## __VA_ARGS__);        \
	} while (0)

#define build_logger_warn(_format, ...)                                        \
	do                                                                         \
	{                                                                          \
		build_logger_log(stderr, "warn : ", (_format), ## __VA_ARGS__ );       \
	} while (0)

#define build_logger_error(_format, ...)                                       \
	do                                                                         \
	{                                                                          \
		build_logger_log(stderr, "error: ", (_format), ## __VA_ARGS__ );       \
	} while (0)

#define build_vector_initial_capacity 4
#define build_vector_capacity_multiplier 2

#define build_vector_append(_vector, _value)                                   \
	do                                                                         \
	{                                                                          \
		if ((_vector)->count >= (_vector)->capacity)                           \
		{                                                                      \
			(_vector)->capacity = (((_vector)->capacity <= 0) ?                \
				build_vector_initial_capacity :                                \
				((_vector)->capacity * build_vector_capacity_multiplier)       \
			);                                                                 \
			                                                                   \
			(_vector)->data = realloc((_vector)->data,                         \
				(_vector)->capacity * sizeof(*(_vector)->data)                 \
			);                                                                 \
			                                                                   \
			assert((_vector)->data != NULL);                                   \
		}                                                                      \
		                                                                       \
		(_vector)->data[(_vector)->count++] = (_value);                        \
	} while (0)

#define build_vector_append_many(_vector, _values, _count)                     \
	do                                                                         \
	{                                                                          \
		if (((_vector)->count + (_count)) > (_vector)->capacity)               \
		{                                                                      \
			if ((_vector)->capacity <= 0)                                      \
			{                                                                  \
				(_vector)->capacity = build_vector_initial_capacity;           \
			}                                                                  \
			                                                                   \
			while (((_vector)->count + (_count)) > (_vector)->capacity)        \
			{                                                                  \
				(_vector)->capacity *= build_vector_capacity_multiplier;       \
			}                                                                  \
			                                                                   \
			(_vector)->data = realloc((_vector)->data,                         \
				(_vector)->capacity * sizeof(*(_vector)->data)                 \
			);                                                                 \
			                                                                   \
			assert((_vector)->data != NULL);                                   \
		}                                                                      \
		                                                                       \
		(void)memcpy((_vector)->data + (_vector)->count,                       \
			(_values), (_count) * sizeof(*(_vector)->data)                     \
		);                                                                     \
		                                                                       \
		(_vector)->count += (_count);                                          \
	} while (0)

#define build_vector_drop(_vector)                                             \
	do                                                                         \
	{                                                                          \
		free((_vector)->data);                                                 \
	} while (0)

typedef struct
{
	char_t* data;
	uint64_t capacity;
	uint64_t count;
} build_string_s;

#define build_string_append(_string, ...)                                      \
	do                                                                         \
	{                                                                          \
		const char_t* cstrs[] = { __VA_ARGS__ };                               \
		const uint64_t cstrs_count = sizeof(cstrs) / sizeof(cstrs[0]);         \
		                                                                       \
		for (uint64_t index = 0; index < cstrs_count; ++index)                 \
		{                                                                      \
			const char_t* const cstr = cstrs[index];                           \
			assert(cstr != NULL);                                              \
			                                                                   \
			build_vector_append_many((_string), cstr, strlen(cstr));           \
		}                                                                      \
	} while (0)

typedef struct
{
	const char_t** data;
	uint64_t capacity;
	uint64_t count;
} build_command_s;

#define build_command_append(_command, ...)                                    \
	do                                                                         \
	{                                                                          \
		build_vector_append_many((_command),                                   \
			((const char_t*[]) { __VA_ARGS__ }),                               \
			sizeof((const char_t*[]) { __VA_ARGS__ }) / sizeof(const char_t*)  \
		);                                                                     \
	} while (0)

typedef int32_t build_proc_t;
#define build_proc_invalid ((build_proc_t)(-1))

/**
 * @brief Await async process. This effectively makes the process a synchronous
 * process.
 * 
 * @param proc process descriptor
 * 
 * @return bool_t
 */
static bool_t build_proc_await(const build_proc_t proc);

/**
 * @brief Run process asynchronously.
 * 
 * @param command command to run within the process
 * 
 * @return build_proc_t
 */
static build_proc_t build_proc_run_async(build_command_s* const command);

/**
 * @brief Run process synchronously.
 * 
 * @param command command to run within the process
 * 
 * @return bool_t
 */
static bool_t build_proc_run_sync(build_command_s* const command);

typedef struct
{
	const char_t* name;
	const char_t* description;
	bool_t(*build)(void);
} build_target_s;

typedef struct
{
	build_target_s* data;
	uint64_t capacity;
	uint64_t count;
} build_targets_s;

#define build_targets_append(_targets, ...)                                    \
	do                                                                         \
	{                                                                          \
		build_vector_append_many((_targets),                                   \
			((build_target_s[]) { __VA_ARGS__ }),                              \
			sizeof((build_target_s[]) { __VA_ARGS__ }) / sizeof(build_target_s)\
		);                                                                     \
	} while (0)

#define build_target(_name, _description)                                      \
	static bool_t _name(void);                                                 \
	                                                                           \
	static const build_target_s g_ ## _name = (const build_target_s)           \
	{                                                                          \
		.name        = #_name      ,                                           \
		.description = _description,                                           \
		.build       = _name       ,                                           \
	};                                                                         \
	                                                                           \
	static bool_t _name(void)

#define bind_target(_name)                                                     \
	(g_ ## _name)

/**
 * @brief Shift provided arguments and change the count and the offset pointer
 * accordingly.
 * 
 * @param argc arguments count
 * @param argv pointer to the arguments
 * 
 * @return const char_t*
 */
static const char_t* build_shift_args(int32_t* const argc, const char_t*** const argv);

/**
 * @brief Print the usage for the targets system.
 * 
 * @param stream  stream to print to
 * @param program the name of the program
 * @param targets targets vector
 */
static void build_targets_usage(FILE* const stream, const char_t* const program, const build_targets_s* const targets);

/**
 * @brief Parse and run (in one word - apply) the provided targets from command
 * line arguments.
 * 
 * @param targets targets vector
 * @param argc    arguments count
 * @param argv    pointer to the arguments
 */
static void build_targets_apply(build_targets_s* const targets, int32_t argc, const char_t** argv);

/**
 * @brief Move file to a new path.
 * 
 * @param old_path path to move from
 * @param new_path path to move to
 * 
 * @return bool_t
 */
static bool_t build_move_path(const char_t* const old_path, const char_t* const new_path);

/**
 * @brief Check if build needs to rebuild.
 * 
 * @param binary_path binary path
 * @param source_path source path
 * 
 * @return int32_t
 */
static int32_t build_needs_to_rebuild(const char_t* const binary_path, const char_t* const source_path);

#define rebuild_self_command(_binary_path, _source_path)                       \
	"cc",                                                                      \
	"-Wall", "-Wextra", "-Werror",                                             \
	"-o", _binary_path,                                                        \
	_source_path

#define build_rebuild_self(_argc, _argv)                                       \
	do                                                                         \
	{                                                                          \
		assert(_argc >= 1);                                                    \
		assert(_argv != NULL);                                                 \
		                                                                       \
		const char_t* const source_path = __FILE__;                            \
		const char_t* const binary_path = argv[0];                             \
		                                                                       \
		const int32_t rebuild_is_needed = build_needs_to_rebuild(              \
			binary_path, source_path                                           \
		);                                                                     \
		                                                                       \
		if (rebuild_is_needed < 0)                                             \
		{                                                                      \
			exit(1);                                                           \
		}                                                                      \
		                                                                       \
		if (rebuild_is_needed)                                                 \
		{                                                                      \
			build_string_s string = {0};                                       \
			build_string_append(&string, binary_path);                         \
			build_string_append(&string, ".old");                              \
			build_string_append(&string, "\0");                                \
			                                                                   \
			if (!build_move_path(binary_path, string.data))                    \
			{                                                                  \
				exit(1);                                                       \
			}                                                                  \
			                                                                   \
			build_command_s rebuild_command = {0};                             \
			build_command_append(&rebuild_command,                             \
				rebuild_self_command(binary_path, source_path));               \
			                                                                   \
			const bool_t rebuild_succeeded =                                   \
				build_proc_run_sync(&rebuild_command);                         \
			build_vector_drop(&rebuild_command);                               \
			                                                                   \
			if (!rebuild_succeeded)                                            \
			{                                                                  \
			    build_move_path(string.data, binary_path);                     \
			    exit(1);                                                       \
			}                                                                  \
			                                                                   \
			build_command_s command = {0};                                     \
			build_vector_append_many(&command, argv, argc);                    \
			                                                                   \
			if (!build_proc_run_sync(&command))                                \
			{                                                                  \
				exit(1);                                                       \
			}                                                                  \
			                                                                   \
			exit(0);                                                           \
		}                                                                      \
	} while (0)

#define build_targets(...)                                                     \
	int main(int32_t argc, const char_t** argv);                               \
	                                                                           \
	int main(int32_t argc, const char_t** argv)                                \
	{                                                                          \
		build_rebuild_self(argc, argv);                                        \
		build_targets_s targets = {0};                                         \
		build_targets_append(&targets, __VA_ARGS__);                           \
		build_targets_apply(&targets, argc, argv);                             \
		build_vector_drop(&targets);                                           \
		return 0;                                                              \
	}                                                                          \
	                                                                           \
	_Static_assert(1, "")

#endif  // __build_h__

#ifdef __build_c__

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>

static bool_t build_proc_await(const build_proc_t proc)
{
	if (build_proc_invalid == proc)
	{
		return false;
	}

	while (true)
	{
		int32_t status = 0;

		if (waitpid(proc, &status, 0) < 0)
		{
			build_logger_error("could not await command (pid %d): %s", proc, strerror(errno));
			return false;
		}

		if (WIFEXITED(status))
		{
			const int32_t exit_status = WEXITSTATUS(status);

			if (exit_status != 0)
			{
				build_logger_error("command exited with code %d", exit_status);
				return false;
			}

			break;
		}

		if (WIFSIGNALED(status))
		{
			build_logger_error("command process was terminated by %s", strsignal(WTERMSIG(status)));
			return false;
		}
	}

	return true;
}

static build_proc_t build_proc_run_async(build_command_s* const command)
{
	assert(command != NULL);

	if (command->count <= 0)
	{
		build_logger_error("could not run empty command");
		return build_proc_invalid;
	}

	{  // note: here the preview of the command is created and displayed.
		build_string_s preview = {0};

		for (uint64_t index = 0; index < command->count; ++index)
		{
			build_string_append(&preview, command->data[index]);

			if (index < (command->count - 1))
			{
				build_string_append(&preview, " ");
			}
			else
			{
				build_string_append(&preview, "\0");
			}
		}

		build_logger_log(stdout, "proc : ", "%s", preview.data);
		build_vector_drop(&preview);
	}

	const build_proc_t proc = fork();

	if (proc < 0)
	{
		build_logger_error("could not fork child process: %s", strerror(errno));
		return build_proc_invalid;
	}

	if (0 == proc)
	{
		if (command->data[command->count - 1] != NULL)
		{
			build_command_append(command, NULL);
		}

		if (execvp(command->data[0], (char_t* const*)command->data) < 0)
		{
			build_logger_error("could not execute child process: %s", strerror(errno));
			exit(1);
		}

		assert(0 && "should never reach this statement.");
	}

	return proc;
}

static bool_t build_proc_run_sync(build_command_s* const command)
{
	assert(command != NULL);
	const build_proc_t proc = build_proc_run_async(command);

	if (build_proc_invalid == proc)
	{
		return false;
	}

	return build_proc_await(proc);
}

static const char_t* build_shift_args(int32_t* const argc, const char_t*** const argv)
{
	assert(argc != NULL);
	assert(argv != NULL);

	const char_t* argument = NULL;

	if (*argc > 0)
	{
		argument = **argv;
		++(*argv);
		--(*argc);
	}

	return argument;
}

static void build_targets_usage(FILE* const stream, const char_t* const program, const build_targets_s* const targets)
{
	assert(stream != NULL);
	assert(program != NULL);
	assert(targets != NULL);

	build_logger_log(stream, (const char_t*)NULL, "usage: %s [options] <targets...>", program);
	build_logger_log(stream, (const char_t*)NULL, "options:");
	build_logger_log(stream, (const char_t*)NULL, "    -h, --help");
	build_logger_log(stream, (const char_t*)NULL, "        print this help message.");
	build_logger_log(stream, (const char_t*)NULL, "    -v, --version");
	build_logger_log(stream, (const char_t*)NULL, "        print the version of the build.h file in use.");

	build_logger_log(stream, (const char_t*)NULL, "targets:");
	for (uint64_t index = 0; index < targets->count; ++index)
	{
		const build_target_s* const target = &targets->data[index];

		assert(target != NULL);
		assert(target->name != NULL);
		assert(target->description != NULL);

		build_logger_log(stream, (const char_t*)NULL, "    %s", target->name);
		build_logger_log(stream, (const char_t*)NULL, "        %s", target->description);
	}
}

static void build_targets_apply(build_targets_s* const targets, int32_t argc, const char_t** argv)
{
	assert(targets != NULL);
	assert(argc > 0);
	assert(argv != NULL);

	const char_t* const program = build_shift_args(&argc, &argv);
	assert(program != NULL);

	if (argc <= 0)
	{
		build_logger_error("no targets were provided.");
		build_targets_usage(stderr, program, targets);
		exit(1);
	}

	build_targets_s selected_targets = {0};

	while (true)
	{
		const char_t* const option = build_shift_args(&argc, &argv);
		if (NULL == option) { break; }

		const uint64_t option_length = strlen(option);
		assert(option_length > 0);

		#define help_flag_short_name "-h"
		#define help_flag_long_name "--help"

		#define help_version_short_name "-v"
		#define help_version_long_name "--version"

		if ((((sizeof(help_flag_short_name) - 1) == option_length) && (strncmp(option, help_flag_short_name, option_length) == 0)) ||
			(((sizeof(help_flag_long_name)  - 1) == option_length) && (strncmp(option, help_flag_long_name , option_length) == 0)))
		{
			build_targets_usage(stdout, program, targets);
			exit(0);
		}
		else if ((((sizeof(help_version_short_name) - 1) == option_length) && (strncmp(option, help_version_short_name, option_length) == 0)) ||
				 (((sizeof(help_version_long_name)  - 1) == option_length) && (strncmp(option, help_version_long_name,  option_length) == 0)))
		{
			build_logger_log(stdout, (const char_t*)NULL, "v%lu.%lu.%lu", build_version_major, build_version_minor, build_version_patch);
			exit(0);
		}
		else
		{
			build_target_s* found_target = NULL;

			for (uint64_t index = 0; index < targets->count; ++index)
			{
				build_target_s* const current_target = &targets->data[index];
				assert(current_target != NULL);

				assert(current_target->name != NULL);
				assert(current_target->description != NULL);
				assert(current_target->build != NULL);

				const uint64_t target_name_length = strlen(current_target->name);
				assert(target_name_length > 0);

				if ((option_length == target_name_length) && (strncmp(option, current_target->name, option_length) == 0))
				{
					found_target = current_target;
					break;
				}
			}

			if (NULL == found_target)
			{
				build_logger_error("could not find a provided build target `%s` in targets vector", option);
				build_targets_usage(stderr, program, targets);
				exit(1);
			}

			build_targets_append(&selected_targets, *found_target);
		}
	}

	for (uint64_t index = 0; index < selected_targets.count; ++index)
	{
		build_target_s* const selected_target = &selected_targets.data[index];
		assert(selected_target != NULL);

		if (!selected_target->build())
		{
			build_logger_error("build target `%s` failed", selected_target->name);
			exit(1);
		}
	}

	build_vector_drop(&selected_targets);
}

static bool_t build_move_path(const char_t* const old_path, const char_t* const new_path)
{
	assert(old_path != NULL);
	assert(new_path != NULL);

	build_logger_info("moving %s -> %s", old_path, new_path);

	if (rename(old_path, new_path) < 0)
	{
		build_logger_error("could not move %s to %s: %s", old_path, new_path, strerror(errno));
		return false;
	}

	return true;
}

static int32_t build_needs_to_rebuild(const char_t* const binary_path, const char_t* const source_path)
{
	assert(binary_path != NULL);
	assert(source_path != NULL);

	struct stat statbuf = {0};

	if (stat(binary_path, &statbuf) < 0)
	{
		if (ENOENT == errno)
		{
			return 1;
		}

		build_logger_error("could not stat %s: %s", binary_path, strerror(errno));
		return -1;
	}

	const time_t binary_path_time = statbuf.st_mtime;

	if (stat(source_path, &statbuf) < 0)
	{
		build_logger_error("could not stat %s: %s", source_path, strerror(errno));
		return -1;
	}

	const time_t source_path_time = statbuf.st_mtime;

	if (source_path_time > binary_path_time)
	{
		return 1;
	}

	return 0;
}

#endif  // __build_c__
