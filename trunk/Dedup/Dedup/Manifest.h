#pragma once

#include "ManiNode.h"
#include <string>
#include <list>

using namespace std;

class Manifest
{
private:
	list<ManiNode> nodes;
	string name;

public:
	Manifest(void);
	Manifest(string name_);
	~Manifest(void);
	string getName(void);
	int getSize(void);
	void setName(string name_);
	void addManiNode(ManiNode node);
	list<ManiNode>* getNodes(void);
};
