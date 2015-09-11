#ifndef BLOCK_H_
#define BLOCK_H_

#pragma warning(disable:4996)

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <exception>
#include "variable_item.h"
#include "word.h"

class FunctionItem;

class Block
{
public:
	static void s_FreeAll(Block * block);
	Block();
	Block * AddChild();
	void FreeAll();
	int64_t SetName();
	char * GeneratLabelName();
	// location
	int64_t beginning_;
	int64_t end_;
	// pointer
	FunctionItem * function_;
	Block * parent_;
	Block * child_;
	Block * brother_;
	// name
	int64_t id_;
	char * name_;
	char * name_in_;
	char * name_out_;
	// information
	std::vector<VariableItem *> variable_table_;
	Word word_header; // It is a linked list.
	int64_t label_id_now_;
private:
	// id allocator
	static int64_t s_GenerateId();
	static int64_t s_id_;
};

void Block::s_FreeAll(Block * block)
{
	if (NULL == block)
	{
		throw std::exception("Function \"void Block::s_FreeAll(Block * block)\" says: Invalid parameter \"block\".");
	}
	block->FreeAll();
	delete block;
}

Block::Block()
{
	beginning_ = -1;
	end_ = -1;
	function_ = NULL;
	parent_ = NULL;
	child_ = NULL;
	brother_ = NULL;
	id_ = s_GenerateId();
	name_ = NULL;
	name_in_ = NULL;
	name_out_ = NULL;
	label_id_now_ = -1;
}

Block * Block::AddChild()
{
	Block * node;
	Block * node_brother;
	if (NULL == child_)
	{
		// add to child
		child_ = new Block();
		if (NULL == child_)
		{
			return NULL;
		}
		child_->parent_ = this;
		return child_;
	}
	else
	{
		// add to child's brother
		node = child_;
		node_brother = child_->brother_;
		while (node_brother != NULL)
		{
			node = node_brother;
			node_brother = node_brother->brother_;
		}
		node->brother_ = new Block();
		if (NULL == node->brother_)
		{
			return NULL;
		}
		node->brother_->parent_ = this;
	}
	return node->brother_;
}

void Block::FreeAll()
{
	if (child_)
	{
		child_->FreeAll();
		delete child_;
		child_ = NULL;
	}
	if (brother_)
	{
		brother_->FreeAll();
		delete brother_;
		brother_ = NULL;
	}
}

int64_t Block::SetName()
{
	// name
	if (name_ != NULL)
	{
		delete[] name_;
		name_ = NULL;
	}
	name_ = new char(8 + int64_t(log10(double(id_))));
	if (NULL == name_)
	{
		return -1;
	}
	sprintf(name_, "block_%I64d", id_);
	// name in
	if (name_in_ != NULL)
	{
		delete[] name_in_;
		name_in_ = NULL;
	}
	name_in_ = new char(11 + int64_t(log10(double(id_))));
	if (NULL == name_in_)
	{
		return -1;
	}
	sprintf(name_in_, "block_%I64d_in", id_);
	// name out
	if (name_out_ != NULL)
	{
		delete[] name_out_;
		name_out_ = NULL;
	}
	name_out_ = new char(12 + int64_t(log10(double(id_))));
	if (NULL == name_out_)
	{
		return -1;
	}
	sprintf(name_out_, "block_%I64d_out", id_);
	return 1;
}

char * Block::GeneratLabelName()
{
	// The returned value point to a block of dynamically allocated memory. It should be reclaim after using.
	char * value;
	label_id_now_ += 1;
	value = new char[strlen(name_) + int64_t(log10(double(label_id_now_))) + 8];
	if (NULL == value)
	{
		return NULL;
	}
	sprintf(value, "%s_label_%I64d", name_, label_id_now_);
	return value;
}

int64_t Block::s_GenerateId()
{
	s_id_ += 1;
	return s_id_;
}

int64_t Block::s_id_ = -2;

#endif
