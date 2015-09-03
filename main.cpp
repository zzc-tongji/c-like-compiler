#pragma warning(disable:4244)
#pragma warning(disable:4996)

#include <stdlib.h>
#include <vector>
#include "source_file.h"
#include "error.h"
#include "function_item.h"
#include "block.h"

int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p);
int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem> * function_table_p, Block ** block_root_pp);

int main(int argc, char ** argv)
{
	char path[1024];
	SourceFile source_file;
	Error error;
	std::vector<FunctionItem> function_table;
	Block * block_root_p;

	// test file
	strcpy(path, "example.c.test");

	if (-1 == ReadSourceFile(path, &source_file, &error))
	{
		printf("%s\n", error.get_error_string_());
		system("PAUSE");
		return 0;
	}
	if (-1 == Preprocess(&source_file, &error, &function_table, &block_root_p))
	{
		printf("%s\n", error.get_error_string_());
		system("PAUSE");
		return 0;
	}

	// test block
	{
		int i = 0;
		do
		{
			printf("content:    [% 6I64d] %c\nline:       [% 6I64d] %I64d\nannotation: [% 6I64d] %s\n\n", source_file.index_, source_file.content_[source_file.index_], source_file.line_index_, source_file.line_, source_file.annotation_index_, source_file.annotation_ ? "true" : "false");
			i += 1;
			if (i % 50 == 0)
			{
				system("PAUSE");
			}
		} while (source_file.MoveNext() == 1);
	}

	system("PAUSE");
	return 0;
}

int64_t ReadSourceFile(const char * path, SourceFile * source_file_p, Error * error_p)
{
	FILE * fp = NULL;
	// open file
	fp = fopen(path, "r");
	if (NULL == fp)
	{
		error_p->major_no_ = 0;
		error_p->minor_no_ = 0;
		fp = NULL;
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
	source_file_p->content_[source_file_p->content_size_] = '\0';
	if (NULL == source_file_p->content_)
	{
		error_p->major_no_ = 0;
		error_p->minor_no_ = 2;
		return -1;
	}
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

int64_t Preprocess(SourceFile * source_file_p, Error * error_p, std::vector<FunctionItem> * function_table_p, Block ** block_root_pp)
{
	// stack of '(', '[', '{' and '@' (annotation)
	std::vector<char> parenthesis;
	std::vector<char> bracket;
	std::vector<char> brace;
	std::vector<char> annotation;
	// pair
	std::pair<int64_t, int64_t> annotation_item;
	// block
	Block * block_p;
	Block * block_next_p;
	// line: The first line must be at the beginning of the source file.
	source_file_p->line_table_.push_back(0);
	// function: The first function must be at the beginning of the source file. If not, there must be an function format error. This type of error will be checked out behind.
	function_table_p->push_back(FunctionItem());
	(*function_table_p)[function_table_p->size() - 1].beginning_ = 0;
	// block
	(*block_root_pp) = new Block();
	block_p = *block_root_pp;
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
				// fill in vector "annotation" (1)
				annotation_item.first = source_file_p->index_;
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
					error_p->line_no_ = source_file_p->line_table_.size() - 1;
					return -1;
				}
				annotation.pop_back();
				source_file_p->annotation_ = false;
				// fill in vector "annotation" (2)
				annotation_item.second = source_file_p->index_ + 1;
				source_file_p->annotation_table_.push_back(annotation_item);
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
				error_p->line_no_ = source_file_p->line_table_.size() - 1;
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
				error_p->line_no_ = source_file_p->line_table_.size() - 1;
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
			if (NULL == block_next_p)
			{
				error_p->major_no_ = 1;
				error_p->minor_no_ = 4;
				error_p->line_no_ = source_file_p->line_table_.size() - 1;
				// free
				Block::s_FreeAll(*block_root_pp);
				return -1;
			}
			block_next_p->SetName();
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
				error_p->line_no_ = source_file_p->line_table_.size() - 1;
				// free
				Block::s_FreeAll(*block_root_pp);
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
				(*function_table_p)[function_table_p->size() - 1].end_ = source_file_p->index_;
				function_table_p->push_back(FunctionItem());
				(*function_table_p)[function_table_p->size() - 1].beginning_ = source_file_p->index_ + 1;

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
		error_p->line_no_ = source_file_p->line_table_.size() - 1;
		return -1;
	}
	if (false == bracket.empty())
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 2;
		error_p->line_no_ = source_file_p->line_table_.size() - 1;
		return -1;
	}
	if (false == brace.empty())
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 3;
		error_p->line_no_ = source_file_p->line_table_.size() - 1;
		// free
		Block::s_FreeAll(*block_root_pp);
		return -1;
	}
	if (false == annotation.empty())
	{
		error_p->major_no_ = 1;
		error_p->minor_no_ = 0;
		error_p->line_no_ = source_file_p->line_table_.size() - 1;
		return -1;
	}
	// link functions with blocks
	for (block_p = (*block_root_pp)->child_; block_p != NULL; block_p = block_p->brother_)
	{
		for (int64_t i = 0; i < function_table_p->size(); ++i)
		{
			if (block_p->end_ == (*function_table_p)[i].end_)
			{
				block_p->function_ = &(*function_table_p)[i];
				(*function_table_p)[i].block_tree = block_p;
				break;
			}
		}
	}
	// get ready
	source_file_p->ReadyToMove();
	return 1;
}
