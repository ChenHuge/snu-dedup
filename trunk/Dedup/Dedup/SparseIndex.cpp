#include "StdAfx.h"
#include "SparseIndex.h"

#include "MyString.h"

SparseIndex::SparseIndex(void)
{
	SparseIndex(500, 10);
}

SparseIndex::SparseIndex(int entryNum_, int entrySize_)
{
	setEntryNum(entryNum_);
	setEntrySize(entrySize_);
}


SparseIndex::~SparseIndex(void)
{
}


int SparseIndex::getEntryNum(void)
{
	return 0;
}


int SparseIndex::getEntrySize(void)
{
	return 0;
}


void SparseIndex::setEntryNum(int entryNum_)
{
	entryNum = entryNum_;
}


void SparseIndex::setEntrySize(int entrySize_)
{
	entrySize = entrySize_;
}


bool SparseIndex::addIndex(string hashKey, string maniName)
{
	hash_map<string, vector<string>>::iterator iter = SITable.find(hashKey);
	
	//�̹� SITable�� �����ϴ� hashKey�� ���
	if (iter != SITable.end()) { 
		if (iter->second.size() == entrySize) {//�̹� �ش� hashKey�� ���� entry�� full�� ���
			cout << "SITable���� " << hashKey << "�� �ش��ϴ� entry�� ��á���ϴ�." << endl;
			return false;
		}
		
		iter->second.push_back(maniName);
	}
	else {
		//���� SITable�� ���������ʴ� hashKey�� ���
		if (isFull()) { //�̹� SITable�� ���� ���
			cout << "SITable�� ��á���ϴ�." << endl;
			return false;
		}

		vector<string> maniNames;
		maniNames.push_back(maniName);
		SITable.insert(hash_map<string, vector<string>>::value_type(hashKey, maniNames));
	}

	return true;
}

vector<string> SparseIndex::chooseChampions(vector<char*> hooks)
{
	vector<string> champions;

	// point �ű��
	hash_map<string, float> candidates;

	for (int i = 0 ; i < hooks.size() ; i++) 
	{
		string hook(hooks[i]);
		hash_map<string, vector<string>>::iterator iter = SITable.find(hook);
		
		if (iter == SITable.end()) // SITable�� ���� hook�� ���
			continue;
	
		for (int j = 0 ; j < iter->second.size() ; j++) {
			string maniName = iter->second[j];
			hash_map<string, float>::iterator can_iter = candidates.find(maniName);
			if (can_iter != candidates.end())
				can_iter->second += 1.0;
			else
				candidates.insert(hash_map<string, float>::value_type(maniName, 1.0));
		}
	}

	// champions ����
	for (int i = 0 ; i < hooks.size() ; i++) 
	{
		string hook(hooks[i]);
		hash_map<string, vector<string>>::iterator iter = SITable.find(hook);
		if (iter == SITable.end()) // SITable�� ���� hook�� ���
			continue;

		string champ = "";
		float max_point = 0.0;

		// hook�� �ش��ϴ� �� entry���� point�� �ִ��� maniName ã��
		for (int j = 0 ; j < iter->second.size() ; j++) {
			string maniName = iter->second[j];
			hash_map<string, float>::iterator can_iter = candidates.find(maniName);
				
			if (max_point < can_iter->second) {  
				champ = maniName;
				max_point = can_iter->second;
			}
			//���� max_point�� ���ٸ� �׳� �ռ� ����� maniName�� ���� ������ �Ѵ�.
		}

		//champ�� �̹� ������ ���� �ƴ϶�� champions�� �߰���Ű��, point�� 0.5 ������Ų��.
		bool already = false;
		for (int j = 0 ; j < champions.size() ; j++) {
			if (champions[j] == champ) {
				already = true;
				break;
			}
		}
		if (!already) {
			champions.push_back(champ);
			hash_map<string, float>::iterator can_iter = candidates.find(champ);
			can_iter->second += 0.5;
		}
	}

	return champions;
}


bool SparseIndex::isFull(void)
{
	if (entryNum >= SITable.size())
		return true;
	else
		return false;
}
