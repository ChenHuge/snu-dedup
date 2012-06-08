#include "StdAfx.h"
#include "ManifestStore.h"
#include <Windows.h>
#include <io.h>


using namespace std;

ManifestStore::ManifestStore(void)
{
}


ManifestStore::~ManifestStore(void)
{
}

hash_map<string, ManiNode> ManifestStore::loadChampions(vector<string> chap_names)
{
	hash_map<string, ManiNode> keys;

	for (int i = 0 ; i < chap_names.size() ; i++) 
	{
		ifstream fin("ManifestStore\\" + chap_names[i], ios::in);
		if (!fin) {
			cerr << "ERROR: Fail to open a manifest file '" << chap_names[i] << "'" << endl;
			cerr << "       at ManifestStore::loadChampions" << endl;
			Sleep(2000);
			exit(1);
		}

		string hash, cont;
		fpos_t pos;
		size_t length;

		while (fin >> hash >> cont >> pos >> length) {
			ManiNode maniNode(hash, cont, pos, length);
			keys.insert(hash_map<string, ManiNode>::value_type(hash, maniNode));
		}

		fin.close();
	}

	return keys;
}

void ManifestStore::createManifest(Manifest manifest)
{
	if (access(("ManifestStore\\" + manifest.getName()).c_str(), 00) == 0) {  //이미 해당 Manifest file이 존재할 경우
		cerr << "ERROR: Fail to create a manifest file '" << manifest.getName() << "'" << endl;
		cerr << "       '" << manifest.getName() << "' is already existed." << endl;
		cerr << "       at ManifestStore::createManifest" << endl;
		Sleep(2000);
		exit(1);
	}

	ofstream fout("ManifestStore\\" + manifest.getName(), ios::out);
	if (!fout) {
		cerr << "ERROR: Fail to create a manifest file '" << manifest.getName() << "'" << endl;
		cerr << "       at ManifestStore::createManifest" << endl;
		Sleep(2000);
		exit(1);
	}

	list<ManiNode>* p_nodes = manifest.getNodes();
	list<ManiNode>::iterator iter = p_nodes->begin();
	for (iter = p_nodes->begin() ; iter != p_nodes->end() ; iter++) {
		fout << iter->getHashKey() << ' ' << iter->getContainer() << ' ' << iter->getPosition() << ' ' << iter->getLength() << endl;
	}

	fout.close();
}
