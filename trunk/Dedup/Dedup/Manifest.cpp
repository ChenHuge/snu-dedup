#include "StdAfx.h"
#include "Manifest.h"


Manifest::Manifest(void)
{
	setName("");
}

Manifest::Manifest(string name_)
{
	setName(name_);
}

Manifest::~Manifest(void)
{
}

string Manifest::getName(void)
{
	return name;
}

int Manifest::getSize(void)
{
	return 0;
}

void Manifest::setName(string name_)
{
	name = name_;
}

void Manifest::addManiNode(ManiNode node)
{
	nodes.push_back(node);
}


list<ManiNode>* Manifest::getNodes(void)
{
	return &nodes;
}
