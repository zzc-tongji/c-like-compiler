#ifndef FUNCTION_ITEM_H_
#define FUNCTION_ITEM_H_

#pragma warning(disable:4996)

#include <stdlib.h>
#include <stdint.h>
#include <exception>
#include "block.h"
#include "variable_item.h"
#include "word.h"

class FunctionItem
{
public:
	static FunctionItem * s_Malloc();
	static void s_Free(FunctionItem * function_item);
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
	std::vector<VariableItem *> parameter_table_;
	Word word_header; // It is a linked list.
	std::vector<CodeItem *> intermediate;
	std::vector<CodeItem *> assembler;
};

FunctionItem * FunctionItem::s_Malloc()
{
	return new FunctionItem();
}

void FunctionItem::s_Free(FunctionItem * function_item)
{
	if (function_item != NULL)
	{
		delete function_item;
	}
}

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
	for (int64_t i = 0; i < parameter_table_.size(); ++i)
	{
		VariableItem::s_Free(parameter_table_[i]);
		parameter_table_[i] = NULL;
	}
}

int64_t FunctionItem::SetName(const char * name)
{
	if (NULL == name || "" == name)
	{
		throw std::exception("Function \"int64_t FunctionItem::SetName(const char * name)\" says: Invalid parameter \"name\".");
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

#endif
