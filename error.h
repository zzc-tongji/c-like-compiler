#ifndef ERROR_H_
#define ERROR_H_

#include <stdint.h>
#include <stdio.h>

class Error
{
public:
	// function
	Error();
	char * get_error_string_();
	// variable
	int64_t major_no_;
	int64_t minor_no_;
	int64_t line_no_;
private:
	// variable
	int64_t error_no_;
	char error_string_[1024];
};

Error::Error()
{
	major_no_ = -1;
	minor_no_ = -1;
	line_no_ = -1;
	error_no_ = -1;
	memset(error_string_, 0, 1024);
}

char * Error::get_error_string_()
{
	error_no_ = (major_no_ << 12) + minor_no_;
	switch (error_no_)
	{
	case 0x0000:
		sprintf(error_string_, "[0x%04I64X]", error_no_);
		break;
	case 0x0001:
		sprintf(error_string_, "[0x%04I64X]", error_no_);
		break;
	case 0x0002:
		sprintf(error_string_, "[0x%04I64X]", error_no_);
		break;
	case 0x0003:
		sprintf(error_string_, "[0x%04I64X]", error_no_);
		break;
	case 0x1000:
		sprintf(error_string_, "[0x%I64X] Line %I64d", error_no_, line_no_ + 1);
		break;
	case 0x1001:
		sprintf(error_string_, "[0x%I64X] Line %I64d", error_no_, line_no_ + 1);
		break;
	case 0x1002:
		sprintf(error_string_, "[0x%I64X] Line %I64d", error_no_, line_no_ + 1);
		break;
	case 0x1003:
		sprintf(error_string_, "[0x%I64X] Line %I64d", error_no_, line_no_ + 1);
		break;
	default:
		error_string_[0] = '\0';
		break;
	}
	return error_string_;
}

#endif
