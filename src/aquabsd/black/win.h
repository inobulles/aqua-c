// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once
#define AQUABSD_BLACK_WIN

#include <root.h>

#include <stdbool.h>

typedef struct {
	device_t device;
	uint64_t internal_win;
} win_t;

typedef enum {
	WIN_CMD_CREATE      = 0x6377,
	WIN_CMD_DESTROY     = 0x6463,
	WIN_CMD_REGISTER_CB = 0x7263,
	WIN_CMD_LOOP        = 0x6C6F,
	WIN_CMD_GET_FB      = 0x6662,
	WIN_CMD_GET_DT_MS   = 0x6474,
	WIN_CMD_GET_X_RES   = 0x7872,
	WIN_CMD_GET_Y_RES   = 0x7972,
	WIN_CMD_SET_CAPTION = 0x7363,
} win_cmd_t;

typedef enum {
	WIN_CB_DRAW,
	WIN_CB_RESIZE,
} win_cb_kind_t;

typedef enum {
	WIN_FLAG_NONE = 0b00,
	WIN_FLAG_WITH_FB = 0b01,
	WIN_FLAG_CUSTOM_PRESENTER = 0b10,
} win_flag_t;

AQUA_C_FN err_t win_create(win_t* win, size_t x_res, size_t y_res, win_flag_t flags) {
	win->device = query_device("aquabsd.black.win");

	if (win->device == NO_DEVICE) {
		return ERR_NO_DEVICE;
	}

	win->internal_win = SEND_DEVICE(win->device, WIN_CMD_CREATE, x_res, y_res, flags);

	if (!win->internal_win || win->internal_win == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

AQUA_C_FN void win_destroy(win_t* win) {
	SEND_DEVICE(win->device, WIN_CMD_DESTROY, win->internal_win);
}

AQUA_C_FN err_t win_register_cb(win_t* win, win_cb_kind_t type, int (*cb) (uint64_t _, uint64_t param), void* param) {
	if (SEND_DEVICE(win->device, WIN_CMD_REGISTER_CB, win->internal_win, type, (uint64_t) cb, (uint64_t) param) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

AQUA_C_FN err_t win_loop(win_t* win) {
	if (SEND_DEVICE(win->device, WIN_CMD_LOOP, win->internal_win) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

AQUA_C_FN uint8_t* win_get_fb(win_t* win) {
	return (void*) SEND_DEVICE(win->device, WIN_CMD_GET_FB, win->internal_win);
}

AQUA_C_FN uint64_t win_get_dt_ms(win_t* win) {
	return SEND_DEVICE(win->device, WIN_CMD_GET_DT_MS, win->internal_win);
}

AQUA_C_FN size_t win_get_x_res(win_t* win) {
	return SEND_DEVICE(win->device, WIN_CMD_GET_X_RES, win->internal_win);
}

AQUA_C_FN size_t win_get_y_res(win_t* win) {
	return SEND_DEVICE(win->device, WIN_CMD_GET_Y_RES, win->internal_win);
}

AQUA_C_FN err_t win_set_caption(win_t* win, const char* caption) {
	if (SEND_DEVICE(win->device, WIN_CMD_SET_CAPTION, win->internal_win, (uint64_t) caption) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}
