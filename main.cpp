#pragma warning(disable:4244)
#pragma warning(disable:4996)

#include <stdlib.h>
#include <vector>
#include <exception>
#include "source_file.h"
#include "error.h"
#include "function_item.h"
#include "annotation_item.h"
#include "block.h"
#include "word.h"

//#define TEST_BLOCK_1
//#define TEST_BLOCK_2
//#define TEST_BLOCK_3

int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p);
int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem *> * function_table_p, std::vector<Block *> * block_table);
int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer);

int main(int argc, char ** argv)
{
	char path[1024];
	SourceFile source_file;
	Error error;
	std::vector<FunctionItem *> function_table;
	std::vector<Block *> block_table;

	// test file
	strcpy(path, "example.c.test");

	if (-1 == ReadSourceFile(path, &source_file, &error))
	{
		printf("%s\n", error.GetErrorString(&source_file));
		system("PAUSE");
		return 0;
	}
	if (-1 == Preprocess(&source_file, &error, &function_table, &block_table))
	{
		printf("%s\n", error.GetErrorString(&source_file));
		system("PAUSE");
		return 0;
	}
#ifdef TEST_BLOCK_1
	// test block #1
	{
		printf("----------  test block #1: BEGIN ----------\n\n");
		int64_t counter = 0;
		do
		{
			printf("{\ncontent:    [% 6I64d] %c\nline:       [% 6I64d] %I64d\nannotation: [% 6I64d] %s\n}\n", source_file.index_, source_file.content_[source_file.index_], source_file.line_index_, source_file.line_, source_file.annotation_index_, source_file.annotation_ ? "true" : "false");
			counter += 1;
			if (counter % 50 == 0)
			{
				system("PAUSE");
			}
		} while (1 == source_file.MoveNext());
		printf("----------  test block #1: END ----------\n\n");
		system("PAUSE");
	}
#endif
	for (int64_t i = 0; i < function_table.size(); ++i)
	{
		if (-1 == LexicalAnalyse(&source_file, &error, &block_table, false, function_table[i]))
		{
			printf("%s\n", error.GetErrorString(&source_file));
			system("PAUSE");
			return 0;
		}
	}
	for (int64_t i = 0; i < block_table.size(); ++i)
	{
		if (-1 == LexicalAnalyse(&source_file, &error, &block_table, true, block_table[i]))
		{
			printf("%s\n", error.GetErrorString(&source_file));
			system("PAUSE");
			return 0;
		}
	}
#ifdef TEST_BLOCK_2
	// test block #2
	{
		printf("----------  test block #2: BEGIN ----------\n\n");
		Word * word;
		for (int64_t i = 0; i < block_table.size(); ++i)
		{
			word = &(block_table[i]->word_header);
			printf("# %s: BEGIN #\n", block_table[i]->name_);
			while (word != NULL)
			{
				if (word->content_)
				{
					printf("%s", word->content_);
				}
				word = word->next_;
			}
			printf("\n# %s: END #\n\n", block_table[i]->name_);
		}
		for (int64_t i = 0; i < function_table.size(); ++i)
		{
			word = &(function_table[i]->word_header);
			printf("# function head %I64d: BEGIN #\n", i);
			while (word != NULL)
			{
				if (word->content_)
				{
					printf("%s", word->content_);
				}
				word = word->next_;
			}
			printf("\n# function head %I64d: BEGIN #\n\n", i);
		}
		printf("----------  test block #2: END ----------\n\n");
		system("PAUSE");
	}
#endif
#ifdef TEST_BLOCK_3
	// test block #3
	{
		printf("----------  test block #3: BEGIN ----------\n\n");
		Word * word;
		int64_t counter = 0;
		for (int64_t i = 0; i < function_table.size(); ++i)
		{
			word = &(function_table[i]->word_header);
			printf("# function: BEGIN #\n");
			while (word != NULL)
			{
				printf("{\n");
				if (word->content_)
				{
					printf("\tcontent:           %s\n", word->content_);
				}
				printf("\ttype:              %I64d\n", word->type_);
				printf("\tsource_file_index: %I64d\n", word->source_file_index_);
				printf("}\n");
				counter += 1;
				if (counter % 50 == 0)
				{
					system("PAUSE");
				}
				word = word->next_;
			}
			printf("\n# function: END #\n\n");
			system("PAUSE");
			counter = 0;
		}
		for (int64_t i = 0; i < block_table.size(); ++i)
		{
			word = &(block_table[i]->word_header);
			printf("# %s: BEGIN #\n", block_table[i]->name_);
			while (word != NULL)
			{
				printf("{\n");
				if (word->content_)
				{
					printf("\tcontent:           %s\n", word->content_);
				}
				printf("\ttype:              %I64d\n", word->type_);
				printf("\tsource_file_index: %I64d\n", word->source_file_index_);
				printf("}\n");
				counter += 1;
				if (counter % 50 == 0)
				{
					system("PAUSE");
				}
				word = word->next_;
			}
			printf("\n# %s: END #\n\n", block_table[i]->name_);
			system("PAUSE");
			counter = 0;
		}
		printf("----------  test block #3: END ----------\n\n");
	}
#endif
	// reclaim memory
	for (int64_t i = 0; i < function_table.size(); ++i)
	{
		FunctionItem::s_Free(function_table[i]);
		function_table[i] = NULL;
	}
	// Do not reclaim memory in "block_table"! They have been reclaimed already.
	system("PAUSE");
	return 0;
}

