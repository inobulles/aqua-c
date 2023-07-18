// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2023 Aymeric Wibo

#pragma once

#include <root.h>

typedef uint64_t fs_descr_t;

typedef enum {
	FS_FLAGS_READ   = 0b00001,
	FS_FLAGS_WRITE  = 0b00010,
	FS_FLAGS_APPEND = 0b01000,
	FS_FLAGS_CREATE = 0b10000,
} fs_flags_t;

typedef enum {
	FS_CMD_OPEN  = 0x6F70,
	FS_CMD_CLOSE = 0x636C,

	FS_CMD_SIZE  = 0x737A,
	FS_CMD_MMAP  = 0x6D6D,

	FS_CMD_READ  = 0x7264,
	FS_CMD_WRITE = 0x7772,
} fs_cmd_t;

#define FS_OPEN_ERR 0

static device_t fs_device = -1;

static fs_descr_t fs_open(char const* drive, char const* path, fs_flags_t flags) {
	fs_device = query_device("core.fs");

	if (fs_device == NO_DEVICE) {
		return FS_OPEN_ERR;
	}

	return SEND_DEVICE(fs_device, FS_CMD_OPEN, (uint64_t) drive, (uint64_t) path, flags);
}

static void fs_close(fs_descr_t descr) {
	SEND_DEVICE(fs_device, FS_CMD_CLOSE, descr);
}

static uint64_t fs_size(fs_descr_t descr) {
	return SEND_DEVICE(fs_device, FS_CMD_SIZE, descr);
}

static void* fs_mmap(fs_descr_t descr) {
	return (void*) SEND_DEVICE(fs_device, FS_CMD_MMAP, descr);
}

static err_t fs_read(fs_descr_t descr, void* buf, size_t len) {
	if (SEND_DEVICE(fs_device, FS_CMD_READ, descr, (uint64_t) buf, len) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

static err_t fs_write(fs_descr_t descr, void const* buf, size_t len) {
	if (SEND_DEVICE(fs_device, FS_CMD_WRITE, descr, (uint64_t) buf, len) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}
