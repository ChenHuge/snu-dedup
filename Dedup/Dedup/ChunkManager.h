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
/*	enum {GET_CHUNK_ONLY  = 0, ADD_INFO = 1,
		SHOW_ARRANGED = 0, SHOW_ALL = 1}; */
	ChunkManager(int CS, int SS);

	/* ������ ���µ� ������ ��� ûŷ�� ���͸� ����, �ƴ� ��� �� ���͸� �����Ѵ�.
	   ��° ���ڷ� GET_CHUNK_ONLY�� �ָ� �����ϰ� ûũ��θ� �̷���� ���͸� ���,
	   ADD_INFO�� �ָ� ���Ͽ��� ���������� �о���� ���̰� ���� PUSH�� ���͸� ��´�.
	   �׷��� ADD_INFO �ɼ��� �ش� ûũ�� �ٽ� ���Ͽ� �Ἥ ������ �����ϴµ��� ���ǹǷ�,
	   �� �޼���� ������� ����. char* filename���� ���ڷ� �޴� ���� �̸��� �޼��尡
	   �������̵��Ǿ������Ƿ�, �װ��� ����϶�. �ش� �޼���� �� �޼��忡��
	   GET_CHUNK_ONLY �ɼ��� �� �Ͱ� ������ ������ �Ѵ�. */
	vector<char*> getChunkList(const char* filename, int opt);

	/* ���ڷ� ���� ûũ����Ʈ�� ûũ ����� �����ش�. �ɼ����� SHOW_ALL�� �ָ�
	   ûũ�� ���� ��� �״�� ����ϸ�, SHOW_ARRANGED�� �ָ� �κ������� �����Ͽ�
	   �� �κи� �����ش�. ���ڷδ� ADD_INFO�� ���� ����� ����Ʈ�� �־�� �ùٷ� ��µȴ�.
	   Ư���� ������ ������ SHOW_ARRANGED �ɼ����� ����ϱ⸦ ���Ѵ�.
	   ���ڷ� hashed lischunkList, char* filename);

	/* ���� ���ϸ�� ����� ���ϸ��� ���ڷ� �ָ�, ���� ������ ûŷ�Ͽ� ûũ����Ʈ�� ���� ��,
	   �ش� ûũ ����Ʈ�� ����� ���ϸ��� ���� ���Ϸ� �ٽ� �Ἥ �����ϴ� �޼����̴�. */
	void copyStream(char* src, char* dst);

	//list�� ���� ���, �ɼ����� SHOW_HASHLIST�� ����϶�. */
	void showChunkList(vector<char*> chunkList, int opt);

	/* ûũ����Ʈ�� �ٽ� ���Ϸ� ����Ͽ� �����Ҷ� ����Ѵ�. ��, ��ϵ� ûũ����Ʈ��
	   ADD_INFO �ɼ��� ���� ������� ���� ���������� �о���� ������ ���̰� ��ϵǾ�
       �־�� �Ѵ�. �׷��� ���� ���, �� �޼��忡 ���� ���� ������ ������ ���̰�
	   ������ ������ ���� ������ �� ����.	*/
	void writeChunkList(vector<char*> chunkList, char* filename);

	/* �����͸� �޾Ƽ� SHA-1�� ���� �ؽ� ���� ��ȯ�ϴ� �޼����̴�. */
	char* getHashKey(char* data);

	/* ���ڷ� ���޵� ����Ʈ�� ��� chunk�鿡 ���� SHA-1 �ؽ� ���� ���� ����Ʈ�� ����� ��ȯ�Ѵ�. */
	vector<char*> getHashedList(vector<char*>* list);

	vector<char*> getSegment(vector<char*>* list, int index);

	vector<char*> getSample(vector<char*>* hashedSegment, int numOfZeroBits);
	int getNumOfZeroBits(const char* str);
};

#endif