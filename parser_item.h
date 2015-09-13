#ifndef PARSER_ITEM_H_
#define PARSER_ITEM_H_

#include <stdint.h>
#include <vector>

class ParserItem
{
public:
	static ParserItem * s_Malloc();
	static void s_Free(ParserItem * parser_item);
	static const int64_t c_vn_ = -1;
	static const int64_t c_vn_s_ = -2;
	ParserItem();
	int64_t left;
	std::vector<int64_t> right;
};

ParserItem * ParserItem::s_Malloc()
{
	return new ParserItem();
}

void ParserItem::s_Free(ParserItem * parser_item)
{
	if (parser_item != NULL)
	{
		delete parser_item;
	}
}

ParserItem::ParserItem()
{
	left = c_vn_;
}

#endif
