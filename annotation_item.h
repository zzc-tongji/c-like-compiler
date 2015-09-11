#ifndef ANNOTATION_ITEM_H_
#define ANNOTATION_ITEM_H_

#include <stdint.h>

class AnnotationItem
{
public:
	static AnnotationItem * s_Malloc();
	static void s_Free(AnnotationItem * annotation_item);
	AnnotationItem();
	int64_t beginning_;
	int64_t end_;
};

AnnotationItem * AnnotationItem::s_Malloc()
{
	return new AnnotationItem();
}

void AnnotationItem::s_Free(AnnotationItem * annotation_item)
{
	if (annotation_item != NULL)
	{
		delete annotation_item;
	}
}

AnnotationItem::AnnotationItem()
{
	beginning_ = -1;
	end_ = -1;
}

#endif
