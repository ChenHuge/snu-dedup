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
vector<char*> ChunkManager::getChunkList(char* filename, int opt) 
{
	FILE *fp;
	vector<char*> chunkList;
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
		return (vector<char*>)0;
	}

	fopen_s(&fp, filename, "rb");
	if(fp==NULL) {
		printf("Failed to open %s.\n", filename);
	}
	else {
		while(1) {
			char *temp = new char[chunkSize]; // chunkSize��ŭ�� �ӽ� ������ temp�� �Ҵ�.
			readLen = fread(temp, sizeof(char), chunkSize, fp);
			temp[readLen]='\0';
			if(feof(fp)) {
				if(readLen>0) {
					char* LL = new char(12);
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
	return chunkList;
}

/* ���ڷ� ���� ûũ����Ʈ�� ûũ ����� �����ش�. �ɼ����� SHOW_ALL�� �ָ�
	ûũ�� ���� ��� �״�� ����ϸ�, SHOW_ARRANGED�� �ָ� �κ������� �����Ͽ�
	�� �κи� �����ش�. ���ڷδ� ADD_INFO�� ���� ����� ����Ʈ�� �־�� �ùٷ� ��µȴ�.
	Ư���� ������ ������ SHOW_ARRANGED �ɼ����� ����ϱ⸦ ���Ѵ�.
	���ڷ� hashed list�� ���� ���, �ɼ����� SHOW_HASHLIST�� ����϶�. */

void ChunkManager::showChunkList(vector<char*> chunkList, int opt) {
	vector<char*>::iterator iter;
	int i=0;
	if(chunkList.empty()) {
		printf("List is empty.\n");
		return;
	}
	else
		// �� ûũ�� �ϳ��� ������ش�.
		if(opt==SHOW_ARRANGED) {
			int i, size = chunkList.size();
			for(i=0; i<size-1; i++) {
				char temp[80] = "";
				strncpy_s(temp, chunkList[i], 60);
				printf("[%d] %s...\n", i, temp);
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
				char temp[80] = "";
				strncpy_s(temp, chunkList[i], 60);
				printf("[%d] %s\n", i, temp);
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
void ChunkManager::writeChunkList(vector<char*> chunkList, char* filename) {
	FILE *wp;
	vector<char*>::iterator iter;
	fopen_s(&wp, filename, "wb");
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
			int LL = atoi(*iter);

			for(iter=chunkList.begin(); iter!=chunkList.end()-1; iter++) {
				if(iter==chunkList.end()-2)
					fwrite(*iter, sizeof(char), LL, wp);
				else
					fwrite(*iter, sizeof(char), chunkSize, wp);
			}
			fclose(wp);
		}
	}
}

/* ���� ���ϸ�� ����� ���ϸ��� ���ڷ� �ָ�, ���� ������ ûŷ�Ͽ� ûũ����Ʈ�� ���� ��,
	�ش� ûũ ����Ʈ�� ����� ���ϸ��� ���� ���Ϸ� �ٽ� �Ἥ �����ϴ� �޼����̴�. */
void ChunkManager::copyStream(char* src, char* dst) {
	vector<char*> chunkList = getChunkList(src, ADD_INFO);
	writeChunkList(chunkList, dst);
}

/* �����͸� �޾Ƽ� SHA-1�� ���� �ؽ� ���� ��ȯ�ϴ� �޼����̴�. */
char* ChunkManager::getHashKey(char* data) {
	SHA1 sha;
	char* hashKey = new char[40];
	unsigned m_d[5];
	int i=0, str_len;
	sha.Reset();

	str_len = strlen(data);
	while(i<str_len) {
		sha.Input(data[i]);
		i++;
	}
		
	if(!sha.Result(m_d)) {
		printf("sha: could not compute message digest for %s\n", data);
		return (char*)NULL;
	}
	else {
		sha.~SHA1();
		sprintf(hashKey, "%08X%08X%08X%08X%08X", m_d[0], m_d[1], m_d[2], m_d[3], m_d[4]);
		return hashKey;
	}
}

/* ���ڷ� ���޵� ����Ʈ�� ��� chunk�鿡 ���� SHA-1 �ؽ� ���� ���� ����Ʈ�� ����� ��ȯ�Ѵ�. */
vector<char*> ChunkManager::getHashedList(vector<char*> list) {
	vector<char*> hashedList;
	int size = list.size();
	if(list.empty()) 
		printf("List is empty.\n");
	else 
		for(int i=0; i<size; i++)
			hashedList.push_back(getHashKey(list[i]));

	return hashedList;
}

vector<char*> ChunkManager::getSegment(vector<char*> list, int index) {
	/* index�� 0�̸� 0~7�� ûũ.
		index�� 1�̸� 8~15�� ûũ.
		index�� 2�̸� 16~23�� ûũ.
		index�� k�̸� k*segmentSize~k*segmentSize+segmentSize-1�� ûũ. */
	vector<char*> segment;
	int size = list.size();
	if(list.empty())
		printf("List is empty.\n");
	else if(index*segmentSize >= size) {
		printf("Invalid index.\n");
	}
	else
		for(int i=index*segmentSize; i<index*segmentSize+segmentSize; i++) {
			if(i >= size) break;
			segment.push_back(list[i]);
		}

	return segment;
}

