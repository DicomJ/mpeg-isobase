#ifndef __MPEG_ISOBASE_SCHEME_INFORMATION_BOX__
#define __MPEG_ISOBASE_SCHEME_INFORMATION_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SchemeInformationBox: Box {

    static constexpr Type container_type[] = {'sinf', 'srpp'};
    static constexpr Type box_type = 'schi';
    static constexpr const char *box_name = "SchemeInformationBox";

    //Box scheme_specific_data[];

    SchemeInformationBox(Composer &composer): Box(composer, box_type) {}
    SchemeInformationBox(Parser &parser): Box(parser) {}
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SCHEME_INFORMATION_BOX__

