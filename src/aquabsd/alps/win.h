// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2023 Aymeric Wibo

#pragma once

#include <root.h>

typedef struct {
	device_t device;
	uint64_t internal_win;
} win_t;

typedef enum {
	WIN_CMD_CREATE      = 0x6377,
	WIN_CMD_DESTROY     = 0x6463,
	WIN_CMD_SET_CAPTION = 0x7363,
} win_cmd_t;

static err_t win_create(win_t* win, uint64_t x_res, uint64_t y_res) {
	win->device = query_device("aquabsd.alps.win");

	if (win->device == NO_DEVICE) {
		return ERR_NO_DEVICE;
	}

	win->internal_win = SEND_DEVICE(win->device, WIN_CMD_CREATE, x_res, y_res);

	if (!win->internal_win || win->internal_win == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

static void win_destroy(win_t* win) {
	SEND_DEVICE(win->device, WIN_CMD_DESTROY, win->internal_win);
}

static int win_set_caption(win_t* win, char* caption) {
	if (SEND_DEVICE(win->device, WIN_CMD_SET_CAPTION, win->internal_win, (uint64_t) caption) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}
