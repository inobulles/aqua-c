// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2023 Aymeric Wibo

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define NO_DEVICE ((uint64_t) -1)
#define INTERNAL_ERROR ((uint64_t) -1)

#define AQUA_C_FN static __attribute__((unused))

typedef enum {
	SUCCESS = 0,
	ERR_NO_DEVICE = -1,
	ERR_INTERNAL = -2,
} err_t;

typedef uint64_t device_t;

typedef uint64_t (*kos_query_device_t) (uint64_t _, uint64_t name);
typedef uint64_t (*kos_send_device_t) (uint64_t _, uint64_t device, uint64_t cmd, uint64_t data);

static kos_query_device_t kos_query_device = NULL;
static kos_send_device_t kos_send_device = NULL;

void aqua_set_kos_functions(kos_query_device_t _kos_query_device, kos_send_device_t _kos_send_device) {
	kos_query_device = _kos_query_device;
	kos_send_device = _kos_send_device;
}

static void must_run_with_aqua_kos(void) {
	fprintf(stderr, "You must run this program with the AQUA KOS\n");
}

AQUA_C_FN device_t query_device(const char* name) {
	if (!kos_query_device) {
		must_run_with_aqua_kos();
		return -1;
	}

	return kos_query_device(0, (uint64_t) name);
}

AQUA_C_FN uint64_t send_device(device_t device, uint16_t cmd, void* data) {
	if (!kos_send_device) {
		must_run_with_aqua_kos();
		return -1;
	}

	return kos_send_device(0, device, (uint64_t) cmd, (uint64_t) data);
}

#define SEND_DEVICE(device, cmd, ...) \
	(send_device((device), (cmd), (uint64_t[]) { __VA_ARGS__ }))

AQUA_C_FN char const* err_str(err_t err) {
	switch (err) {
	case SUCCESS:       return "success";
	case ERR_NO_DEVICE: return "no device";
	case ERR_INTERNAL:  return "internal error";
	default:            return "generic error";
	}
}

// entry symbol

int main(void);

#if !defined(NO_ENTRY)
int __native_entry(void) {
	int const rv = main();
	return rv;
}
#endif
