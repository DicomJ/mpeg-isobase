#ifndef __MPEG_ISOBASE_BOXES__
#define __MPEG_ISOBASE_BOXES__

#include <mpeg-isobase/box_map.h>


namespace mpeg {
namespace isobase {

struct Boxes: Box::Map {

    template <typename Box, typename ...Args>
    void add(const Args &...args) { Box::Map::template add<Box, mpeg::isobase::Box::Parser>(args...); }
};

struct RegisterBoxes : virtual Boxes  {
    RegisterBoxes();
};


}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_BOXES__

