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
#include "parser_item.h"

//#define TEST_BLOCK_1
//#define TEST_BLOCK_2
//#define TEST_BLOCK_3
//#define TEST_BLOCK_4
//#define TEST_BLOCK_5
//#define TEST_BLOCK_6
//#define TEST_BLOCK_7

int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p);
int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem *> * function_table_p, std::vector<Block *> * block_table);
int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer);
void RemoveBlankWord(bool is_block, void * pointer);
int64_t ParseFunctionHead(SourceFile * source_file_p, Error * error_p, FunctionItem * function_item_p);
int64_t ParseBlock(SourceFile * source_file_p, Error * error_p, Block * block_p);
int64_t ParseBlock_GetSymbol(Word * word_p);

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
		system("PAUSE");
	}
#endif
	for (int64_t i = 0; i < function_table.size(); ++i)
	{
		RemoveBlankWord(false, function_table[i]);
	}
	for (int64_t i = 0; i < block_table.size(); ++i)
	{
		RemoveBlankWord(true, block_table[i]);
	}
#ifdef TEST_BLOCK_4
	// test block #4
	{
		printf("----------  test block #4: BEGIN ----------\n\n");
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
		printf("----------  test block #4: END ----------\n\n");
		system("PAUSE");
	}
#endif
	for (int64_t i = 0; i < function_table.size(); ++i)
	{
		if (-1 == ParseFunctionHead(&source_file, &error, function_table[i]))
		{
			printf("%s\n", error.GetErrorString(&source_file));
			system("PAUSE");
			return 0;
		}
	}
#ifdef TEST_BLOCK_5
	// test block #5
	{
		printf("----------  test block #5: BEGIN ----------\n\n");
		for (int64_t i = 0; i < function_table.size(); ++i)
		{
			printf("# %s: BEGIN #\n", function_table[i]->name_);
			printf("{\n");
			printf("\ttype of returned value: %I64d\n", function_table[i]->return_type_);
			for (int64_t j = 0; j < function_table[i]->parameter_table_.size(); ++j)
			{
				printf("\tparameter       % 6I64d: %s\n", j, function_table[i]->parameter_table_[j]->name_);
			}
			printf("}\n");
			printf("# %s: END #\n\n", function_table[i]->name_);
			system("PAUSE");
		}
		printf("----------  test block #5: END ----------\n\n");
		system("PAUSE");
	}
#endif
	for (int64_t i = 0; i < block_table.size(); ++i)
	{
		if (-1 == ParseBlock(&source_file, &error, block_table[i]))
		{
			printf("%s\n", error.GetErrorString(&source_file));
			system("PAUSE");
			return 0;
		}
	}
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
	if (NULL == (*function_table_p)[function_table_p->size() - 1])
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 4;
		return -1;
	}
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
				if (NULL == source_file_p->annotation_table_[source_file_p->annotation_table_.size() - 1])
				{
					error_p->major_no_ = 1;
					error_p->minor_no_ = 4;
					// free
					Block::s_FreeAll(block_root);
					return -1;
				}
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
				if (NULL == (*function_table_p)[function_table_p->size() - 1])
				{
					error_p->major_no_ = 1;
					error_p->minor_no_ = 4;
					// free
					Block::s_FreeAll(block_root);
					return -1;
				}
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
				case '$':
					// read a char
					Word::s_buffer_[Word::s_buffer_index_] = ch;
					Word::s_MoveBufferIndex(true, 1);
					// set status
					status = 303;
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
				word_p->type_ = Word::c_constant_int_;
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
				word_p->type_ = Word::c_constant_int_;
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
		case 303:
			if ('$' == ch)
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

