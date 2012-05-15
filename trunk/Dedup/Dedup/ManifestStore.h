#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iterator>

#include "Manifest.h"

using namespace std;

class ManifestStore
{

public:
	ManifestStore(void);
	~ManifestStore(void);
	list<Manifest> loadChampions(vector<string> chap_names);
	void createManifest(Manifest manifest);
};



