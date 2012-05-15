#pragma once
#include <string>

using namespace std;

class ManiNode
{
private:
	string hash_key;
	string container;
	long int position;
	size_t length;

public:
	ManiNode(void);
	ManiNode(string hash_key_);
	ManiNode(string hash_key_, string container_, long int position_, size_t length_);
	~ManiNode(void);
	void setHashKey(string hash_key_);
	void setContainer(string container_);
	void setPosition(long int position_);
	void setLength(size_t length_);
	string getHashKey(void);
	string getContainer(void);
	long int getPosition(void);
	size_t getLength(void);
};