void RemoveBlankWord(bool is_block, void * pointer)
{
	if (NULL == pointer)
	{
		throw std::exception("Function \"void RemoveBlankWord(bool is_block, void * pointer)\" says: Invalid parameter \"pointer\".");
	}
	Block * block_p;
	FunctionItem * function_item_p;
	Word * word_p;
	Word * word_next_p;
	if (is_block)
	{
		// block
		block_p = (Block *)pointer;
		function_item_p = NULL;
		word_p = block_p->word_header.next_;
		// remove "{" in the beginning
		word_next_p = word_p->next_;
		Word::s_Remove(word_p);
		word_p = word_next_p;
	}
	else
	{
		// function (head)
		block_p = NULL;
		function_item_p = (FunctionItem *)pointer;
		word_p = function_item_p->word_header.next_;
	}
	while (word_p)
	{
		word_next_p = word_p->next_;
		if (is_block && NULL == word_next_p)
		{
			// remove "}" in the end
			Word::s_Remove(word_p);
			word_p = NULL;
		}
		else
		{
			// remove blank word
			if (Word::c_separator_ == word_p->type_)
			{
				switch (word_p->content_[0])
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					Word::s_Remove(word_p);
					break;
				default:
					break;
				}
			}
			word_p = word_next_p;
		}
	}
}

int64_t ParseFunctionHead(SourceFile * source_file_p, Error * error_p, FunctionItem * function_item_p)
{
	if (NULL == source_file_p)
	{
		throw std::exception("Function \"int64_t ParseFunctionHead(SourceFile * source_file_p, Error * error_p, FunctionItem * function_item_p)\" says: Invalid parameter \"source_file_p\".");
	}
	if (NULL == error_p)
	{
		throw std::exception("Function \"int64_t ParseFunctionHead(SourceFile * source_file_p, Error * error_p, FunctionItem * function_item_p)\" says: Invalid parameter \"error_p\".");
	}
	if (NULL == function_item_p)
	{
		throw std::exception("Function \"int64_t ParseFunctionHead(SourceFile * source_file_p, Error * error_p, FunctionItem * function_item_p)\" says: Invalid parameter \"function_item_p\".");
	}
	Word * word_p = function_item_p->word_header.next_;
	int64_t parameter_number = 0;
	bool end = false;
	//VariableItem temp;
	// type of return value
	VariableItem * pointer;
	if (NULL == word_p)
	{
		// error
		error_p->major_no_ = 3;
		error_p->minor_no_ = 1;
		source_file_p->JumpTo(function_item_p->beginning_);
		return -1;
	}
	if (word_p->type_ != Word::c_keyword_)
	{
		// error
		error_p->major_no_ = 3;
		error_p->minor_no_ = 2;
		source_file_p->JumpTo(word_p->source_file_index_);
		return -1;
	}
	if (0 == strcmp(word_p->content_, "void"))
	{
		function_item_p->return_type_ = VariableItem::c_void_;
	}
	else if (0 == strcmp(word_p->content_, "int"))
	{
		function_item_p->return_type_ = VariableItem::c_int_;
	}
	else
	{
		// error
		error_p->major_no_ = 3;
		error_p->minor_no_ = 2;
		source_file_p->JumpTo(word_p->source_file_index_);
		return -1;
	}
	// function name
	word_p = word_p->next_;
	if (NULL == word_p)
	{
		// error
		error_p->major_no_ = 3;
		error_p->minor_no_ = 3;
		source_file_p->JumpTo(function_item_p->beginning_);
		return -1;
	}
	if (word_p->type_ != Word::c_identifier_)
	{
		// error
		error_p->major_no_ = 3;
		error_p->minor_no_ = 4;
		source_file_p->JumpTo(word_p->source_file_index_);
		return -1;
	}
	function_item_p->SetName(word_p->content_);
	// "("
	word_p = word_p->next_;
	if (NULL == word_p)
	{
		// error
		error_p->major_no_ = 3;
		error_p->minor_no_ = 3;
		source_file_p->JumpTo(function_item_p->beginning_);
		return -1;
	}
	if (word_p->type_ != Word::c_operator_ || word_p->content_[0] != '(')
	{
		// error
		error_p->major_no_ = 3;
		error_p->minor_no_ = 5;
		source_file_p->JumpTo(word_p->source_file_index_);
		return -1;
	}
	// function with no parameter
	if (word_p != NULL && word_p->next_ != NULL && ')' == word_p->next_->content_[0])
	{
		end = true;
	}
	// function with parameter
	while (false == end)
	{
		// type of parameter
		word_p = word_p->next_;
		if (NULL == word_p)
		{
			// error
			error_p->major_no_ = 3;
			error_p->minor_no_ = 3;
			source_file_p->JumpTo(function_item_p->beginning_);
			return -1;
		}
		if (strcmp(word_p->content_, "int"))
		{
			// error
			error_p->major_no_ = 3;
			error_p->minor_no_ = 6;
			source_file_p->JumpTo(word_p->source_file_index_);
			return -1;
		}
		function_item_p->parameter_table_.push_back(pointer);
		function_item_p->parameter_table_[function_item_p->parameter_table_.size() - 1] = VariableItem::s_Malloc();
		if (NULL == function_item_p->parameter_table_[function_item_p->parameter_table_.size() - 1])
		{
			error_p->major_no_ = 3;
			error_p->minor_no_ = 9;
			return -1;
		}
		function_item_p->parameter_table_[function_item_p->parameter_table_.size() - 1]->type_ = VariableItem::c_int_;
		// parameter name
		word_p = word_p->next_;
		if (NULL == word_p)
		{
			// error
			error_p->major_no_ = 3;
			error_p->minor_no_ = 3;
			source_file_p->JumpTo(function_item_p->beginning_);
			return -1;
		}
		if (word_p->type_ != Word::c_identifier_)
		{
			// error
			error_p->major_no_ = 3;
			error_p->minor_no_ = 7;
			source_file_p->JumpTo(word_p->source_file_index_);
			return -1;
		}
		if (-1 == function_item_p->parameter_table_[function_item_p->parameter_table_.size() - 1]->SetName(word_p->content_))
		{
			// error
			error_p->major_no_ = 3;
			error_p->minor_no_ = 9;
			return -1;
		}
		// "," or ")"
		word_p = word_p->next_;
		if (NULL == word_p)
		{
			// error
			error_p->major_no_ = 3;
			error_p->minor_no_ = 3;
			source_file_p->JumpTo(function_item_p->beginning_);
			return -1;
		}
		if (word_p->type_ != Word::c_separator_ || word_p->content_[0] != ',')
		{
			if (word_p->type_ != Word::c_operator_ || word_p->content_[0] != ')')
			{
				// error
				error_p->major_no_ = 3;
				error_p->minor_no_ = 8;
				source_file_p->JumpTo(word_p->source_file_index_);
				return -1;
			}
			else
			{
				end = true;
			}
		}
	}
	return 1;
}

