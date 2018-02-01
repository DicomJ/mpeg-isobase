#ifndef __MPEG_ISOBASE_SUBTITLE_MEDIA_HEADER_BOX__
#define __MPEG_ISOBASE_SUBTITLE_MEDIA_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SubtitleMediaHeaderBox: FullBox {

    static constexpr Type container_type = 'minf';
    static constexpr Type box_type = 'sthd';
    static constexpr const char *box_name = "SubtitleMediaHeaderBox";

    SubtitleMediaHeaderBox(Composer &composer, Version version = 0, Flags flags = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {}
    SubtitleMediaHeaderBox(Parser &parser): FullBox(parser) {}
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SUBTITLE_MEDIA_HEADER_BOX__

