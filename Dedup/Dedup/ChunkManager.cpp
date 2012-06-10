#include "stdafx.h"
#include "ChunkManager.h"
#include "SHA1.h"


ChunkManager::ChunkManager(int CS, int SS) 
{
	chunkSize = CS;
	segmentSize = SS;
}

/* ������ ���µ� ������ ��� ûŷ�� ���͸� ����, �ƴ� ��� �� ���͸� �����Ѵ�.
	   ��° ���ڷ� GET_CHUNK_ONLY�� �ָ� �����ϰ� ûũ��θ� �̷���� ���͸� ���,
	   ADD_INFO�� �ָ� ���Ͽ��� ���������� �о���� ���̰� ���� PUSH�� ���͸� ��´�.
	   �׷��� ADD_INFO �ɼ��� �ش� ûũ�� �ٽ� ���Ͽ� �Ἥ ������ �����ϴµ��� ���ǹǷ�,
	   �� �޼���� ������� ����. char* filename���� ���ڷ� �޴� ���� �̸��� �޼��尡
	   �������̵��Ǿ������Ƿ�, �װ��� ����϶�. �ش� �޼���� �� �޼��忡��
	   GET_CHUNK_ONLY �ɼ��� �� �Ͱ� ������ ������ �Ѵ�. */
vector<string> ChunkManager::getChunkList(string filename, int opt) 
{
	FILE *fp = NULL;
	vector<string> chunkList;
	chunkList.clear();
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
		return (vector<string>)0;
	}

	fopen_s(&fp, filename.c_str(), "rb");
	if(fp==NULL) {
		cout << "Failed to open " << filename << "." << endl;
	}
	else {
		while(1) {
			char* t = new char[chunkSize+1];
			strcpy(t, "");
			string temp = ""; // chunkSize��ŭ�� �ӽ� ������ temp�� �Ҵ�.
			readLen = fread(t, sizeof(char), chunkSize, fp);
			t[readLen] = '\0';
			temp.assign(t);

			if(feof(fp)) {
				if(readLen>0) {
					string LL = "";
					char t[10] = "";
					lastLength = readLen;

					sprintf(t, "%d", lastLength);
					LL.assign(t);
					chunkList.push_back(temp);
					if(def_opt == ADD_INFO)
						chunkList.push_back(LL);
				}
				delete [chunkSize]t;
				break;
			}
			chunkList.push_back(temp);
			delete [chunkSize]t;
		}
		fclose(fp);
	}
	return chunkList;
}

/* ���ڷ� ���� ûũ����Ʈ�� ûũ ����� �����ش�. �ɼ����� SHOW_ALL�� �ָ�
	ûũ�� ���� ��� �״�� ����ϸ�, SHOW_ARRANGED�� �ָ� �κ������� �����Ͽ�
	�� �κи� �����ش�. ���ڷδ� ADD_INFO�� ���� ����� ����Ʈ�� �־�� �ùٷ� ��µȴ�.
	Ư���� ������ ������ SHOW_ARRANGED �ɼ����� ����ϱ⸦ ���Ѵ�.
	���ڷ� hashed list�� ���� ���, �ɼ����� SHOW_HASHLIST�� ����϶�. */

