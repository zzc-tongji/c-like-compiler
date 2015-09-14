#ifndef VARIABLE_ITEM_H_
#define VARIABLE_ITEM_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

class VariableItem
{
public:
	const static int64_t c_void_ = 0; // use for functions which returns "void" and blocks
	const static int64_t c_int_ = 1;
	static VariableItem * s_Malloc();
	static void s_Free(VariableItem * variable_item);
	static char s_buffer_[1024];
	VariableItem();
	~VariableItem();
	int64_t SetName(const char * name);
	int64_t SetGlobalName(const char * global_name);
	// information
	char * name_;
	char * global_name_;
	int64_t type_;
	int64_t stack_offset;
};

VariableItem * VariableItem::s_Malloc()
{
	return new VariableItem();
}

void VariableItem::s_Free(VariableItem * variable_item)
{
	if (variable_item != NULL)
	{
		delete variable_item;
	}
}

char VariableItem::s_buffer_[1024] = { '\0' };

VariableItem::VariableItem()
{
	name_ = NULL;
	global_name_ = NULL;
	type_ = -1;
	stack_offset = -1;
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

int64_t VariableItem::SetName(const char * name)
{
	if (NULL == name || "" == name)
	{
		throw std::exception("Function \"int64_t VariableItem::SetName(const char * name)\" says: Invalid parameter \"name\".");
	}
	if (name_ != NULL)
	{
		delete[] name_;
		name_ = NULL;
	}
	name_ = new char[strlen(name) + 1];
	if (NULL == name_)
	{
		return -1;
	}
	strcpy(name_, name);
	return 1;
}

int64_t VariableItem::SetGlobalName(const char * global_name)
{
	if (NULL == global_name || "" == global_name)
	{
		throw std::exception("Function \"int64_t SetGlobalName(const char * global_name)\" says: Invalid parameter \"global_name\".");
	}
	if (global_name_ != NULL)
	{
		delete[] global_name_;
		global_name_ = NULL;
	}
	global_name_ = new char[strlen(global_name) + 1];
	if (NULL == global_name_)
	{
		return -1;
	}
	strcpy(global_name_, global_name);
	return 1;
}

#endif
