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
	if (fp != NULL)   //�̹� �����ִ� ��� false ����
		return false;

	CFileFind pFind;
	BOOL bWorking = pFind.FindFile(CString("ChunkContainer\\*.*"));

	CString filename = _T("");
	while (bWorking) 
	{
		bWorking = pFind.FindNextFileW();

		if (pFind.IsDirectory())  // ���丮�ϸ��� ����.
			continue;

		filename = pFind.GetFileName();
	}

	if (filename == _T("")) //ChunkContainer������ �ƹ� CC�� ���� ���
		return openContainer("1");
	else
		return openContainer(MyString::CString2string(filename));
}


bool ChunkContainer::openContainer(string c_name) throw (exception)
{
	if (fp != NULL)   //�̹� �����ִ� ��� false ����
		return false;

	fp = fopen(("ChunkContainer\\" + c_name).c_str(), "ab");
	if (fp == NULL) {
		string errMsg = "ERROR: Failed to open Container '" + c_name + "'.";
		throw exception(errMsg.c_str());
	}

	container_name = c_name;

	return true;
}

size_t ChunkContainer::writeChunk(string chunk, int chunkSize)
{
	if (fp == NULL)  // you should call openContainer() before writeChunk()
		return -2;

	// �����̳��� ũ�Ⱑ 1GB �̻��̸� �ݰ�, ���ο� �����̳� ����
	if (ftell(fp) >= 1024 * 1024 * 1024) {
		int cur_name_num = MyString::string2int(container_name);
		string new_name = MyString::int2string(cur_name_num + 1);
		closeContainer();
		openContainer(new_name);
	}

	char* cstr = new char[chunkSize + 1];
	strcpy(cstr, chunk.c_str());

	size_t write_len = fwrite(cstr, sizeof(char), chunkSize, fp);  // It returns the number of character written.

	delete cstr;

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


fpos_t ChunkContainer::getCurPos(void) throw (exception)
{
	if (fp == NULL) {
		throw exception("ERROR: file isn't opened. ChunkContainer::getCurPos");
	}

	fpos_t pos;
	if (fgetpos(fp, &pos) != 0) {
		throw exception("ERROR: fail to fgetpos(). ChunkContainer::getCurPos");
	}

	return pos;
}

string ChunkContainer::getName(void)
{
	return container_name;
}