void ChunkManager::showChunkList(vector<string> chunkList, int opt) 
{
	vector<string>::iterator iter;
	int i=0;
	if(chunkList.empty()) {
		printf("List is empty.\n");
		return;
	}
	else
		// �� ûũ�� �ϳ��� ������ش�.
		if(opt==SHOW_ARRANGED) {
			int size = chunkList.size();
			for(int i=0; i<size-1; i++) {
				string temp = "";
				char t[100] = "";
				strncpy(t, chunkList[i].c_str(), 60);
				temp.assign(t);
				cout << "[" << i << "] " << temp << "..." << endl;
			}
		//	printf("\nSIZE OF THE LAST CHUNK(index %d) : %s.\nCHUNKSIZE: %d\n", i-1, chunkList[i], chunkSize);
			printf("Press enter.\n");
			getchar();
		}
		// ûũ�� �̾ ������ش�. ���� ������ �״�� ���� ���� ��� ���.
		else if(opt==SHOW_ALL) {
			for(iter=chunkList.begin(); iter!=chunkList.end(); iter++)
				printf("%s", *iter);
			printf("\nPress enter.\n");
			getchar();
		}
		else if(opt==SHOW_HASHLIST) {
			int i, size = chunkList.size();
			for(i=0; i<size; i++) {
				string temp = "";
				char t[100] = "";
				strncpy(t, chunkList[i].c_str(), 60);
				temp.assign(t);
				cout << "[" << i << "] " << temp << endl;
			}
			printf("Press enter.\n");
			getchar();
		}
		else {
			printf("Option parameter should be given.\n");
			return;
		}
}

/* ûũ����Ʈ�� �ٽ� ���Ϸ� ����Ͽ� �����Ҷ� ����Ѵ�. ��, ��ϵ� ûũ����Ʈ��
	ADD_INFO �ɼ��� ���� ������� ���� ���������� �о���� ������ ���̰� ��ϵǾ�
    �־�� �Ѵ�. �׷��� ���� ���, �� �޼��忡 ���� ���� ������ ������ ���̰�
	������ ������ ���� ������ �� ����.	*/
void ChunkManager::writeChunkList(vector<string> chunkList, string filename) 
{
	FILE *wp = NULL;
	vector<string>::iterator iter;
	fopen_s(&wp, filename.c_str(), "wb");
	if(wp==NULL) {
		printf("Failed to create %s.\n", filename);
		return;
	}
	else {
		if(chunkList.empty()) {
			printf("List is empty.\n");
			return;
		}
		else {
			iter = chunkList.end() - 1;
			int LL = atoi((*iter).c_str());

			for(iter=chunkList.begin(); iter!=chunkList.end()-1; iter++) {
				if(iter==chunkList.end()-2)
					fwrite((*iter).c_str(), sizeof(char), LL, wp);
				else
					fwrite((*iter).c_str(), sizeof(char), chunkSize, wp);
			}
			fclose(wp);
		}
	}
}

/* ���� ���ϸ�� ����� ���ϸ��� ���ڷ� �ָ�, ���� ������ ûŷ�Ͽ� ûũ����Ʈ�� ���� ��,
	�ش� ûũ ����Ʈ�� ����� ���ϸ��� ���� ���Ϸ� �ٽ� �Ἥ �����ϴ� �޼����̴�. */
void ChunkManager::copyStream(string src, string dst) 
{
	vector<string> chunkList = getChunkList(src, ADD_INFO);
	writeChunkList(chunkList, dst);
}

/* �����͸� �޾Ƽ� SHA-1�� ���� �ؽ� ���� ��ȯ�ϴ� �޼����̴�. */
string ChunkManager::getHashKey(string data) 
{
	SHA1 sha;
	string hashKey = "";
	unsigned m_d[5] = {0};
	int i=0, str_len;
	sha.Reset();

	str_len = strlen(data.c_str());
	while(i<str_len) {
		sha.Input(data[i]);
		i++;
	}
		
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
vector<string> ChunkManager::getHashedList(vector<string>* list) 
{
	vector<string> hashedList;
	hashedList.clear();
	int size = list->size();
	if(list->empty()) 
		printf("List is empty.\n");
	else 
		for(int i=0; i<size; i++)
			hashedList.push_back(getHashKey((*list)[i]));

	return hashedList;
}

vector<string> ChunkManager::getSegment(vector<string>* list, int index) 
{
	/* index�� 0�̸� 0~7�� ûũ.
		index�� 1�̸� 8~15�� ûũ.
		index�� 2�̸� 16~23�� ûũ.
		index�� k�̸� k*segmentSize~k*segmentSize+segmentSize-1�� ûũ. */
	vector<string> segment;
	segment.clear();
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
	sampleList.clear();
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