#ifndef WORD_H_
#define WORD_H_

#pragma warning(disable:4996)

#include <stdint.h>
#include <vector>

class Word
{
public:
	const int64_t c_keyword_ = 0;
	const int64_t c_identifier_ = 1;
	const int64_t c_constant_ = 2;
	const int64_t c_operator_ = 3;
	const int64_t c_delimiter_ = 4;
	static Word * s_Insert(Word * it_next);
	static void s_Remove(Word * it_self);
	static void s_RemoveAllNext(Word * word);
	Word();
	~Word();
	void RemoveAllNext();
	int64_t SetContent(const char * content);
	// pointer
	Word * previous_;
	Word * next_;
	// information
	char * content_;
	int64_t type_;
};

static Word * s_Insert(Word * it_next)
{
	Word * new_node = new Word();
	if (NULL == it_next || NULL == new_node)
	{
		return new_node;
	}
	// the new node
	new_node->previous_ = it_next;
	new_node->next_ = it_next->next_;
	// next node of the new node: set previous
	if (it_next->next_)
	{
		it_next->next_->previous_ = new_node;
	}
	// previous node of the new node: set next
	it_next->next_ = new_node;
	return new_node;
}

static void s_Remove(Word * it_self) // Pointer "it_self" must point to a dynamically allocated node!
{
	// previous node: set next
	if (it_self->previous_)
	{
		it_self->previous_->next_ = it_self->next_;
	}
	// next node: set previous
	if (it_self->next_)
	{
		it_self->next_->previous_ = it_self->previous_;
	}
	// delete
	delete it_self;
}

static void s_RemoveAllNext(Word * word) // Must be dynamically allocated nodes!
{
	if (word)
	{
		word->RemoveAllNext();
	}
}

Word::Word()
{
	previous_ = NULL;
	next_ = NULL;
	content_ = NULL;
	type_ = -1;
}

Word::~Word()
{
	if (content_ != NULL)
	{
		delete[] content_;
		content_ = NULL;
	}
}

void Word::RemoveAllNext() // This function should not be used outside this header file.
{
	if (next_)
	{
		next_->RemoveAllNext();
		delete next_;
		next_ = NULL;
	}
}

int64_t Word::SetContent(const char * content)
{
	if (NULL == content)
	{
		return 0;
	}
	content_ = new char[strlen(content) + 1];
	if (NULL == content_)
	{
		return -1;
	}
	strcpy(content_, content);
	return 1;
}

#endif
