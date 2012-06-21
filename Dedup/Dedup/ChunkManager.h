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

	/* ������ ���µ� ������ ��� ûŷ�� ���͸� ����, �ƴ� ��� �� ���͸� �����Ѵ�.
	   ��° ���ڷ� GET_CHUNK_ONLY�� �ָ� �����ϰ� ûũ��θ� �̷���� ���͸� ���,
	   ADD_INFO�� �ָ� ���Ͽ��� ���������� �о���� ���̰� ���� PUSH�� ���͸� ��´�.
	   �׷��� ADD_INFO �ɼ��� �ش� ûũ�� �ٽ� ���Ͽ� �Ἥ ������ �����ϴµ��� ���ǹǷ�,
	   �� �޼���� ������� ����. char* filename���� ���ڷ� �޴� ���� �̸��� �޼��尡
	   �������̵��Ǿ������Ƿ�, �װ��� ����϶�. �ش� �޼���� �� �޼��忡��
	   GET_CHUNK_ONLY �ɼ��� �� �Ͱ� ������ ������ �Ѵ�. */
	vector<string> getChunkList(string filename, int opt);

	/* ���ڷ� ���� ûũ����Ʈ�� ûũ ����� �����ش�. �ɼ����� SHOW_ALL�� �ָ�
	   ûũ�� ���� ��� �״�� ����ϸ�, SHOW_ARRANGED�� �ָ� �κ������� �����Ͽ�
	   �� �κи� �����ش�. ���ڷδ� ADD_INFO�� ���� ����� ����Ʈ�� �־�� �ùٷ� ��µȴ�.
	   Ư���� ������ ������ SHOW_ARRANGED �ɼ����� ����ϱ⸦ ���Ѵ�.
	   ���ڷ� hashed lischunkList, char* filename);
	   //list�� ���� ���, �ɼ����� SHOW_HASHLIST�� ����϶�. */
	void showChunkList(vector<string> chunkList, int opt);

	/* ûũ����Ʈ�� �ٽ� ���Ϸ� ����Ͽ� �����Ҷ� ����Ѵ�. ��, ��ϵ� ûũ����Ʈ��
	   ADD_INFO �ɼ��� ���� ������� ���� ���������� �о���� ������ ���̰� ��ϵǾ�
       �־�� �Ѵ�. �׷��� ���� ���, �� �޼��忡 ���� ���� ������ ������ ���̰�
	   ������ ������ ���� ������ �� ����.	*/
	void writeChunkList(vector<string> chunkList, string filename);

	/* ���� ���ϸ�� ����� ���ϸ��� ���ڷ� �ָ�, ���� ������ ûŷ�Ͽ� ûũ����Ʈ�� ���� ��,
	   �ش� ûũ ����Ʈ�� ����� ���ϸ��� ���� ���Ϸ� �ٽ� �Ἥ �����ϴ� �޼����̴�. */
	void copyStream(string src, string dst);

	/* �����͸� �޾Ƽ� SHA-1�� ���� �ؽ� ���� ��ȯ�ϴ� �޼����̴�. */
	string getHashKey(string data);

	/* ���ڷ� ���޵� ����Ʈ�� ��� chunk�鿡 ���� SHA-1 �ؽ� ���� ���� ����Ʈ�� ����� ��ȯ�Ѵ�. */
	vector<string> getHashedList(vector<string>* list);

	vector<string> getSegment(vector<string>* list, int index);

	/* ���ڷ� ���� �ؽ�Ű ����Ʈ�� ���Ͽ� ���ø��� �����Ѵ�.
	   �� �� ���ø��Ǵ� Ű����, �ش� Ű�� ù n��Ʈ�� 0�� �͵��̴�.
	   �̸��׸�, AC6F...�� ù 1��Ʈ�� 0�̹Ƿ�, numOfZeroBits�� 1�� ��쿣 ���ø��ȴ�. */
	vector<string> getSample(vector<string>* hashedSegment, int numOfZeroBits);
	int getNumOfZeroBits(const string str);
};

#endif