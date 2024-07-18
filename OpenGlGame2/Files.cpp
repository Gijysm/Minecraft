#include "Files.h"

bool write_to_file(string name, const char* data, size_t size)
{
    ofstream ofile(name, ios::binary);
    if (ofile.is_open())
	{
		ofile.write(data, size);
		ofile.close();
		return true;
	}
    return false;
}

bool read_from_file(string name, char* data, size_t size)
{
	ifstream ifile(name, ios::binary);
	if (ifile.is_open())
	{
		ifile.read(data, size);
		ifile.close();
		return true;
	}
	return false;
}
