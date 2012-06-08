#include "StdAfx.h"
#include "ChunkContainer.h"
#include "MyString.h"


ChunkContainer::ChunkContainer(void)
{
	container_name = "";
	fp = NULL;
}


ChunkContainer::~ChunkContainer(void)
{
	if (fp != NULL) {
		fclose(fp);
		fp = NULL;
		container_name = "";
	}
}


bool ChunkContainer::openContainer(void)
{
	if (fp != NULL)   //이미 열려있는 경우 false 리턴
		return false;

	CFileFind pFind;
	BOOL bWorking = pFind.FindFile(CString("ChunkContainer\\*.*"));

	CString filename = _T("");
	while (bWorking) 
	{
		bWorking = pFind.FindNextFileW();

		if (pFind.IsDirectory())  // 디렉토리일리는 없다.
			continue;

		filename = pFind.GetFileName();
	}

	if (filename == _T("")) //ChunkContainer폴더에 아무 CC도 없을 경우
		openContainer("1");
	else
		openContainer(MyString::CString2string(filename));
}


bool ChunkContainer::openContainer(string c_name)
{
	if (fp != NULL)   //이미 열려있는 경우 false 리턴
		return false;

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

	// 컨테이너의 크기가 1GB 이상이면 닫고, 새로운 컨테이너 열기
	if (ftell(fp) >= 1024 * 1024 * 1024) {
		int cur_name_num = MyString::string2int(container_name);
		string new_name = MyString::int2string(cur_name_num + 1);
		closeContainer();
		openContainer(new_name);
	}

	size_t write_len = fwrite(chunk, sizeof(char), chunkSize, fp);  // It returns the number of character written.

	return write_len;
}


void ChunkContainer::closeContainer(void)
{
	if (fp != NULL) {
		fclose(fp);
		fp = NULL;
		container_name = "";
	}
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

string ChunkContainer::getName(void)
{
	return container_name;
}