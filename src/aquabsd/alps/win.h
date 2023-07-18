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
	WIN_CMD_REGISTER_CB = 0x7263,
	WIN_CMD_LOOP        = 0x6C6F,
} win_cmd_t;

typedef enum {
	WIN_CB_DRAW,
	WIN_CB_RESIZE,
} win_cb_t;

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

static err_t win_set_caption(win_t* win, char* caption) {
	if (SEND_DEVICE(win->device, WIN_CMD_SET_CAPTION, win->internal_win, (uint64_t) caption) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

static err_t win_register_cb(win_t* win, win_cb_t type, int (*cb) (uint64_t _, uint64_t param), void* param) {
	if (SEND_DEVICE(win->device, WIN_CMD_REGISTER_CB, win->internal_win, type, (uint64_t) cb, (uint64_t) param) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

static err_t win_loop(win_t* win) {
	if (SEND_DEVICE(win->device, WIN_CMD_LOOP, win->internal_win) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}
