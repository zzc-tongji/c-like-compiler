#ifndef ERROR_H_
#define ERROR_H_

#include <stdlib.h>
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
		sprintf(error_string_, "[0x%04I64X] Fail to match \"{\" and \"}\".", error_no_);
		break;
	case 0x1002:
		sprintf(error_string_, "[0x%04I64X] Fail to acquire enough memory.", error_no_);
		break;
	case 0x2001:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} Can not identify the word.", error_no_, display_line_, display_offset_);
		break;
	case 0x2002:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} There is an illegal character.", error_no_, display_line_, display_offset_);
		break;
	case 0x2003:
		sprintf(error_string_, "[0x%04I64X] Fail to acquire enough memory.", error_no_);
		break;
	case 0x3001:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} The function head is empty.", error_no_, display_line_, display_offset_);
		break;
	case 0x3002:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} The type of returned value should be \"int\" or \"void\".", error_no_, display_line_, display_offset_);
		break;
	case 0x3003:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} There is an unexpected ending.", error_no_, display_line_, display_offset_);
		break;
	case 0x3004:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} The function name should be an identifier.", error_no_, display_line_, display_offset_);
		break;
	case 0x3005:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} There should be a \"(\".", error_no_, display_line_, display_offset_);
		break;
	case 0x3006:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} The type of parameter should be \"int\".", error_no_, display_line_, display_offset_);
		break;
	case 0x3007:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} The parameter name should be an identifier.", error_no_, display_line_, display_offset_);
		break;
	case 0x3008:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} There should be a \")\" or \",\".", error_no_, display_line_, display_offset_);
		break;
	case 0x3009:
		sprintf(error_string_, "[0x%04I64X] Can not find function \"main\".", error_no_);
		break;
	case 0x300A:
		sprintf(error_string_, "[0x%04I64X] Fail to acquire enough memory.", error_no_);
		break;
	case 0x4001:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} There is a syntax error.", error_no_, display_line_, display_offset_);
		break;
	case 0x4002:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} This variable has not been defined.", error_no_, display_line_, display_offset_);
		break;
	case 0x4003:
		sprintf(error_string_, "[0x%04I64X] {Line: %I64d, offset: %I64d} This function has not been defined.", error_no_, display_line_, display_offset_);
		break;
	case 0x4004:
		sprintf(error_string_, "[0x%04I64X] Fail to acquire enough memory.", error_no_);
		break;
	case 0x5001:
		sprintf(error_string_, "[0x%04I64X] Fail to open intermediate file.", error_no_);
		break;
	default:
		error_string_[0] = '\0';
		break;
	}
	return error_string_;
}

#endif
