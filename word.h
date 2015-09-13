#ifndef WORD_H_
#define WORD_H_

#pragma warning(disable:4996)

#include <stdlib.h>
#include <stdint.h>
#include <vector>

class Word
{
public:
	const static int64_t c_keyword_ = 0;
	const static int64_t c_identifier_ = 1;
	const static int64_t c_constant_int_ = 2;
	const static int64_t c_operator_ = 3;
	const static int64_t c_separator_ = 4;
	static Word * s_Insert(Word * it_next);
	static void s_Remove(Word * it_self);
	static int64_t s_MoveBufferIndex(bool offset, int64_t number);
	static char s_buffer_[1024];
	static int64_t s_buffer_index_;
	// keyword table
	static std::vector<char *> s_keyword_table_;
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
	int64_t source_file_index_;
};

Word * Word::s_Insert(Word * it_next)
{
	if (NULL == it_next)
	{
		throw std::exception("Function \"Word * Word::s_Insert(Word * it_next)\" says: Invalid parameter \"it_next\".");
	}
	Word * new_node = new Word();
	if (NULL == new_node)
	{
		return NULL;
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

void Word::s_Remove(Word * it_self) // Pointer "it_self" must point to a dynamically allocated node!
{
	if (NULL == it_self)
	{
		throw std::exception("Function \"void Word::s_Remove(Word * it_self)\" says: Invalid parameter \"it_self\".");
	}
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

std::vector<char *> Word::s_keyword_table_ =
{
	"void",
	"int",
	"if",
	"else",
	"while",
	"return",
	"input",
	"output"
};

int64_t Word::s_MoveBufferIndex(bool offset, int64_t number)
{
	// Note: Fill first and move next!
	if (offset)
	{
		// move
		if (s_buffer_index_ + number > 1023)
		{
			return -1;
		}
		s_buffer_index_ += number;
	}
	else
	{
		// jump to
		if (number < 0 || number > 1023)
		{
			throw std::exception("Function \"int64_t Word::s_MoveBufferIndex(bool offset, int64_t number)\" says: Invalid parameter \"number\".");
		}
		s_buffer_index_ = number;
	}
	// set 0
	s_buffer_[s_buffer_index_] = '\0';
	return 1;
}

char Word::s_buffer_[1024] = { '\0' };

int64_t Word::s_buffer_index_ = 0;

Word::Word()
{
	previous_ = NULL;
	next_ = NULL;
	content_ = NULL;
	type_ = -1;
	source_file_index_ = -1;
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
	if (NULL == content || "" == content)
	{
		throw std::exception("Function \"int64_t Word::SetContent(const char * content)\" says: Invalid parameter \"content\".");
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
