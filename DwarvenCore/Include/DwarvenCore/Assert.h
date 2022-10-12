#pragma once

#include <assert.h>
#include <iostream>

#define DFAssert(expr, text) if(!(expr)) {						\
	std::cout << "============== ASSERT ==============\n";		\
	std::cout << text << "\n";									\
	std::cout << "====================================\n";		\
	assert(false);												\
}


#define DFCompileAssert(expr, text) {static_assert(expr, text);}