#ifndef FILES_H
#define FILES_H
#include "lib.h"
#include "Defines.h"

extern bool write_to_file(string name, const char* data, size_t size);
extern bool read_from_file(string name, char* data, size_t size);

#endif // !FILES_H