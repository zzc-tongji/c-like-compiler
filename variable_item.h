#ifndef VARIABLE_ITEM_H_
#define VARIABLE_ITEM_H_

#include <stdint.h>
#include <string.h>

class VariableItem
{
public:
	const int64_t c_void = 0; // use for functions which returns "void" and blocks
	const int64_t c_int = 1;
	const int64_t c_double = 2;
	VariableItem();
	~VariableItem();
	void * MallocName(int64_t name_size);
	void * MallocGlobalName(int64_t global_name_size);
	// name, global name (with block id) and type (int or double)
	char * name_;
	char * global_name_;
	int64_t type_;
};

VariableItem::VariableItem()
{
	name_ = NULL;
	global_name_ = NULL;
	type_ = c_void;
}

VariableItem::~VariableItem()
{
	if (name_ != NULL)
	{
		delete[] name_;
		name_ = NULL;
	}
	if (global_name_ != NULL)
	{
		delete[] global_name_;
		global_name_ = NULL;
	}
}

void * VariableItem::MallocName(int64_t name_size)
{
	if (name_ != NULL)
	{
		delete[] name_;
		name_ = NULL;
	}
	name_ = new char[name_size];
	return name_;
}

void * VariableItem::MallocGlobalName(int64_t global_name_size)
{
	if (global_name_ != NULL)
	{
		delete[] global_name_;
		global_name_ = NULL;
	}
	global_name_ = new char[global_name_size];
	return global_name_;
}

#endif
