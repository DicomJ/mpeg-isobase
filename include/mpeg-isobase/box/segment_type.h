#ifndef __MPEG_ISOBASE_SEGMENT_TYPE_BOX__
#define __MPEG_ISOBASE_SEGMENT_TYPE_BOX__

#include <mpeg-isobase/box/file_type.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SegmentTypeBox: FileTypeBox {
    static constexpr Type box_type = 'styp';
    static constexpr const char *box_name = "SegmentTypeBox";

    using FileTypeBox::FileTypeBox;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SEGMENT_TYPE_BOX__

