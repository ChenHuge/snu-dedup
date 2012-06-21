#pragma once

#include <string>
#include <iostream>

using namespace std;

class ChunkContainer
{
public:
	ChunkContainer(void);
	~ChunkContainer(void);

private:
	string container_name;
	FILE* fp;

public:
	bool openContainer(void);
	bool openContainer(string c_name);
	size_t writeChunk(char* chunk, int chunkSize);
	void closeContainer(void);
	fpos_t getCurPos(void);
	string getName();
};