int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p)
{
	if (NULL == path || "" == path)
	{
		throw std::exception("Function \"int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p)\" says: Invalid parameter \"path\".");
	}
	if (NULL == source_file_p)
	{
		throw std::exception("Function \"int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p)\" says: Invalid parameter \"source_file_p\".");
	}
	if (NULL == error_p)
	{
		throw std::exception("Function \"int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p)\" says: Invalid parameter \"error_p\".");
	}
	FILE * fp = NULL;
	// open file
	fp = fopen(path, "r");
	if (NULL == fp)
	{
		error_p->major_no_ = 0;
		error_p->minor_no_ = 0;
		return -1;
	}
	// get file size
	fseek(fp, 0, SEEK_END);
	source_file_p->content_size_ = ftell(fp);
	if (0 == source_file_p->content_size_)
	{
		error_p->major_no_ = 0;
		error_p->minor_no_ = 1;
		return -1;
	}
	fseek(fp, 0, SEEK_SET);
	// malloc
	source_file_p->Malloc(source_file_p->content_size_ + 1);
	if (NULL == source_file_p->content_)
	{
		error_p->major_no_ = 0;
		error_p->minor_no_ = 2;
		return -1;
	}
	source_file_p->content_[source_file_p->content_size_] = '\0';
	// read file
	for (int64_t offset = 0, length = 0, remain = source_file_p->content_size_; remain > 0;)
	{
		length = fread(source_file_p->content_ + offset, 1, remain, fp);
		if (feof(fp))
		{
			break;
		}
		if (length <= 0)
		{
			error_p->major_no_ = 0;
			error_p->minor_no_ = 3;
			return -1;
		}
		offset += length;
		remain -= length;
	}
	// close file
	fclose(fp);
	fp = NULL;
	return 1;
}