int64_t ParseBlock(SourceFile * source_file_p, Error * error_p, Block * block_p)
{
	if (NULL == source_file_p)
	{
		throw std::exception("Function \"int64_t ParseBlock(SourceFile * source_file_p, Error * error_p, Block * block_p)\" says: Invalid parameter \"source_file_p\".");
	}
	if (NULL == error_p)
	{
		throw std::exception("Function \"int64_t ParseBlock(SourceFile * source_file_p, Error * error_p, Block * block_p)\" says: Invalid parameter \"error_p\".");
	}
	if (NULL == block_p)
	{
		throw std::exception("Function \"int64_t ParseBlock(SourceFile * source_file_p, Error * error_p, Block * block_p)\" says: Invalid parameter \"block_p\".");
	}
	char priority_table[28][28];
	const char undefined = 'u';
	const char c_equal_to = '=';
	const char c_less_than = '<';
	const char c_greater_than = '>';
	// u : initialization
	for (int64_t i = 0; i < 28; ++i)
	{
		for (int64_t j = 0; j < 28; ++j)
		{
			priority_table[i][j] = undefined;
		}
	}
	// = : 18 items
	priority_table[0][0] = c_equal_to;
	priority_table[1][14] = c_equal_to;
	priority_table[1][26] = c_equal_to;
	priority_table[2][0] = c_equal_to;
	priority_table[8][9] = c_equal_to;
	priority_table[9][16] = c_equal_to;
	priority_table[16][17] = c_equal_to;
	priority_table[17][20] = c_equal_to;
	priority_table[18][26] = c_equal_to;
	priority_table[19][8] = c_equal_to;
	priority_table[20][16] = c_equal_to;
	priority_table[21][8] = c_equal_to;
	priority_table[22][14] = c_equal_to;
	priority_table[23][1] = c_equal_to;
	priority_table[24][1] = c_equal_to;
	priority_table[26][1] = c_equal_to;
	priority_table[26][2] = c_equal_to;
	priority_table[26][8] = c_equal_to;
	// < : 59 + 27 items
	priority_table[0][26] = c_less_than;
	priority_table[2][3] = c_less_than;
	priority_table[2][4] = c_less_than;
	priority_table[2][5] = c_less_than;
	priority_table[2][6] = c_less_than;
	priority_table[2][7] = c_less_than;
	priority_table[2][8] = c_less_than;
	priority_table[2][25] = c_less_than;
	priority_table[2][26] = c_less_than;
	priority_table[3][5] = c_less_than;
	priority_table[3][6] = c_less_than;
	priority_table[3][7] = c_less_than;
	priority_table[3][8] = c_less_than;
	priority_table[3][25] = c_less_than;
	priority_table[3][26] = c_less_than;
	priority_table[4][5] = c_less_than;
	priority_table[4][6] = c_less_than;
	priority_table[4][7] = c_less_than;
	priority_table[4][8] = c_less_than;
	priority_table[4][25] = c_less_than;
	priority_table[4][26] = c_less_than;
	priority_table[5][8] = c_less_than;
	priority_table[5][25] = c_less_than;
	priority_table[5][26] = c_less_than;
	priority_table[6][8] = c_less_than;
	priority_table[6][25] = c_less_than;
	priority_table[6][26] = c_less_than;
	priority_table[7][8] = c_less_than;
	priority_table[7][25] = c_less_than;
	priority_table[7][26] = c_less_than;
	priority_table[8][3] = c_less_than;
	priority_table[8][4] = c_less_than;
	priority_table[8][5] = c_less_than;
	priority_table[8][6] = c_less_than;
	priority_table[8][7] = c_less_than;
	priority_table[8][8] = c_less_than;
	priority_table[8][10] = c_less_than;
	priority_table[8][11] = c_less_than;
	priority_table[8][12] = c_less_than;
	priority_table[8][13] = c_less_than;
	priority_table[8][15] = c_less_than;
	priority_table[8][25] = c_less_than;
	priority_table[8][26] = c_less_than;
	priority_table[10][25] = c_less_than;
	priority_table[10][26] = c_less_than;
	priority_table[11][25] = c_less_than;
	priority_table[11][26] = c_less_than;
	priority_table[12][25] = c_less_than;
	priority_table[12][26] = c_less_than;
	priority_table[13][10] = c_less_than;
	priority_table[13][11] = c_less_than;
	priority_table[13][12] = c_less_than;
	priority_table[13][13] = c_less_than;
	priority_table[13][25] = c_less_than;
	priority_table[13][26] = c_less_than;
	priority_table[15][25] = c_less_than;
	priority_table[15][26] = c_less_than;
	priority_table[22][25] = c_less_than;
	priority_table[22][26] = c_less_than;
	for (int64_t i = 1; i < 27; ++i)
	{
		priority_table[27][i] = c_less_than;
	}
	// > : 67 + 27 + 26 items
	priority_table[0][14] = c_greater_than;
	priority_table[2][14] = c_greater_than;
	priority_table[3][3] = c_greater_than;
	priority_table[3][4] = c_greater_than;
	priority_table[3][9] = c_greater_than;
	priority_table[3][14] = c_greater_than;
	priority_table[4][3] = c_greater_than;
	priority_table[4][4] = c_greater_than;
	priority_table[4][9] = c_greater_than;
	priority_table[4][14] = c_greater_than;
	priority_table[5][3] = c_greater_than;
	priority_table[5][4] = c_greater_than;
	priority_table[5][5] = c_greater_than;
	priority_table[5][6] = c_greater_than;
	priority_table[5][7] = c_greater_than;
	priority_table[5][9] = c_greater_than;
	priority_table[5][14] = c_greater_than;
	priority_table[6][3] = c_greater_than;
	priority_table[6][4] = c_greater_than;
	priority_table[6][5] = c_greater_than;
	priority_table[6][6] = c_greater_than;
	priority_table[6][7] = c_greater_than;
	priority_table[6][9] = c_greater_than;
	priority_table[6][14] = c_greater_than;
	priority_table[7][3] = c_greater_than;
	priority_table[7][4] = c_greater_than;
	priority_table[7][5] = c_greater_than;
	priority_table[7][6] = c_greater_than;
	priority_table[7][7] = c_greater_than;
	priority_table[7][9] = c_greater_than;
	priority_table[7][14] = c_greater_than;
	priority_table[9][0] = c_greater_than;
	priority_table[9][3] = c_greater_than;
	priority_table[9][4] = c_greater_than;
	priority_table[9][5] = c_greater_than;
	priority_table[9][6] = c_greater_than;
	priority_table[9][7] = c_greater_than;
	priority_table[9][9] = c_greater_than;
	priority_table[9][14] = c_greater_than;
	priority_table[10][9] = c_greater_than;
	priority_table[11][9] = c_greater_than;
	priority_table[12][9] = c_greater_than;
	priority_table[13][9] = c_greater_than;
	priority_table[15][9] = c_greater_than;
	priority_table[15][14] = c_greater_than;
	priority_table[25][3] = c_greater_than;
	priority_table[25][4] = c_greater_than;
	priority_table[25][5] = c_greater_than;
	priority_table[25][6] = c_greater_than;
	priority_table[25][7] = c_greater_than;
	priority_table[25][9] = c_greater_than;
	priority_table[25][10] = c_greater_than;
	priority_table[25][11] = c_greater_than;
	priority_table[25][12] = c_greater_than;
	priority_table[25][14] = c_greater_than;
	priority_table[25][15] = c_greater_than;
	priority_table[26][3] = c_greater_than;
	priority_table[26][4] = c_greater_than;
	priority_table[26][5] = c_greater_than;
	priority_table[26][6] = c_greater_than;
	priority_table[26][7] = c_greater_than;
	priority_table[26][9] = c_greater_than;
	priority_table[26][10] = c_greater_than;
	priority_table[26][11] = c_greater_than;
	priority_table[26][12] = c_greater_than;
	priority_table[26][14] = c_greater_than;
	priority_table[26][15] = c_greater_than;
	for (int64_t i = 1; i < 27; ++i)
	{
		priority_table[14][i] = c_greater_than;
		if (i != 20)
		{
			priority_table[17][i] = c_greater_than;
		}
	}
#ifdef TEST_BLOCK_6
	// test block
	{
		printf("----------  test block #6 : BEGIN ----------\n\n");
		printf("# priority table: BEGIN #\n\n");
		printf("   ");
		for (int64_t j = 0; j < 28; ++j)
		{
			printf("%02I64d ", j);
		}
		printf("\n");
		for (int64_t i = 0; i < 28; ++i)
		{
			printf("%02I64d  ", i);
			for (int64_t j = 0; j < 27; ++j)
			{
				if ('u' == priority_table[i][j])
				{
					printf("   ", priority_table[i][j]);
				}
				else
				{
					printf("%c  ", priority_table[i][j]);
				}
			}
			printf("\n");
		}
		printf("\n# priority table: END #\n\n");
		printf("----------  test block #6 : END ----------\n\n");
		system("PAUSE");
	}
#endif
	std::vector<ParserItem *> parser_table;
	ParserItem * pointer = NULL;
	for (int64_t i = 0; i < 25; ++i)
	{
		parser_table.push_back(pointer);
		parser_table[parser_table.size() - 1] = ParserItem::s_Malloc();
		if (NULL == parser_table[parser_table.size() - 1])
		{
			error_p->major_no_ = 4;
			error_p->minor_no_ = 2;
			return -1;
		}
	}
	// 0
	parser_table[0]->right.push_back(ParserItem::c_vn_);
	parser_table[0]->right.push_back(14);
	// 1
	parser_table[1]->right.push_back(19);
	parser_table[1]->right.push_back(8);
	parser_table[1]->right.push_back(ParserItem::c_vn_);
	parser_table[1]->right.push_back(9);
	parser_table[1]->right.push_back(16);
	parser_table[1]->right.push_back(17);
	// 2
	parser_table[2]->right.push_back(19);
	parser_table[2]->right.push_back(8);
	parser_table[2]->right.push_back(ParserItem::c_vn_);
	parser_table[2]->right.push_back(9);
	parser_table[2]->right.push_back(16);
	parser_table[2]->right.push_back(17);
	parser_table[2]->right.push_back(20);
	parser_table[2]->right.push_back(16);
	parser_table[2]->right.push_back(17);
	// 3
	parser_table[3]->right.push_back(21);
	parser_table[3]->right.push_back(8);
	parser_table[3]->right.push_back(ParserItem::c_vn_);
	parser_table[3]->right.push_back(9);
	parser_table[3]->right.push_back(16);
	parser_table[3]->right.push_back(17);
	// 4
	parser_table[4]->right.push_back(22);
	parser_table[4]->right.push_back(ParserItem::c_vn_);
	parser_table[4]->right.push_back(14);
	// 5
	parser_table[5]->right.push_back(23);
	parser_table[5]->right.push_back(1);
	parser_table[5]->right.push_back(26);
	parser_table[5]->right.push_back(1);
	parser_table[5]->right.push_back(14);
	// 6
	parser_table[6]->right.push_back(24);
	parser_table[6]->right.push_back(1);
	parser_table[6]->right.push_back(26);
	parser_table[6]->right.push_back(1);
	parser_table[6]->right.push_back(14);
	// 7
	parser_table[7]->right.push_back(26);
	parser_table[7]->right.push_back(2);
	parser_table[7]->right.push_back(ParserItem::c_vn_);
	// 8
	parser_table[8]->right.push_back(26);
	parser_table[8]->right.push_back(2);
	parser_table[8]->right.push_back(0);
	parser_table[8]->right.push_back(ParserItem::c_vn_);
	parser_table[8]->right.push_back(0);
	// 9
	parser_table[9]->right.push_back(ParserItem::c_vn_);
	parser_table[9]->right.push_back(3);
	parser_table[9]->right.push_back(ParserItem::c_vn_);
	// 10
	parser_table[10]->right.push_back(ParserItem::c_vn_);
	parser_table[10]->right.push_back(4);
	parser_table[10]->right.push_back(ParserItem::c_vn_);
	// 11
	parser_table[11]->right.push_back(ParserItem::c_vn_);
	parser_table[11]->right.push_back(5);
	parser_table[11]->right.push_back(ParserItem::c_vn_);
	// 12
	parser_table[12]->right.push_back(ParserItem::c_vn_);
	parser_table[12]->right.push_back(6);
	parser_table[12]->right.push_back(ParserItem::c_vn_);
	// 13
	parser_table[13]->right.push_back(ParserItem::c_vn_);
	parser_table[13]->right.push_back(7);
	parser_table[13]->right.push_back(ParserItem::c_vn_);
	// 14
	parser_table[14]->right.push_back(8);
	parser_table[14]->right.push_back(ParserItem::c_vn_);
	parser_table[14]->right.push_back(9);
	// 15
	parser_table[15]->right.push_back(25);
	// 16
	parser_table[16]->right.push_back(26);
	// 17
	parser_table[17]->right.push_back(18);
	parser_table[17]->right.push_back(26);
	// 18
	parser_table[18]->right.push_back(26);
	parser_table[18]->right.push_back(8);
	parser_table[18]->right.push_back(ParserItem::c_vn_);
	parser_table[18]->right.push_back(9);
	// 19
	parser_table[19]->right.push_back(26);
	parser_table[19]->right.push_back(8);
	parser_table[19]->right.push_back(9);
	// 20
	parser_table[20]->right.push_back(ParserItem::c_vn_);
	parser_table[20]->right.push_back(15);
	parser_table[20]->right.push_back(ParserItem::c_vn_);
	// 21
	parser_table[21]->right.push_back(ParserItem::c_vn_);
	parser_table[21]->right.push_back(10);
	parser_table[21]->right.push_back(ParserItem::c_vn_);
	// 22
	parser_table[22]->right.push_back(ParserItem::c_vn_);
	parser_table[22]->right.push_back(11);
	parser_table[22]->right.push_back(ParserItem::c_vn_);
	// 23
	parser_table[23]->right.push_back(ParserItem::c_vn_);
	parser_table[23]->right.push_back(12);
	parser_table[23]->right.push_back(ParserItem::c_vn_);
	// 24
	parser_table[24]->right.push_back(13);
	parser_table[24]->right.push_back(ParserItem::c_vn_);
#ifdef TEST_BLOCK_7
	// test block
	{
		printf("----------  test block #7 : BEGIN ----------\n\n");
		printf("# parser: BEGIN #\n\n");
		for (int64_t i = 0; i < parser_table.size(); ++i)
		{
			printf("[% 3I64d] % 3I64d ---> ", i, parser_table[i]->left);
			for (int64_t j = 0; j < parser_table[i]->right.size(); ++j)
			{
				printf("% 3I64d ", parser_table[i]->right[j]);
			}
			printf("\n");
		}
		printf("\n# parser: END #\n\n");
		printf("----------  test block #7 : END ----------\n\n");
		system("PAUSE");
	}
#endif
	std::vector<int64_t> symbol_stack;
	Word * word_p;
	int64_t stack_top_vt;
	int64_t next_vt;
	int64_t vt_index_1;
	int64_t vt_index_2;
	int64_t phrase_beginning_index;
	int64_t phrase_end_index;
	int64_t phrase_length;
	int64_t selected_phrase_index;
	bool matched;
	bool first_phrase = true;
	bool end_immediate = false;
	//prioritized operators algorithm
	word_p = block_p->word_header.next_;
	if (NULL == word_p)
	{
		return 1;
	}
	// push '@'
	symbol_stack.push_back(27);
	while (false == end_immediate)
	{
		// step 1: read symbol
		if (word_p != NULL)
		{
			for (int64_t i = 0; i < symbol_stack.size(); ++i)
			{
				if (symbol_stack[i] >= 0)
				{
					stack_top_vt = symbol_stack[i];
				}
			}
			next_vt = ParseBlock_GetSymbol(word_p);
			while (priority_table[stack_top_vt][next_vt] != '>')
			{
				if (priority_table[stack_top_vt][next_vt] == 'u')
				{
					// error
					error_p->major_no_ = 4;
					error_p->minor_no_ = 1;
					if (NULL != word_p)
					{
						source_file_p->JumpTo(word_p->source_file_index_);
					}
					return -1;
				}
				stack_top_vt = next_vt;
				symbol_stack.push_back(stack_top_vt);
				word_p = word_p->next_;
				if (word_p != NULL)
				{
					next_vt = ParseBlock_GetSymbol(word_p);
				}
			}
		}
		// step 2: locate leftmost prime phrase
		// Tips: symbol_stack[0] == 27
		for (vt_index_2 = symbol_stack.size() - 1; ParserItem::c_vn_ == symbol_stack[vt_index_2]; --vt_index_2)
			;
		while (true)
		{
			if (0 == vt_index_2)
			{
				end_immediate = true;
				break;
			}
			for (vt_index_1 = vt_index_2 - 1; ParserItem::c_vn_ == symbol_stack[vt_index_1]; --vt_index_1)
				;
			if ('<' == priority_table[symbol_stack[vt_index_1]][symbol_stack[vt_index_2]])
			{
				break;
			}
			vt_index_2 = vt_index_1;
		}
		if (end_immediate)
		{
			break;
		}
		phrase_beginning_index = vt_index_1 + 1;
		phrase_end_index = symbol_stack.size() - 1;
		phrase_length = phrase_end_index - phrase_beginning_index + 1;
		// step 3: select a parser to reduct
		for (int64_t i = 0; i < 25; ++i)
		{
			matched = false;
			if (parser_table[i]->right.size() == phrase_length)
			{
				matched = true;
				for (int64_t j = 0; j < phrase_length; ++j)
				{
					if (parser_table[i]->right[j] != symbol_stack[phrase_beginning_index + j])
					{
						matched = false;
						break;
					}
				}
			}
			if (matched)
			{
				selected_phrase_index = i;
				break;
			}
		}
		if (false == matched)
		{
			// error
		}
		// pop
		for (int64_t i = 0; i < phrase_length; ++i)
		{
			symbol_stack.pop_back();
		}
		// push
		if (selected_phrase_index != 0)
		{
			symbol_stack.push_back(ParserItem::c_vn_);
		}
		// step 4: semantic processing
	}
	return 1;
}

