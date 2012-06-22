#include "stdafx.h"
#include "ChunkManager.h"
#include "SHA1.h"


ChunkManager::ChunkManager(int CS, int SS) 
{
	chunkSize = CS;
	segmentSize = SS;
}

ChunkManager::~ChunkManager()
{
	int size = chunkList.size();
	for (int i = 0 ; i < size ; i++) {
		delete[] chunkList[i];
		chunkList[i] = NULL;
	}
}

/* ������ ���µ� ������ ��� ûŷ�� ���͸� ����, �ƴ� ��� �� ���͸� �����Ѵ�.
	   ��° ���ڷ� GET_CHUNK_ONLY�� �ָ� �����ϰ� ûũ��θ� �̷���� ���͸� ���,
	   ADD_INFO�� �ָ� ���Ͽ��� ���������� �о���� ���̰� ���� PUSH�� ���͸� ��´�.
	   �׷��� ADD_INFO �ɼ��� �ش� ûũ�� �ٽ� ���Ͽ� �Ἥ ������ �����ϴµ��� ���ǹǷ�,
	   �� �޼���� ������� ����. char* filename���� ���ڷ� �޴� ���� �̸��� �޼��尡
	   �������̵��Ǿ������Ƿ�, �װ��� ����϶�. �ش� �޼���� �� �޼��忡��
	   GET_CHUNK_ONLY �ɼ��� �� �Ͱ� ������ ������ �Ѵ�. */
vector<char*>* ChunkManager::getChunkList(const char* filename, int opt) {
	FILE *fp;
		
	int readLen = 0;
	int def_opt = 0;

	if(opt == ADD_INFO) {
		def_opt = opt;
	}
	else if(opt == GET_CHUNK_ONLY) {
		def_opt = opt;
	}
	else {
		printf("Bad option parameter is given.\n");
		return NULL;
	}

	fopen_s(&fp, filename, "rb");
	if(fp==NULL) {
		printf("Failed to open %s.\n", filename);
	}
	else {
		while(1) {
			char *temp = new char[chunkSize*2]; // chunkSize��ŭ�� �ӽ� ������ temp�� �Ҵ�.
			readLen = fread(temp, sizeof(char), chunkSize, fp);
				
			if(feof(fp)) {
				if(readLen>0) {
					char* LL = new char[12];
					lastLength = readLen;
					sprintf(LL, "%d", lastLength);
					chunkList.push_back(temp);
					if(def_opt == ADD_INFO)
						chunkList.push_back(LL);
				}
				break;
			}
			chunkList.push_back(temp);
		}
		fclose(fp);
	}
	return &chunkList;
}


/* ûũ����Ʈ�� �ٽ� ���Ϸ� ����Ͽ� �����Ҷ� ����Ѵ�. ��, ��ϵ� ûũ����Ʈ��
	ADD_INFO �ɼ��� ���� ������� ���� ���������� �о���� ������ ���̰� ��ϵǾ�
    �־�� �Ѵ�. �׷��� ���� ���, �� �޼��忡 ���� ���� ������ ������ ���̰�
	������ ������ ���� ������ �� ����.	*/
void ChunkManager::writeChunkList(vector<char*>* chunkList, char* filename) {
		FILE *wp;
		vector<char*>::iterator iter;
		fopen_s(&wp, filename, "wb");
		if(wp==NULL) {
			printf("Failed to create %s.\n", filename);
			return;
		}
		else {
			if(chunkList->empty()) {
				printf("List is empty.\n");
				return;
			}
			else {
				iter = chunkList->end() - 1;
				int LL = atoi(*iter);

				for(iter=chunkList->begin(); iter!=chunkList->end()-1; iter++) {
					if(iter==chunkList->end()-2)
						fwrite(*iter, sizeof(char), LL, wp);
					else
						fwrite(*iter, sizeof(char), chunkSize, wp);
				}
				fclose(wp);
			}
		}
	}


/* �����͸� �޾Ƽ� SHA-1�� ���� �ؽ� ���� ��ȯ�ϴ� �޼����̴�. */
string ChunkManager::getHashKey(char* data, bool last) 
{
	string str = "";
	str.assign(data);

	SHA1 sha;
	string hashKey = "";
	unsigned m_d[5] = {0};
	int i=0, str_len;
	sha.Reset();

	/*str_len = strlen(str.c_str());

	while(i<str_len) {
		sha.Input(str[i]);
		i++;
	}*/

	if (last)
		sha.Input(data, chunkSize);
	else
		sha.Input(data, lastLength);
		
	if(!sha.Result(m_d)) {
		printf("sha: could not compute message digest for %s\n", data);
		return (string)NULL;
	}
	else {
		sha.~SHA1();
		char temp[100] = "";
		sprintf(temp, "%08X%08X%08X%08X%08X", m_d[0], m_d[1], m_d[2], m_d[3], m_d[4]);
		hashKey.assign(temp);
		return hashKey;
	}
}

/* ���ڷ� ���޵� ����Ʈ�� ��� chunk�鿡 ���� SHA-1 �ؽ� ���� ���� ����Ʈ�� ����� ��ȯ�Ѵ�. */
vector<string> ChunkManager::getHashedList(vector<char*>* list) 
{
	vector<string> hashedList;
	int size = list->size();
	if(list->empty()) 
		printf("List is empty.\n");
	else {
		for(int i=0; i<size; i++) {
			if (i + 1 == size)
				hashedList.push_back(getHashKey((*list)[i], true));
			else
				hashedList.push_back(getHashKey((*list)[i], false));
		}
	}

	return hashedList;
}

