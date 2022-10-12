#pragma once

#include <stdio.h>

template <typename T>
void df::Log(const T& arg) {
	printf(arg);
	printf("\n");
}

template <typename T, typename ...ARGS>
void df::Log(const T& arg, ARGS... args) {
	printf(arg, args...);
	printf("\n");
}