#include "StdAfx.h"
#include "SparseIndex.h"
#include <fstream>
#include "MyString.h"

using namespace std;


SparseIndex::SparseIndex(void)
{
	setEntryNum(500);
	setEntrySize(10);
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
	hash_map<string, list<string>>::iterator iter = SITable.find(hashKey);
	
	//이미 SITable에 존재하는 hashKey인 경우
	if (iter != SITable.end()) { 
		if (iter->second.size() == entrySize) {//이미 해당 hashKey에 대한 entry가 full인 경우
			/*cout << "SITable에서 " << hashKey << "에 해당하는 entry가 꽉찼습니다." << endl;
			return false;*/

			//list의 맨 앞 요소 제거
			iter->second.pop_front();
		}
		
		iter->second.push_back(maniName);
	}
	else {
		//아직 SITable에 존재하지않는 hashKey인 경우
		if (isFull()) { //이미 SITable이 꽉찬 경우
			/*cout << "SITable이 꽉찼습니다." << endl;
			return false;*/
			
			//SITable의 맨 앞 엔트리 제거
			SITable.erase(SITable.begin());
		}

		list<string> maniNames;
		maniNames.push_back(maniName);
		SITable.insert(hash_map<string, list<string>>::value_type(hashKey, maniNames));
	}

	return true;
}

vector<string> SparseIndex::chooseChampions(vector<string> hooks, int maxNumChamp)
{
	vector<string> champions;

	// point 매기기
	hash_map<string, float> candidates;

	for (int i = 0 ; i < hooks.size() ; i++) 
	{
		hash_map<string, list<string>>::iterator iter = SITable.find(hooks[i]);
		
		if (iter == SITable.end()) // SITable에 없는 hook인 경우
			continue;
	
		list<string>::iterator list_iter = iter->second.begin();
		for (list_iter = iter->second.begin() ; list_iter != iter->second.end() ; list_iter++) {
			string maniName = *list_iter;
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
		// MaxNumChampion을 초과햇다면 그만!
		if (champions.size() >= maxNumChamp)
			break;

		hash_map<string, list<string>>::iterator iter = SITable.find(hooks[i]);
		if (iter == SITable.end()) // SITable에 없는 hook인 경우
			continue;

		string champ = "";
		float max_point = 0.0;

		// hook에 해당하는 한 entry에서 point가 최대인 maniName 찾기
		list<string>::iterator list_iter = iter->second.begin();
		for (list_iter = iter->second.begin() ; list_iter != iter->second.end() ; list_iter++) {
			string maniName = *list_iter;
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
	if (entryNum <= SITable.size())
		return true;
	else
		return false;
}


bool SparseIndex::save(void)
{
	ofstream fout("SparseIndex", ios::out);
	if (!fout)
		return false;

	fout << entryNum << endl;
	fout << entrySize << endl;
	fout << SITable.size() << endl;

	hash_map<string, list<string>>::iterator iter = SITable.begin();
	for (iter = SITable.begin() ; iter != SITable.end() ; iter++) {
		fout << iter->first << " ";
		fout << iter->second.size() << " ";

		list<string>::iterator list_iter = iter->second.begin();
		for (list_iter = iter->second.begin() ; list_iter != iter->second.end() ; list_iter++) {
			fout << *list_iter << " ";
		}

		fout << endl;
	}

	fout.close();
}


bool SparseIndex::load(void)
{
	ifstream fin("SparseIndex", ios::in);
	if (!fin)
		return false;

	int tableSize = 0;
	
	fin >> entryNum;
	fin >> entrySize;
	fin >> tableSize;

	for (int i = 0 ; i < tableSize ; i++) {
		string hashKey;
		int listSize = 0;
		list<string> maninames;

		fin >> hashKey;
		fin >> listSize;
		
		for (int j = 0 ; j < listSize ; j++) {
			string mani;
			fin >> mani;
			maninames.push_back(mani);
		}

		SITable.insert(hash_map<string, list<string>>::value_type(hashKey, maninames));
	}

	fin.close();
}
