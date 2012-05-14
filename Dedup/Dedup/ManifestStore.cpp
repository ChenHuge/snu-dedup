#include "StdAfx.h"
#include "ManifestStore.h"


ManifestStore::ManifestStore(void)
{
}


ManifestStore::~ManifestStore(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////

Manifest::Manifest(void)
{
	setName("");
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


//////////////////////////////////////////////////////////////////////////////////////

ManiNode::ManiNode(void)
{
	ManiNode("", "", 0, 0);
}

ManiNode::ManiNode(string hash_key_)
{
	ManiNode(hash_key_, "", 0, 0);
}

ManiNode::ManiNode(string hash_key_, string container_, long int position_, size_t length_)
{
	setHashKey(hash_key_);
	setContainer(container_);
	setPosition(position_);
	setLength(length_);
}

ManiNode::~ManiNode(void)
{
}


void ManiNode::setHashKey(string hash_key_)
{
	hash_key = hash_key_;
}


void ManiNode::setContainer(string container_)
{
	container = container_;
}


void ManiNode::setPosition(long int position_)
{
	position = position_;
}


void ManiNode::setLength(size_t length_)
{
	length = length_;
}


string ManiNode::getHashKey(void)
{
	return hash_key;
}


string ManiNode::getContainer(void)
{
	return container;
}


long int ManiNode::getPosition(void)
{
	return position;
}


size_t ManiNode::getLength(void)
{
	return length;
}



