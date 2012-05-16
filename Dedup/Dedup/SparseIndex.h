#pragma once

#include <hash_map>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>

using namespace stdext;
using namespace std;

class SparseIndex
{
public:
	SparseIndex(void);
	SparseIndex(int entryNum_, int entrySize_);
	~SparseIndex(void);

private:
	hash_map<string, vector<string>> SITable;
	int entryNum;
	int entrySize;

public:
	int getEntryNum(void);
	int getEntrySize(void);
	void setEntryNum(int entryNum_);
	void setEntrySize(int entrySize_);
	bool addIndex(string hashKey, string maniName);
	vector<string> chooseChampions(vector<char*> hooks);
	bool isFull(void);
};

