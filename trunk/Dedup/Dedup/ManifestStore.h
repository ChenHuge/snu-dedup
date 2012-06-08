#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <hash_map>

#include "Manifest.h"
#include "ManiNode.h"

using namespace std;

class ManifestStore
{

public:
	ManifestStore(void);
	~ManifestStore(void);
	hash_map<string, ManiNode> loadChampions(vector<string> chap_names);
	void createManifest(Manifest manifest);
};



