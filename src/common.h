#pragma once

#define FIX_STRANGE_MATH

template <typename T>
void swap(T &a, T &b) {
	T temp = a;
	a = b;
	b = temp;
}

template <typename T>
T sqr(T x) { return x*x; }