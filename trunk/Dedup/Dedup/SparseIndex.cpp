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
	
	//이미 SITable에 존재하는 hashKey인 경우
	if (iter != SITable.end()) { 
		if (iter->second.size() == entrySize) {//이미 해당 hashKey에 대한 entry가 full인 경우
			cout << "SITable에서 " << hashKey << "에 해당하는 entry가 꽉찼습니다." << endl;
			return false;
		}
		
		iter->second.push_back(maniName);
	}
	else {
		//아직 SITable에 존재하지않는 hashKey인 경우
		if (isFull()) { //이미 SITable이 꽉찬 경우
			cout << "SITable이 꽉찼습니다." << endl;
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

	// point 매기기
	hash_map<string, float> candidates;

	for (int i = 0 ; i < hooks.size() ; i++) 
	{
		string hook(hooks[i]);
		hash_map<string, vector<string>>::iterator iter = SITable.find(hook);
		
		if (iter == SITable.end()) // SITable에 없는 hook인 경우
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

	// champions 고르기
	for (int i = 0 ; i < hooks.size() ; i++) 
	{
		string hook(hooks[i]);
		hash_map<string, vector<string>>::iterator iter = SITable.find(hook);
		if (iter == SITable.end()) // SITable에 없는 hook인 경우
			continue;

		string champ = "";
		float max_point = 0.0;

		// hook에 해당하는 한 entry에서 point가 최대인 maniName 찾기
		for (int j = 0 ; j < iter->second.size() ; j++) {
			string maniName = iter->second[j];
			hash_map<string, float>::iterator can_iter = candidates.find(maniName);
				
			if (max_point < can_iter->second) {  
				champ = maniName;
				max_point = can_iter->second;
			}
			//만약 max_point와 같다면 그냥 앞서 검출된 maniName을 고르는 것으로 한다.
		}

		//champ이 이미 선정된 놈이 아니라면 champions에 추가시키고, point를 0.5 증가시킨다.
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
