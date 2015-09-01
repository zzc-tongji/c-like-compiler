#ifndef SOURCE_FILE_H_
#define SOURCE_FILE_H_

#include <stdint.h>
#include <vector>

class SourceFile
{
public:
	SourceFile();
	~SourceFile();
	void * Malloc(int64_t size);
	void Free();
	void ReadyToMove();
	int64_t JumpTo(int64_t location);
	int64_t MoveNext();
	// content
	char * content_;
	int64_t content_size_;
	int64_t index_;
	// line
	std::vector<int64_t> line_table_;
	int64_t line_size_;
	int64_t line_index_;
	int64_t line_;
	// function
	std::vector<int64_t> function_table_;
	int64_t function_size_;
	int64_t function_index_;
	int64_t function_;
	// annotation
	std::vector<std::pair<int64_t, int64_t>> annotation_table_;
	int64_t annotation_size_;
	int64_t annotation_index_;
	bool annotation_;
private:
	// lock of moving
	bool move_enabled_;
};

SourceFile::SourceFile()
{
	content_ = NULL;
	move_enabled_ = false;
}

SourceFile::~SourceFile()
{
	Free();
}

void * SourceFile::Malloc(int64_t size)
{
	Free();
	content_ = new char[size];
	return content_;
}

void SourceFile::Free()
{
	if (NULL != content_)
	{
		delete[] content_;
		content_ = NULL;
	}
}

void SourceFile::ReadyToMove()
{
	// unlock
	move_enabled_ = true;
	// jump to the beginning
	JumpTo(0);
}

int64_t SourceFile::JumpTo(int64_t location)
{
	if (false == move_enabled_)
	{
		return -1;
	}
	if (location < 0 || location > content_size_ - 1)
	{
		return 0;
	}
	// content index
	index_ = location;
	// line
	line_index_ = 0;
	for (int64_t i = 0; i < line_size_; ++i)
	{
		if (location >= line_table_[line_index_])
		{
			line_index_ += 1;
		}
		else
		{
			break;
		}
	}
	line_index_ -= 1;
	line_ = line_table_[line_index_];
	// function
	function_index_ = 0;
	for (int64_t i = 0; i < function_size_; ++i)
	{
		if (location >= function_table_[function_index_])
		{
			function_index_ += 1;
		}
		else
		{
			break;
		}
	}
	function_index_ -= 1;
	function_ = function_table_[function_index_];
	// annotation
	annotation_index_ = 0;
	for (int64_t i = 0; i < annotation_size_; ++i)
	{
		if (location >= annotation_table_[annotation_index_].first)
		{
			annotation_index_ += 1;
		}
		else
		{
			break;
		}
	}
	annotation_index_ -= 1;
	if (location <= annotation_table_[annotation_index_].second)
	{
		annotation_ = true;
	}
	else
	{
		annotation_ = false;
	}
	return 1;
}

int64_t SourceFile::MoveNext()
{
	if (false == move_enabled_)
	{
		return -1;
	}
	if (index_ == content_size_ - 1)
	{
		return 0;
	}
	// move
	index_ += 1;
	// line
	if (line_index_ + 1 < line_size_ && index_ >= line_table_[line_index_ + 1])
	{
		line_index_ += 1;
		line_ = line_table_[line_index_];
	}
	// function
	if (function_index_ + 1 < function_size_ && index_ > function_table_[function_index_ + 1])
	{
		function_index_ += 1;
		function_ = function_table_[function_index_];
	}
	// annotation
	if (-2 != annotation_index_)
	{
		if (annotation_)
		{
			if (index_ > annotation_table_[annotation_index_].second)
			{
				// annotation => non-annotation
				annotation_ = false;
			}
		}
		else
		{
			if (annotation_index_ + 1 < annotation_size_ && index_ >= annotation_table_[annotation_index_ + 1].first)
			{
				// non-annotation => annotation
				annotation_index_ += 1;
				annotation_ = true;
			}
		}
	}
	return 1;
}

#endif
