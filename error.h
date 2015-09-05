#ifndef ERROR_H_
#define ERROR_H_

#include <stdint.h>
#include <stdio.h>
#include <exception>
#include "source_file.h"

class Error
{
public:
	// function
	Error();
	char * GetErrorString(SourceFile * source_file_p);
	// variable
	int64_t major_no_;
	int64_t minor_no_;
private:
	// variable
	int64_t error_no_;
	int64_t display_line_;
	int64_t display_offset_;
	char error_string_[1024];
};

Error::Error()
{
	major_no_ = -1;
	minor_no_ = -1;
	error_no_ = -1;
	display_line_ = -1;
	display_offset_ = -1;
	memset(error_string_, 0, 1024);
}

char * Error::GetErrorString(SourceFile * source_file_p)
{
	if (NULL == source_file_p)
	{
		throw std::exception("Function \"char * Error::GetErrorString(SourceFile * source_file_p)\" says: Invalid parameter \"source_file_p\".");
	}
	error_no_ = (major_no_ << 12) + minor_no_;
	display_line_ = source_file_p->line_index_ + 1;
	display_offset_ = source_file_p->index_ - source_file_p->line_ + 1;
	switch (error_no_)
	{
	case 0x0000:
		sprintf(error_string_, "[0x%04I64X] Fail to open source file.", error_no_);
		break;
	case 0x0001:
		sprintf(error_string_, "[0x%04I64X] The source file is empty.", error_no_);
		break;
	case 0x0002:
		sprintf(error_string_, "[0x%04I64X] Fail to acquire enough memory.", error_no_);
		break;
	case 0x0003:
		sprintf(error_string_, "[0x%04I64X] Fail to read source file.", error_no_);
		break;
	case 0x1000:
		sprintf(error_string_, "[0x%04I64X] Fail to match \"\x2F\x2A\" and \"\x2A\x2F\" (annotation tag).", error_no_);
		break;
	case 0x1001:
		sprintf(error_string_, "[0x%04I64X] Fail to match \"(\" and \")\".", error_no_);
		break;
	case 0x1002:
		sprintf(error_string_, "[0x%04I64X] Fail to match \"[\" and \"]\".", error_no_);
		break;
	case 0x1003:
		sprintf(error_string_, "[0x%04I64X] Fail to match \"{\" and \"}\".", error_no_);
		break;
	case 0x1004:
		sprintf(error_string_, "[0x%04I64X] Fail to acquire enough memory.", error_no_);
		break;
	default:
		error_string_[0] = '\0';
		break;
	}
	return error_string_;
}

#endif
