#pragma once
#include <string>

using namespace std;

class ManiNode
{
private:
	string hash_key;
	string container;
	fpos_t position;
	size_t length;

public:
	ManiNode(void);
	ManiNode(string hash_key_);
	ManiNode(string hash_key_, string container_, fpos_t position_, size_t length_);
	~ManiNode(void);
	void setHashKey(string hash_key_);
	void setContainer(string container_);
	void setPosition(fpos_t position_);
	void setLength(size_t length_);
	string getHashKey(void);
	string getContainer(void);
	fpos_t getPosition(void);
	size_t getLength(void);
};

