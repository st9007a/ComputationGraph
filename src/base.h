#ifndef BASE_H
#define BASE_H

#define FATAL(fmt, ...)             \
	do {                            \
		printf(fmt, ##__VA_ARGS__); \
		exit(1);                    \
	} while(0);

#define MEMORY_EXHAUSTED_ERROR "Memory Exhausted Error"
#define RESOURCE_EXHAUSTED_ERROR "Resource Exhausted Error"
#define DUPLICATE_NAMED_ERROR "Duplicate Named Error"
#define UNEXPECTED_SHAPE_ERROR "Unexpected Shape Error"
#define FEED_DATA_ERROR "Feed Data Error"

#endif