vector<char*> ChunkManager::getSegment(vector<char*>* list, int index) {
	/* index�� 0�̸� 0~7�� ûũ.
		index�� 1�̸� 8~15�� ûũ.
		index�� 2�̸� 16~23�� ûũ.
		index�� k�̸� k*segmentSize~k*segmentSize+segmentSize-1�� ûũ. */
	vector<char*> segment;
	int size = list->size();
	if(list->empty())
		printf("List is empty.\n");
	else if(index*segmentSize >= size) {
		printf("Invalid index.\n");
	}
	else
		for(int i=index*segmentSize; i<index*segmentSize+segmentSize; i++) {
			if(i >= size) break;
			segment.push_back((*list)[i]);
		}

	return segment;
}

vector<string> ChunkManager::getSegmentForHash(vector<string>* list, int index) {
	/* index�� 0�̸� 0~7�� ûũ.
		index�� 1�̸� 8~15�� ûũ.
		index�� 2�̸� 16~23�� ûũ.
		index�� k�̸� k*segmentSize~k*segmentSize+segmentSize-1�� ûũ. */
	vector<string> segment;
	int size = list->size();
	if(list->empty())
		printf("List is empty.\n");
	else if(index*segmentSize >= size) {
		printf("Invalid index.\n");
	}
	else
		for(int i=index*segmentSize; i<index*segmentSize+segmentSize; i++) {
			if(i >= size) break;
			segment.push_back((*list)[i]);
		}

	return segment;
}

/* ���ڷ� ���� �ؽ�Ű ����Ʈ�� ���Ͽ� ���ø��� �����Ѵ�.
	   �� �� ���ø��Ǵ� Ű����, �ش� Ű�� ù n��Ʈ�� 0�� �͵��̴�.
	   �̸��׸�, AC6F...�� ù 1��Ʈ�� 0�̹Ƿ�, numOfZeroBits�� 1�� ��쿣 ���ø��ȴ�. */
vector<string> ChunkManager::getSample(vector<string>* hashedSegment, int numOfZeroBits) 
{
	vector<string> sampleList;
	vector<string>::iterator iter;
	int zeroBitCount = 0;
	if(numOfZeroBits > 8) {
		printf("Sampling rate too low.\n");
	}
	else if(numOfZeroBits < 0) {
		printf("Undeterminable sampling rate.\n");
	}
	for(iter=hashedSegment->begin(); iter!=hashedSegment->end(); iter++) {
		string temp = "";
		char t[100] = "";
		t[2] = 0;
		strncpy(t, (*iter).c_str(), 2);
		temp.assign(t);
		if(getNumOfZeroBits(temp) >= numOfZeroBits)
			sampleList.push_back(*iter);
	}
	return sampleList;
}

int ChunkManager::getNumOfZeroBits(const string str) 
{
	if(strlen(str.c_str()) != 2)
		return -1;

	char front[10] = "", back[10] = "";
	int countZeroBits = 0;

	if(str[0] == '0') strncpy_s(front, "0000", 4);
	else if(str[0] == '1') strncpy_s(front, "0001", 4);
	else if(str[0] == '2') strncpy_s(front, "0010", 4);
	else if(str[0] == '3') strncpy_s(front, "0011", 4);
	else if(str[0] == '4') strncpy_s(front, "0100", 4);
	else if(str[0] == '5') strncpy_s(front, "0101", 4);
	else if(str[0] == '6') strncpy_s(front, "0110", 4);
	else if(str[0] == '7') strncpy_s(front, "0111", 4);
	else if(str[0] == '8') strncpy_s(front, "1000", 4);
	else if(str[0] == '9') strncpy_s(front, "1001", 4);
	else if(str[0] == 'A') strncpy_s(front, "1010", 4);
	else if(str[0] == 'B') strncpy_s(front, "1011", 4);
	else if(str[0] == 'C') strncpy_s(front, "1100", 4);
	else if(str[0] == 'D') strncpy_s(front, "1101", 4);
	else if(str[0] == 'E') strncpy_s(front, "1110", 4);
	else if(str[0] == 'F') strncpy_s(front, "1111", 4);
	else return -1;

	if(str[1] == '0') strncpy_s(back, "0000", 4);
	else if(str[1] == '1') strncpy_s(back, "0001", 4);
	else if(str[1] == '2') strncpy_s(back, "0010", 4);
	else if(str[1] == '3') strncpy_s(back, "0011", 4);
	else if(str[1] == '4') strncpy_s(back, "0100", 4);
	else if(str[1] == '5') strncpy_s(back, "0101", 4);
	else if(str[1] == '6') strncpy_s(back, "0110", 4);
	else if(str[1] == '7') strncpy_s(back, "0111", 4);
	else if(str[1] == '8') strncpy_s(back, "1000", 4);
	else if(str[1] == '9') strncpy_s(back, "1001", 4);
	else if(str[1] == 'A') strncpy_s(back, "1010", 4);
	else if(str[1] == 'B') strncpy_s(back, "1011", 4);
	else if(str[1] == 'C') strncpy_s(back, "1100", 4);
	else if(str[1] == 'D') strncpy_s(back, "1101", 4);
	else if(str[1] == 'E') strncpy_s(back, "1110", 4);
	else if(str[1] == 'F') strncpy_s(back, "1111", 4);
	else return -1;

	strcat(front, back);
	for(int i=0; i<8; i++) {
		if(front[i] == '0') countZeroBits++;
		else return countZeroBits;;
	}

	return countZeroBits;
}