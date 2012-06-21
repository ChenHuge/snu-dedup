#ifndef _CHUNK_MANAGER_H_
#define _CHUNK_MANAGER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>

#define SHOW_HASHLIST 2
#define SHOW_ALL 1
#define SHOW_ARRANGED 0
#define ADD_INFO 1
#define GET_CHUNK_ONLY 0

using namespace std;

class ChunkManager {
private:
	int chunkSize;
	int segmentSize;
	int lastLength;
public:
	char* asdf(void);
/*	enum {GET_CHUNK_ONLY  = 0, ADD_INFO = 1,
		SHOW_ARRANGED = 0, SHOW_ALL = 1}; */
	ChunkManager(int CS, int SS);

	/* 파일을 여는데 성공한 경우 청킹된 벡터를 리턴, 아닌 경우 빈 벡터를 리턴한다.
	   둘째 인자로 GET_CHUNK_ONLY를 주면 순수하게 청크들로만 이루어진 벡터를 얻고,
	   ADD_INFO을 주면 파일에서 마지막으로 읽어들인 길이가 끝에 PUSH된 벡터를 얻는다.
	   그러나 ADD_INFO 옵션은 해당 청크를 다시 파일에 써서 파일을 복구하는데만 사용되므로,
	   이 메서드는 사용하지 말라. char* filename만을 인자로 받는 같은 이름의 메서드가
	   오버라이딩되어있으므로, 그것을 사용하라. 해당 메서드는 이 메서드에서
	   GET_CHUNK_ONLY 옵션을 준 것과 동일한 역할을 한다. */
	vector<string> getChunkList(string filename, int opt);

	/* 인자로 들어온 청크리스트의 청크 목록을 보여준다. 옵션으로 SHOW_ALL을 주면
	   청크를 실제 모습 그대로 출력하며, SHOW_ARRANGED를 주면 부분적으로 정리하여
	   앞 부분만 보여준다. 인자로는 ADD_INFO을 통해 얻어진 리스트를 넣어야 올바로 출력된다.
	   특별한 이유가 없으면 SHOW_ARRANGED 옵션으로 사용하기를 권한다.
	   인자로 hashed lischunkList, char* filename);
	   //list를 넣을 경우, 옵션으로 SHOW_HASHLIST를 사용하라. */
	void showChunkList(vector<string> chunkList, int opt);

	/* 청크리스트를 다시 파일로 기록하여 복원할때 사용한다. 단, 기록될 청크리스트는
	   ADD_INFO 옵션을 통해 만들어져 끝에 마지막으로 읽어들인 파일의 길이가 기록되어
       있어야 한다. 그렇지 않을 경우, 이 메서드에 의해 새로 쓰여진 파일의 길이가
	   원본과 동일할 것을 보장할 수 없다.	*/
	void writeChunkList(vector<string> chunkList, string filename);

	/* 원본 파일명과 복사될 파일명을 인자로 주면, 원본 파일을 청킹하여 청크리스트를 만든 뒤,
	   해당 청크 리스트를 복사될 파일명을 갖는 파일로 다시 써서 복구하는 메서드이다. */
	void copyStream(string src, string dst);

	/* 데이터를 받아서 SHA-1에 따른 해시 값을 반환하는 메서드이다. */
	string getHashKey(string data);

	/* 인자로 전달된 리스트의 모든 chunk들에 대한 SHA-1 해시 값을 구해 리스트를 만들어 반환한다. */
	vector<string> getHashedList(vector<string>* list);

	vector<string> getSegment(vector<string>* list, int index);

	/* 인자로 들어온 해시키 리스트에 대하여 샘플링을 수행한다.
	   이 때 샘플링되는 키들은, 해당 키의 첫 n비트가 0인 것들이다.
	   이를테면, AC6F...은 첫 1비트가 0이므로, numOfZeroBits가 1인 경우엔 샘플링된다. */
	vector<string> getSample(vector<string>* hashedSegment, int numOfZeroBits);
	int getNumOfZeroBits(const string str);
};

#endif