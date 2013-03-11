#ifndef WIN32_H
#define WIN32_H

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN 1
	namespace Windows {
	#include <Windows.h>
	}
#endif

#endif