int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem *> * function_table_p, std::vector<Block *> * block_table)
{
	if (NULL == source_file_p)
	{
		throw std::exception("Function \"int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem> * function_table_p, std::vector<Block *> * block_table)\" says: Invalid parameter \"source_file_p\".");
	}
	if (NULL == error_p)
	{
		throw std::exception("Function \"int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem> * function_table_p, std::vector<Block *> * block_table)\" says: Invalid parameter \"error_p\".");
	}
	if (NULL == function_table_p)
	{
		throw std::exception("Function \"int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem> * function_table_p, std::vector<Block *> * block_table)\" says: Invalid parameter \"function_table_p\".");
	}
	if (NULL == block_table)
	{
		throw std::exception("Function \"int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem> * function_table_p, std::vector<Block *> * block_table)\" says: Invalid parameter \"block_table\".");
	}
	// pointer
	FunctionItem * function_item_pointer = NULL;
	// stack of '(', '[', '{' and '@' (annotation)
	std::vector<char> parenthesis;
	std::vector<char> bracket;
	std::vector<char> brace;
	std::vector<char> annotation;
	// pointer
	AnnotationItem * annotation_item_pointer = NULL;
	// block
	Block * block_root;
	Block * block_p;
	Block * block_next_p;
	// line: The first line must be at the beginning of the source file.
	source_file_p->line_table_.push_back(0);
	// function: The first function must be at the beginning of the source file. If not, there must be an function format error. This type of error will be checked out behind.
	function_table_p->push_back(function_item_pointer);
	(*function_table_p)[function_table_p->size() - 1] = FunctionItem::s_Malloc();
	(*function_table_p)[function_table_p->size() - 1]->beginning_ = 0;
	// block
	block_root = new Block();
	block_p = block_root;
	block_next_p = block_p->child_;
	// traverse
	for (source_file_p->index_ = 0; source_file_p->index_ < source_file_p->content_size_; ++source_file_p->index_)
	{
		switch (source_file_p->content_[source_file_p->index_])
		{
			// line feed
		case '\r':
			if (source_file_p->index_ < source_file_p->content_size_ - 1 && source_file_p->content_[source_file_p->index_ + 1] == '\n')
			{
				// Windows LF: \r\n
				// fill in vector "line"
				source_file_p->line_table_.push_back(source_file_p->index_ + 2);
				if (source_file_p->index_ + 2 >= source_file_p->content_size_)
				{
					// If the final character is LF, remove the final line.
					source_file_p->line_table_.pop_back();
				}
				// skip the next character
				source_file_p->index_ += 1;
			}
			else
			{
				// Mac OS LF : \r
				// fill in vector "line"
				source_file_p->line_table_.push_back(source_file_p->index_ + 1);
				if (source_file_p->index_ + 1 >= source_file_p->content_size_)
				{
					// If the final character is LF, remove the final line.
					source_file_p->line_table_.pop_back();
				}
			}
			break;
		case '\n':
			// Linux LF: \n
			// fill in vector "line"
			source_file_p->line_table_.push_back(source_file_p->index_ + 1);
			if (source_file_p->index_ + 1 >= source_file_p->content_size_)
			{
				// If the final character is LF, remove the final line.
				source_file_p->line_table_.pop_back();
			}
			break;
			// matching (part 1)
		case '/':
			if (source_file_p->index_ < source_file_p->content_size_ - 1 && source_file_p->content_[source_file_p->index_ + 1] == '*')
			{
				// annotation beginning: /*
				annotation.push_back('@');
				source_file_p->annotation_ = true;
				// fill in vector "annotation_table" (1)
				source_file_p->annotation_table_.push_back(annotation_item_pointer);
				source_file_p->annotation_table_[source_file_p->annotation_table_.size() - 1] = AnnotationItem::s_Malloc();
				source_file_p->annotation_table_[source_file_p->annotation_table_.size() - 1]->beginning_ = source_file_p->index_;
				// skip the next character
				source_file_p->index_ += 1;
			}
			break;
		case '*':
			if (source_file_p->index_ < source_file_p->content_size_ - 1 && source_file_p->content_[source_file_p->index_ + 1] == '/')
			{
				// annotation end: */
				if (annotation.empty())
				{
					error_p->major_no_ = 1;
					error_p->minor_no_ = 0;
					return -1;
				}
				annotation.pop_back();
				source_file_p->annotation_ = false;
				// fill in vector "annotation_table" (2)
				source_file_p->annotation_table_[source_file_p->annotation_table_.size() - 1]->end_ = source_file_p->index_ + 1;
				// skip the next character
				source_file_p->index_ += 1;
			}
			break;
		case '(':
			if (source_file_p->annotation_)
			{
				break;
			}
			parenthesis.push_back('(');
			break;
		case ')':
			if (source_file_p->annotation_)
			{
				break;
			}
			if (parenthesis.empty())
			{
				error_p->major_no_ = 1;
				error_p->minor_no_ = 1;
				return -1;
			}
			parenthesis.pop_back();
			break;
		case '[':
			if (source_file_p->annotation_)
			{
				break;
			}
			bracket.push_back('[');
			break;
		case ']':
			if (source_file_p->annotation_)
			{
				break;
			}
			if (bracket.empty())
			{
				error_p->major_no_ = 1;
				error_p->minor_no_ = 2;
				return -1;
			}
			bracket.pop_back();
			break;
		case '{':
			if (source_file_p->annotation_)
			{
				break;
			}
			brace.push_back('{');
			// block
			block_next_p = block_p->AddChild();
			// block pointer table
			block_table->push_back(block_next_p);
			if (NULL == block_next_p)
			{
				error_p->major_no_ = 1;
				error_p->minor_no_ = 4;
				// free
				Block::s_FreeAll(block_root);
				return -1;
			}
			if (-1 == block_next_p->SetName())
			{
				error_p->major_no_ = 1;
				error_p->minor_no_ = 4;
				// free
				Block::s_FreeAll(block_root);
				return -1;
			}
			block_next_p->beginning_ = source_file_p->index_;
			block_p = block_next_p;
			break;
		case '}':
			if (source_file_p->annotation_)
			{
				break;
			}
			if (brace.empty())
			{
				error_p->major_no_ = 1;
				error_p->minor_no_ = 3;
				// free
				Block::s_FreeAll(block_root);
				return -1;
			}
			brace.pop_back();
			// block
			block_p->end_ = source_file_p->index_;
			block_p = block_p->parent_;
			if (brace.empty())
			{
				// Character '}' in the first level means that a function comes to an end.
				// function
				(*function_table_p)[function_table_p->size() - 1]->end_ = source_file_p->index_;
				function_table_p->push_back(function_item_pointer);
				(*function_table_p)[function_table_p->size() - 1] = FunctionItem::s_Malloc();
				(*function_table_p)[function_table_p->size() - 1]->beginning_ = source_file_p->index_ + 1;
			}
			break;
		default:
			break;
		}
	}
	// function: Remove the final item in function table because it must be incorrect.
	function_table_p->pop_back();
	// size
	source_file_p->line_size_ = source_file_p->line_table_.size();
	source_file_p->annotation_size_ = source_file_p->annotation_table_.size();
	// matching (part 2)
	if (false == parenthesis.empty())
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 1;
		return -1;
	}
	if (false == bracket.empty())
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 2;
		return -1;
	}
	if (false == brace.empty())
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 3;
		// free
		Block::s_FreeAll(block_root);
		return -1;
	}
	if (false == annotation.empty())
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 0;
		return -1;
	}
	// link functions with blocks
	for (block_p = block_root->child_; block_p != NULL; block_p = block_p->brother_)
	{
		for (int64_t i = 0; i < function_table_p->size(); ++i)
		{
			if (block_p->end_ == (*function_table_p)[i]->end_)
			{
				block_p->function_ = (*function_table_p)[i];
				(*function_table_p)[i]->block_tree = block_p;
				break;
			}
		}
	}
	// unlink function-blocks from block-root
	for (int64_t i = 0; i < function_table_p->size(); ++i)
	{
		(*function_table_p)[i]->block_tree->parent_ = NULL;
		(*function_table_p)[i]->block_tree->brother_ = NULL;
	}
	// delete block-root
	delete block_root;
	block_root = NULL;
	// get ready
	source_file_p->ReadyToMove();
	return 1;
}

