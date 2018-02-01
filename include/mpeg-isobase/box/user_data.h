#ifndef __MPEG_ISOBASE_USER_DATA_BOX__
#define __MPEG_ISOBASE_USER_DATA_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct UserDataBox: Box {

    static constexpr Type container_type[] = {'moov', 'trak'};
    static constexpr Type box_type = 'udta';
    static constexpr const char *box_name = "UserDataBox";

    UserDataBox(Composer &composer, bool largesize = false): Box(composer, box_type, largesize) {}
    UserDataBox(Parser &parser): Box(parser) {}
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_USER_DATA_BOX__

