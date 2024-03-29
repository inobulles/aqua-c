// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2024 Aymeric Wibo

#pragma once

#include <root.h>

typedef uint64_t win_t;

static device_t win_device = NO_DEVICE;

static int win_init(void) {
	win_device = query_device("aquabsd.black.win");

	if (win_device == NO_DEVICE) {
		return ERR_NO_DEVICE;
	}

	return SUCCESS;
}

static win_t win_create(uint64_t width, uint64_t height) {
	return SEND_DEVICE(win_device, 0x6377, width, height);
}

static void win_destroy(win_t win) {
	SEND_DEVICE(win_device, 0x6477, win);
}
