#ifndef FUNCTION_ITEM_H_
#define FUNCTION_ITEM_H_

#pragma warning(disable:4996)

#include <stdint.h>
#include "block.h"
#include "variable_item.h"

class FunctionItem
{
public:
	FunctionItem();
	~FunctionItem();
	int64_t SetName(const char * name);
	// location
	int64_t beginning_;
	int64_t end_;
	// block
	Block * block_tree;
	// name
	char * name_;
	// information
	int64_t return_type_;
	std::vector<VariableItem> parameter_table_;
};

FunctionItem::FunctionItem()
{
	beginning_ = -1;
	end_ = -1;
	block_tree = NULL;
	name_ = NULL;
	return_type_ = -1;
}

FunctionItem::~FunctionItem()
{
	if (name_ != NULL)
	{
		delete[] name_;
		name_ = NULL;
	}
	if (block_tree != NULL)
	{
		Block::s_FreeAll(block_tree);
		block_tree = NULL;
	}
}

int64_t FunctionItem::SetName(const char * name)
{
	if (NULL == name)
	{
		return 0;
	}
	name_ = new char[strlen(name) + 1];
	if (NULL == name_)
	{
		return -1;
	}
	strcpy(name_, name);
	return 1;
}

#endif
