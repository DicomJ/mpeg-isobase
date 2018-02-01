#ifndef __MPEG_ISOBASE_DATA_INFORMATION_BOX__
#define __MPEG_ISOBASE_DATA_INFORMATION_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct DataInformationBox: Box {

    static constexpr Type container_type[] = {'minf', 'meta'};
    static constexpr Type box_type = 'dinf';
    static constexpr const char *box_name = "DataInformationBox";

    DataInformationBox(Composer &composer, bool largesize = false): Box(composer, largesize) {}
    DataInformationBox(Parser &parser): Box(parser) {}
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_DATA_INFORMATION_BOX__

