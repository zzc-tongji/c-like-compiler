#ifndef SOURCE_FILE_H_
#define SOURCE_FILE_H_

#include <stdint.h>
#include <vector>

class SourceFile
{
public:
	// function
	SourceFile();
	~SourceFile();
	void * Malloc(int64_t size);
	void Free();
	void ReadyToMove();
	int64_t MoveNext();
	// active variable
	int64_t current_content_index_;
	int64_t current_line_no_;
	int64_t current_line_index_;
	int64_t current_function_no_;
	int64_t current_function_index_;
	int64_t annotation_index_;
	bool annotation_now_;
	// non-active variable
	char * content_;
	int64_t content_size_;
	std::vector<int64_t> line_;
	std::vector<int64_t> function_; // Note: index of the beginning and the end of each function_ body (symbol "{" and "}")
	std::vector<std::pair<int64_t, int64_t>> annotation_;
	bool move_enabled_;
};

SourceFile::SourceFile()
{
	content_ = NULL;
	content_size_ = 0;
	current_content_index_ = 0;
	current_line_index_ = -1;
	current_line_no_ = -1;
	current_function_index_ = -1;
	current_function_no_ = -1;
	annotation_index_ = -1;
	annotation_now_ = false;
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
	}
}

void SourceFile::ReadyToMove()
{
	current_content_index_ = 0;
	current_line_index_ = 0;
	current_function_index_ = 0;
	if (annotation_.empty())
	{
		annotation_index_ = -2;
		annotation_now_ = false;
	}
	else
	{
		annotation_index_ = -1;
		if (0 == annotation_[0].first)
		{
			// The source file begins with annotation.
			annotation_now_ = true;
		}
		else
		{
			// The source file begins with non-annotation.
			annotation_now_ = false;
		}
	}
	move_enabled_ = true;
}

int64_t SourceFile::MoveNext()
{
	if (false == move_enabled_)
	{
		return -1;
	}
	if (current_content_index_ == content_size_ - 1)
	{
		return 0;
	}
	// move
	current_content_index_ += 1;
	// line
	if (current_line_index_ + 1 < line_.size() && current_content_index_ >= line_[current_line_index_ + 1])
	{
		current_line_index_ += 1;
		current_line_no_ = line_[current_line_index_];
	}
	// function
	if (current_function_index_ + 1 < function_.size() && current_content_index_ > function_[current_function_index_ + 1])
	{
		current_function_index_ += 1;
		current_function_no_ = function_[current_function_index_];
	}
	// annotation
	if (-2 != annotation_index_)
	{
		if (annotation_now_)
		{
			if (current_content_index_ > annotation_[annotation_index_].second)
			{
				// annotation => non-annotation
				annotation_now_ = false;
			}
		}
		else
		{
			if (annotation_index_ + 1 < annotation_.size() && current_content_index_ >= annotation_[annotation_index_ + 1].first)
			{
				// non-annotation => annotation
				annotation_index_ += 1;
				annotation_now_ = true;
			}
		}
	}
	return 1;
}

#endif
