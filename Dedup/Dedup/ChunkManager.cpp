#include "stdafx.h"
#include "ChunkManager.h"
#include "SHA1.h"


ChunkManager::ChunkManager(int CS, int SS) 
{
	chunkSize = CS;
	segmentSize = SS;
}

/* 파일을 여는데 성공한 경우 청킹된 벡터를 리턴, 아닌 경우 빈 벡터를 리턴한다.
	   둘째 인자로 GET_CHUNK_ONLY를 주면 순수하게 청크들로만 이루어진 벡터를 얻고,
	   ADD_INFO을 주면 파일에서 마지막으로 읽어들인 길이가 끝에 PUSH된 벡터를 얻는다.
	   그러나 ADD_INFO 옵션은 해당 청크를 다시 파일에 써서 파일을 복구하는데만 사용되므로,
	   이 메서드는 사용하지 말라. char* filename만을 인자로 받는 같은 이름의 메서드가
	   오버라이딩되어있으므로, 그것을 사용하라. 해당 메서드는 이 메서드에서
	   GET_CHUNK_ONLY 옵션을 준 것과 동일한 역할을 한다. */
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
			string temp = ""; // chunkSize만큼의 임시 공간을 temp에 할당.
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

/* 인자로 들어온 청크리스트의 청크 목록을 보여준다. 옵션으로 SHOW_ALL을 주면
	청크를 실제 모습 그대로 출력하며, SHOW_ARRANGED를 주면 부분적으로 정리하여
	앞 부분만 보여준다. 인자로는 ADD_INFO을 통해 얻어진 리스트를 넣어야 올바로 출력된다.
	특별한 이유가 없으면 SHOW_ARRANGED 옵션으로 사용하기를 권한다.
	인자로 hashed list를 넣을 경우, 옵션으로 SHOW_HASHLIST를 사용하라. */

void ChunkManager::showChunkList(vector<string> chunkList, int opt) 
{
	vector<string>::iterator iter;
	int i=0;
	if(chunkList.empty()) {
		printf("List is empty.\n");
		return;
	}
	else
		// 각 청크를 하나씩 출력해준다.
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
		// 청크를 이어서 출력해준다. 원본 파일을 그대로 보고 싶을 경우 사용.
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

/* 청크리스트를 다시 파일로 기록하여 복원할때 사용한다. 단, 기록될 청크리스트는
	ADD_INFO 옵션을 통해 만들어져 끝에 마지막으로 읽어들인 파일의 길이가 기록되어
    있어야 한다. 그렇지 않을 경우, 이 메서드에 의해 새로 쓰여진 파일의 길이가
	원본과 동일할 것을 보장할 수 없다.	*/
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

/* 원본 파일명과 복사될 파일명을 인자로 주면, 원본 파일을 청킹하여 청크리스트를 만든 뒤,
	해당 청크 리스트를 복사될 파일명을 갖는 파일로 다시 써서 복구하는 메서드이다. */
void ChunkManager::copyStream(string src, string dst) 
{
	vector<string> chunkList = getChunkList(src, ADD_INFO);
	writeChunkList(chunkList, dst);
}

/* 데이터를 받아서 SHA-1에 따른 해시 값을 반환하는 메서드이다. */
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

/* 인자로 전달된 리스트의 모든 chunk들에 대한 SHA-1 해시 값을 구해 리스트를 만들어 반환한다. */
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
	/* index가 0이면 0~7번 청크.
		index가 1이면 8~15번 청크.
		index가 2이면 16~23번 청크.
		index가 k이면 k*segmentSize~k*segmentSize+segmentSize-1번 청크. */
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

/* 인자로 들어온 해시키 리스트에 대하여 샘플링을 수행한다.
	   이 때 샘플링되는 키들은, 해당 키의 첫 n비트가 0인 것들이다.
	   이를테면, AC6F...은 첫 1비트가 0이므로, numOfZeroBits가 1인 경우엔 샘플링된다. */
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