int64_t ParseBlock_GetSymbol(Word * word_p)
{
	if (NULL == word_p)
	{
		throw std::exception("Function \"int64_t ParseBlock_GetSymbol(Word * word_p)\" says: Invalid parameter \"word_p\".");
	}
	switch (word_p->type_)
	{
	case Word::c_identifier_:
		return 26;
	case Word::c_constant_int_:
		return 25;
	default:
		switch (word_p->content_[0])
		{
		case '$':
			if ('$' == word_p->content_[1])
			{
				// $$
				return 1;
			}
			else
			{
				// $
				return 0;
			}
		case '=':
			if ('=' == word_p->content_[1])
			{
				// ==
				return 12;
			}
			else
			{
				// =
				return 2;
			}
		case '+':
			return 3;
			break;
		case '-':
			return 4;
		case '*':
			return 5;
		case '/':
			return 6;
		case '%':
			return 7;
		case '(':
			return 8;
		case ')':
			return 9;
		case '>':
			return 10;
		case '<':
			return 11;
		case '!':
			return 13;
		case ';':
			return 14;
		case ',':
			return 15;
		case '{':
			return 16;
		case '}':
			return 17;
		default:
			if (0 == strcmp(word_p->content_, "int"))
			{
				return 18;
			}
			else if (0 == strcmp(word_p->content_, "if"))
			{
				return 19;
			}
			else if (0 == strcmp(word_p->content_, "else"))
			{
				return 20;
			}
			else if (0 == strcmp(word_p->content_, "while"))
			{
				return 21;
			}
			else if (0 == strcmp(word_p->content_, "return"))
			{
				return 22;
			}
			else if (0 == strcmp(word_p->content_, "input"))
			{
				return 23;
			}
			else if (0 == strcmp(word_p->content_, "output"))
			{
				return 24;
			}
			else
			{
				throw std::exception("Function \"int64_t ParseBlock_GetSymbol(Word * word_p)\" says: Invalid word. There is something wrong in function \"int64_t LexicalAnalyse(SourceFile * source_file_p, Error * error_p, std::vector<Block *> * block_pointer_table_p, bool is_block, void * pointer)\".");
			}
		}
	}
	word_p = word_p->next_;
}
