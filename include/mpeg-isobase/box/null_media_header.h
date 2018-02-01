#ifndef __MPEG_ISOBASE_NULL_MEDIA_HEADER_BOX__
#define __MPEG_ISOBASE_NULL_MEDIA_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct NullMediaHeaderBox: FullBox {

    static constexpr Type container_type = 'minf';
    static constexpr Type box_type = 'nmhd';
    static constexpr const char *box_name = "NullMediaHeaderBox";

    NullMediaHeaderBox(Composer &composer, Version version = 0, Flags flags = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {}
    NullMediaHeaderBox(Parser &parser): FullBox(parser) {}
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_NULL_MEDIA_HEADER_BOX__

