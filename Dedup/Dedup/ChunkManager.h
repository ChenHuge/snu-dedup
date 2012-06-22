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
	vector<char*> chunkList;

public:
/*	enum {GET_CHUNK_ONLY  = 0, ADD_INFO = 1,
		SHOW_ARRANGED = 0, SHOW_ALL = 1}; */
	ChunkManager(int CS, int SS);
	~ChunkManager();

	/* ������ ���µ� ������ ��� ûŷ�� ���͸� ����, �ƴ� ��� �� ���͸� �����Ѵ�.
	   ��° ���ڷ� GET_CHUNK_ONLY�� �ָ� �����ϰ� ûũ��θ� �̷���� ���͸� ���,
	   ADD_INFO�� �ָ� ���Ͽ��� ���������� �о���� ���̰� ���� PUSH�� ���͸� ��´�.
	   �׷��� ADD_INFO �ɼ��� �ش� ûũ�� �ٽ� ���Ͽ� �Ἥ ������ �����ϴµ��� ���ǹǷ�,
	   �� �޼���� ������� ����. char* filename���� ���ڷ� �޴� ���� �̸��� �޼��尡
	   �������̵��Ǿ������Ƿ�, �װ��� ����϶�. �ش� �޼���� �� �޼��忡��
	   GET_CHUNK_ONLY �ɼ��� �� �Ͱ� ������ ������ �Ѵ�. */
	vector<char*>* getChunkList(const char* filename, int opt);

	/* ûũ����Ʈ�� �ٽ� ���Ϸ� ����Ͽ� �����Ҷ� ����Ѵ�. ��, ��ϵ� ûũ����Ʈ��
	   ADD_INFO �ɼ��� ���� ������� ���� ���������� �о���� ������ ���̰� ��ϵǾ�
       �־�� �Ѵ�. �׷��� ���� ���, �� �޼��忡 ���� ���� ������ ������ ���̰�
	   ������ ������ ���� ������ �� ����.	*/
	void writeChunkList(vector<char*>* chunkList, char* filename);

	/* �����͸� �޾Ƽ� SHA-1�� ���� �ؽ� ���� ��ȯ�ϴ� �޼����̴�. */
	string getHashKey(char* data, bool last);

	/* ���ڷ� ���޵� ����Ʈ�� ��� chunk�鿡 ���� SHA-1 �ؽ� ���� ���� ����Ʈ�� ����� ��ȯ�Ѵ�. */
	vector<string> getHashedList(vector<char*>* list);

	vector<char*> getSegment(vector<char*>* list, int index);
	vector<string> getSegmentForHash(vector<string>* list, int index);

	/* ���ڷ� ���� �ؽ�Ű ����Ʈ�� ���Ͽ� ���ø��� �����Ѵ�.
	   �� �� ���ø��Ǵ� Ű����, �ش� Ű�� ù n��Ʈ�� 0�� �͵��̴�.
	   �̸��׸�, AC6F...�� ù 1��Ʈ�� 0�̹Ƿ�, numOfZeroBits�� 1�� ��쿣 ���ø��ȴ�. */
	vector<string> getSample(vector<string>* hashedSegment, int numOfZeroBits);
	int getNumOfZeroBits(const string str);
};

#endif