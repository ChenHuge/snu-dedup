#include "StdAfx.h"
#include "ManiNode.h"

ManiNode::ManiNode(void)
{
	setHashKey("");
	setContainer("");
	setPosition(0);
	setLength(0);
}

ManiNode::ManiNode(string hash_key_)
{
	setHashKey(hash_key_);
	setContainer("");
	setPosition(0);
	setLength(0);
}

ManiNode::ManiNode(string hash_key_, string container_, fpos_t position_, size_t length_)
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


void ManiNode::setPosition(fpos_t position_)
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


fpos_t ManiNode::getPosition(void)
{
	return position;
}


size_t ManiNode::getLength(void)
{
	return length;
}
