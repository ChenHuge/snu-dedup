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
	fp = fopen(("ChunkContainer\\" + c_name).c_str(), "ab");
	if (fp == NULL) {
		cerr << "Failed to open Container '" << c_name << "'." << endl;
		exit(1);
	}

	container_name = c_name;

	return true;
}

size_t ChunkContainer::writeChunk(char* chunk, int chunkSize)
{
	if (fp == NULL)  // you should call openContainer() before writeChunk()
		return -2;

	size_t write_len = fwrite(chunk, sizeof(char), chunkSize, fp);  // It returns the number of character written.

	return write_len;
}


void ChunkContainer::closeContainer(void)
{
	if (fp != NULL)
		fclose(fp);
}


fpos_t ChunkContainer::getCurPos(void)
{
	if (fp == NULL) {
		cerr << "ERROR: file isn't opened. ChunkContainer::getCurPos" << endl;
		Sleep(2000);
		exit(1);
	}

	fpos_t pos;
	if (fgetpos(fp, &pos) != 0) {
		cerr << "ERROR: fail to fgetpos(). ChunkContainer::getCurPos" << endl;
		Sleep(2000);
		exit(1);
	}

	return pos;
}
