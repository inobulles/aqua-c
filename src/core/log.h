// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2023 Aymeric Wibo

// this file is very much analogous to the 'umber.h' header file:
// https://github.com/inobulles/umber

// I'm being hella safe here, checking alloc outputs etc etc
// last thing we want is for logging to break down!!

#pragma once

#undef LOG_COMPONENT // force user to defined this *after* header inclusion

#include <root.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum {
	LOG_LVL_FATAL,
	LOG_LVL_ERROR,
	LOG_LVL_WARN,
	LOG_LVL_SUCCESS,
	LOG_LVL_INFO,
	LOG_LVL_VERBOSE,
} log_lvl_t;

typedef enum {
	LOG_CMD_LOG = 0x6C67,
} log_cmd_t;

static device_t log_device = NO_DEVICE;
static bool log_fallback = false;

static int log_init(void) {
	if (log_device == NO_DEVICE) {
		log_device = query_device("core.log");
	}

	if (log_device == NO_DEVICE) {
		if (!log_fallback) {
			log_fallback = true;
			fprintf(stderr, "Failed to query the 'core.log' device; falling back to ugly logging\n");
		}

		return -1;
	}

	return 0;
}

// XXX this weird name is so that we don't conflict with 'log' in 'math.h' 

static void __core_log(log_lvl_t lvl, char const* component, char const* path, char const* func, uint32_t line, char const* msg) {
	if (log_init() < 0) {
		FILE* out = stderr;

		if (lvl == LOG_LVL_SUCCESS || lvl == LOG_LVL_INFO || lvl == LOG_LVL_VERBOSE) {
			out = stdout;
		}

		fprintf(out, "[%s:%d %s %s:%u] %s\n", component, lvl, path, func, line, msg);
	}

	else {
		SEND_DEVICE(log_device, LOG_CMD_LOG, lvl, (uint64_t) component, (uint64_t) path, (uint64_t) func, line, (uint64_t) msg);
	}
}

__attribute__((__format__(__printf__, 6, 0)))
static void __core_log_vlog(log_lvl_t lvl, char const* component, char const* path, char const* func, uint32_t line, char const* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	char* msg;
	vasprintf(&msg, fmt, args);

	if (!msg) {
		__core_log(LOG_LVL_FATAL, "aqua-c", __FILE__, __func__, __LINE__, "Failed to allocate space for log message");
		return;
	}

	va_end(args);

	__core_log(lvl, component, path, func, line, msg);
	free(msg);
}

// helper macros

#define LOG_FATAL(...)   __core_log_vlog(LOG_LVL_FATAL,   LOG_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)   __core_log_vlog(LOG_LVL_ERROR,   LOG_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)    __core_log_vlog(LOG_LVL_WARN,    LOG_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_SUCCESS(...) __core_log_vlog(LOG_LVL_SUCCESS, LOG_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)    __core_log_vlog(LOG_LVL_INFO,    LOG_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_VERBOSE(...) __core_log_vlog(LOG_LVL_VERBOSE, LOG_COMPONENT, __FILE__, __func__, __LINE__, __VA_ARGS__)
