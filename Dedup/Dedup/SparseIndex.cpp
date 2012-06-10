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
	
	//�̹� SITable�� �����ϴ� hashKey�� ���
	if (iter != SITable.end()) { 
		if (iter->second.size() == entrySize) {//�̹� �ش� hashKey�� ���� entry�� full�� ���
			/*cout << "SITable���� " << hashKey << "�� �ش��ϴ� entry�� ��á���ϴ�." << endl;
			return false;*/

			//list�� �� �� ��� ����
			iter->second.pop_front();
		}
		
		iter->second.push_back(maniName);
	}
	else {
		//���� SITable�� ���������ʴ� hashKey�� ���
		if (isFull()) { //�̹� SITable�� ���� ���
			/*cout << "SITable�� ��á���ϴ�." << endl;
			return false;*/
			
			//SITable�� �� �� ��Ʈ�� ����
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

	// point �ű��
	hash_map<string, float> candidates;

	for (int i = 0 ; i < hooks.size() ; i++) 
	{
		hash_map<string, list<string>>::iterator iter = SITable.find(hooks[i]);
		
		if (iter == SITable.end()) // SITable�� ���� hook�� ���
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

	// champions ����
	for (int i = 0 ; i < hooks.size() ; i++) 
	{
		// MaxNumChampion�� �ʰ��޴ٸ� �׸�!
		if (champions.size() >= maxNumChamp)
			break;

		hash_map<string, list<string>>::iterator iter = SITable.find(hooks[i]);
		if (iter == SITable.end()) // SITable�� ���� hook�� ���
			continue;

		string champ = "";
		float max_point = 0.0;

		// hook�� �ش��ϴ� �� entry���� point�� �ִ��� maniName ã��
		list<string>::iterator list_iter = iter->second.begin();
		for (list_iter = iter->second.begin() ; list_iter != iter->second.end() ; list_iter++) {
			string maniName = *list_iter;
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
