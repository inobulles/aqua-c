// This Source Form is subject to the terms of the AQUA Software License, v. 1.0.
// Copyright (c) 2023 Aymeric Wibo

// we don't really do anything else than copying over headers here
// this is because this repo is really meant to be used as a dependency to other projects, not on its own

File.list("src")
	.where { |path| path.endsWith(".h") }
	.each  { |path| Resources.install(path) }
