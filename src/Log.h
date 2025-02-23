#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define DEBUG_LOG(fmt, ...) fprintf(stdout, fmt, __VA_ARGS__)
#define ERROR_LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#define LOG(fmt, ...) fprintf(stdout, fmt, __VA_ARGS__)

#endif
