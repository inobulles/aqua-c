// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once
#define AQUABSD_BLACK_WM

#include <root.h>

#include <stdbool.h>

typedef struct {
	device_t device;
	uint64_t internal_wm;
} wm_t;

typedef enum {
	WM_CMD_CREATE      = 0x6377,
	WM_CMD_DESTROY     = 0x6463,
	WM_CMD_REGISTER_CB = 0x7263,
	WM_CMD_LOOP        = 0x6C6F,
} win_cmd_t;

typedef enum {
	TODO // TODO
} win_cb_kind_t;

typedef enum {
	WM_FLAG_NONE = 0,
	WM_FLAG_POPULATE_DRM_FD = 1 << 0,
} wm_flag_t;

AQUA_C_FN err_t wm_create(wm_t* wm, wm_flag_t flags) {
	wm->device = query_device("aquabsd.black.wm");

	if (wm->device == NO_DEVICE) {
		return ERR_NO_DEVICE;
	}

	wm->internal_wm = SEND_DEVICE(wm->device, WM_CMD_CREATE, flags);

	if (!wm->internal_wm || wm->internal_wm == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

AQUA_C_FN void win_destroy(wm_t* win) {
	SEND_DEVICE(win->device, WM_CMD_DESTROY, win->internal_wm);
}

AQUA_C_FN err_t win_register_cb(wm_t* wm, win_cb_kind_t kind, int (*cb) (uint64_t _, uint64_t param), void* param) {
	if (SEND_DEVICE(wm->device, WM_CMD_REGISTER_CB, wm->internal_wm, kind, (uint64_t) cb, (uint64_t) param) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

AQUA_C_FN err_t win_loop(wm_t* wm) {
	if (SEND_DEVICE(wm->device, WM_CMD_LOOP, wm->internal_wm) == INTERNAL_ERROR) {
		return ERR_INTERNAL;
	}

	return SUCCESS;
}
