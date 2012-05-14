#include "StdAfx.h"
#include "ChunkContainer.h"


ChunkContainer::ChunkContainer(void)
{
	container_name = "";
	fp = NULL;
}


ChunkContainer::~ChunkContainer(void)
{
	if (fp != NULL)
		fclose(fp);
}



bool ChunkContainer::openContainer(string c_name)
{
	fp = fopen(c_name.c_str(), "a");
	if (fp == NULL) {
		cerr << "Failed to open Container '" << c_name << "'." << endl;
		exit(1);
	}

	container_name = c_name;

	return true;
}

size_t ChunkContainer::writeChunk(char* chunk)
{
	if (fp == NULL)  // you should call openContainer() before writeChunk()
		return -2;

	size_t write_len = fwrite(chunk, sizeof(char), strlen(chunk), fp);  // It returns the number of character written.

	return write_len;
}


void ChunkContainer::closeContainer(void)
{
	if (fp != NULL)
		fclose(fp);
}
