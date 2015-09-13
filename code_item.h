#ifndef CODE_ITEM_H_
#define CODE_ITEM_H_

#include <stdlib.h>
#include <stdint.h>
#include <exception>

class CodeItem
{
public:
	static CodeItem * s_Malloc();
	static void s_Free(CodeItem * code_item);
	CodeItem();
	~CodeItem();
	int64_t SetLabel(const char * label);
	int64_t SetOp(const char * op);
	int64_t SetDst(const char * dst);
	int64_t SetSrc(const char * src);
	// content
	char * label_;
	char * op_;
	char * dst_;
	char * src_;
};

CodeItem * CodeItem::s_Malloc()
{
	return new CodeItem();
}

void CodeItem::s_Free(CodeItem * code_item)
{
	if (code_item != NULL)
	{
		delete code_item;
	}
}

CodeItem::CodeItem()
{
	label_ = NULL;
	op_ = NULL;
	dst_ = NULL;
	src_ = NULL;
}

CodeItem::~CodeItem()
{
	if (NULL == label_)
	{
		delete[] label_;
		label_ = NULL;
	}
	if (NULL == op_)
	{
		delete[] op_;
		op_ = NULL;
	}
	if (NULL == dst_)
	{
		delete[] dst_;
		dst_ = NULL;
	}
	if (NULL == src_)
	{
		delete[] src_;
		src_ = NULL;
	}
}

int64_t CodeItem::SetLabel(const char * label)
{
	if (NULL == label || "" == label)
	{
		throw std::exception("Function \"int64_t CodeItem::SetLabel(const char * label)\" says: Invalid parameter \"label\".");
	}
	if (label_ != NULL)
	{
		delete[] label_;
		label_ = NULL;
	}
	label_ = new char[strlen(label) + 1];
	if (NULL == label_)
	{
		return -1;
	}
	strcpy(label_, label);
	return 1;
}

int64_t CodeItem::SetOp(const char * op)
{
	if (NULL == op || "" == op)
	{
		throw std::exception("Function \"int64_t CodeItem::SetOp(const char * op)\" says: Invalid parameter \"op\".");
	}
	if (op_ != NULL)
	{
		delete[] op_;
		op_ = NULL;
	}
	op_ = new char[strlen(op) + 1];
	if (NULL == op_)
	{
		return -1;
	}
	strcpy(op_, op);
	return 1;
}

int64_t CodeItem::SetDst(const char * dst)
{
	if (NULL == dst || "" == dst)
	{
		throw std::exception("Function \"int64_t CodeItem::SetDst(const char * dst)\" says: Invalid parameter \"dst\".");
	}
	if (dst_ != NULL)
	{
		delete[] dst_;
		dst_ = NULL;
	}
	dst_ = new char[strlen(dst) + 1];
	if (NULL == dst_)
	{
		return -1;
	}
	strcpy(dst_, dst);
	return 1;
}

int64_t CodeItem::SetSrc(const char * src)
{
	if (NULL == src || "" == src)
	{
		throw std::exception("Function \"int64_t CodeItem::SetSrc(const char * src)\" says: Invalid parameter \"src\".");
	}
	if (src_ != NULL)
	{
		delete[] src_;
		src_ = NULL;
	}
	src_ = new char[strlen(src) + 1];
	if (NULL == src_)
	{
		return -1;
	}
	strcpy(src_, src);
	return 1;
}

#endif