int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer)
{
	if (NULL == source_file_p)
	{
		throw std::exception("Function \"int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer)\" says: Invalid parameter \"source_file_p\".");
	}
	if (NULL == error_p)
	{
		throw std::exception("Function \"int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer)\" says: Invalid parameter \"error_p\".");
	}
	if (NULL == block_pointer_table_p)
	{
		throw std::exception("Function \"int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer)\" says: Invalid parameter \"block_pointer_table_p\".");
	}
	if (NULL == block_pointer_table_p)
	{
		throw std::exception("Function \"int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer)\" says: Invalid parameter \"pointer\".");
	}
	FunctionItem * function_item_p;
	Block * block_p;
	Word * word_previous_p;
	Word * word_p;
	int64_t end;
	int64_t block_level;
	if (is_block)
	{
		// block
		block_p = (Block *)pointer;
		function_item_p = NULL;
		word_previous_p = block_p->word_header.previous_;
		word_p = &(block_p->word_header);
		source_file_p->JumpTo(block_p->beginning_);
		end = block_p->end_;
		block_level = 0;
	}
	else
	{
		// function (head)
		block_p = NULL;
		function_item_p = (FunctionItem *)pointer;
		word_previous_p = function_item_p->word_header.previous_;
		word_p = &(function_item_p->word_header);
		source_file_p->JumpTo(function_item_p->beginning_);
		end = function_item_p->end_;
		block_level = 1;
	}
	int64_t status = 0;
	char ch = source_file_p->content_[source_file_p->index_];
	while (source_file_p->index_ <= end)
	{
		// annotation
		if (source_file_p->annotation_)
		{
			// move to the next char
			source_file_p->MoveNext();
			// get a char
			ch = source_file_p->content_[source_file_p->index_];
			continue;
		}
		switch (status)
		{
		case 0:
			if (ch >= '0' && ch <= '9')
			{
				if ('0' == ch)
				{
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 199;
				}
				else
				{
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 101;
				}
			}
			else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || '_' == ch)
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 201;
			}
			else
			{
				switch (ch)
				{
				case '-':
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 301;
					break;
				case '(':
				case ')':
				case '!':
				case '*':
				case '/':
				case '%':
				case '+':
				case '<':
				case '>':
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 399;
					break;
				case '=':
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 302;
					break;
				case ' ':
				case '\t':
				case '\r':
				case '\n':
				case ';':
				case ',':
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 499;
					break;
				case '{':
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 499;
					// ignore child block (+)
					block_level += 1;
					if (block_level >= 2)
					{
						// search and jump index
						for (int64_t i = 0; i < block_pointer_table_p->size(); ++i)
						{
							if ((*block_pointer_table_p)[i]->beginning_ == source_file_p->index_)
							{
								source_file_p->JumpTo((*block_pointer_table_p)[i]->end_ - 1);
								break;
							}
						}
					}
					break;
				case '}':
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 499;
					// ignore child block (-)
					block_level -= 1;
					break;
				default:
					// set status (error)
					status = 598;
					break;
				}
			}
			break;
		case 101:
			if (ch >= '0' && ch <= '9')
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 101;
			}
			else
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 198;
			}
			break;
		case 198:
			// fallback a char
			Word::s_MoveBufferIndex(true, -1);
			source_file_p->JumpTo(source_file_p->index_ - 1);
			// add a word
			word_previous_p = word_p;
			word_p = Word::s_Insert(word_previous_p);
			if (NULL == word_p)
			{
				// set status (error)
				status = 597;
			}
			else
			{
				// set word
				word_p->SetContent(Word::s_buffer_);
				word_p->type_ = Word::c_constant_double_;
				word_p->source_file_index_ = source_file_p->index_;
				// reset global buffer
				Word::s_MoveBufferIndex(false, 0);
				// set status
				status = 0;
			}
			break;
		case 199:
			// add a word
			word_previous_p = word_p;
			word_p = Word::s_Insert(word_previous_p);
			if (NULL == word_p)
			{
				// set status (error)
				status = 597;
			}
			else
			{
				// set word
				word_p->SetContent(Word::s_buffer_);
				word_p->type_ = Word::c_constant_double_;
				word_p->source_file_index_ = source_file_p->index_;
				// reset global buffer
				Word::s_MoveBufferIndex(false, 0);
				// set status
				status = 0;
			}
			break;
		case 201:
			if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || '_' == ch)
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 201;
			}
			else
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 299;
			}
			break;
		case 299:
			// fallback a char
			Word::s_MoveBufferIndex(true, -1);
			source_file_p->JumpTo(source_file_p->index_ - 1);
			// add a word
			word_previous_p = word_p;
			word_p = Word::s_Insert(word_previous_p);
			if (NULL == word_p)
			{
				// set status (error)
				status = 597;
			}
			else
			{
				// set word
				word_p->SetContent(Word::s_buffer_);
				word_p->type_ = Word::c_identifier_;
				for (int64_t i = 0; i < Word::s_keyword_table_.size(); ++i)
				{
					if (0 == strcmp(Word::s_keyword_table_[i], word_p->content_))
					{
						word_p->type_ = Word::c_keyword_;
					}
				}
				word_p->source_file_index_ = source_file_p->index_;
				// reset global buffer
				Word::s_MoveBufferIndex(false, 0);
				// set status
				status = 0;
			}
			break;
		case 301:
			if (ch >= '1' && ch <= '9')
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 101;
			}
			else
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 398;
			}
			break;
		case 302:
			if ('=' == ch)
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 399;
			}
			else
			{
				// read a char
				Word::s_buffer_[Word::s_buffer_index_] = ch;
				Word::s_MoveBufferIndex(true, 1);
				// set status
				status = 398;
			}
			break;
		case 398:
			// fallback a char
			Word::s_MoveBufferIndex(true, -1);
			source_file_p->JumpTo(source_file_p->index_ - 1);
			// add a word
			word_previous_p = word_p;
			word_p = Word::s_Insert(word_previous_p);
			if (NULL == word_p)
			{
				// set status (error)
				status = 597;
			}
			else
			{
				// set word
				word_p->SetContent(Word::s_buffer_);
				word_p->type_ = Word::c_operator_;
				word_p->source_file_index_ = source_file_p->index_;
				// reset global buffer
				Word::s_MoveBufferIndex(false, 0);
				// set status
				status = 0;
			}
			break;
		case 399:
			// add a word
			word_previous_p = word_p;
			word_p = Word::s_Insert(word_previous_p);
			if (NULL == word_p)
			{
				// set status (error)
				status = 597;
			}
			else
			{
				// set word
				word_p->SetContent(Word::s_buffer_);
				word_p->type_ = Word::c_operator_;
				word_p->source_file_index_ = source_file_p->index_;
				// reset global buffer
				Word::s_MoveBufferIndex(false, 0);
				// set status
				status = 0;
			}
			break;
		case 499:
			// add a word
			word_previous_p = word_p;
			word_p = Word::s_Insert(word_previous_p);
			if (NULL == word_p)
			{
				// set status (error)
				status = 597;
			}
			else
			{
				// set word
				word_p->SetContent(Word::s_buffer_);
				word_p->type_ = Word::c_separator_;
				word_p->source_file_index_ = source_file_p->index_;
				// reset global buffer
				Word::s_MoveBufferIndex(false, 0);
				// set status
				status = 0;
			}
			break;
		case 597:
			// error: out of memory
			error_p->major_no_ = 2;
			error_p->minor_no_ = 3;
			return -1;
		case 598:
			// error: illegal character
			error_p->major_no_ = 2;
			error_p->minor_no_ = 2;
			return -1;
		case 599:
			// error: unrecognized word
			error_p->major_no_ = 2;
			error_p->minor_no_ = 1;
			return -1;
		default:
			throw std::exception("Function \"int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer)\" says: Invalid variable \"status\".");
			break;
		}
		if (status != 198 && status != 199 && status != 299 && status != 398 && status != 399 && status != 499)
		{
			// move to the next char
			source_file_p->MoveNext();
			// get a char
			ch = source_file_p->content_[source_file_p->index_];
		}
	}
	return 1;
}
