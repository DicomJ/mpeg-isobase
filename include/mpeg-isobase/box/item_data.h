#ifndef __MPEG_ISOBASE_ITEM_DATA_BOX__
#define __MPEG_ISOBASE_ITEM_DATA_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ItemDataBox: Box {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'idat';
    static constexpr const char *box_name = "ItemDataBox";

    UInt8<>::Array data;

    ItemDataBox(Composer &composer, const std::vector<uint8_t> &data, bool largesize = false): Box(composer, box_type, largesize) {
        composer.get(this->data, data);
    }

    ItemDataBox(Parser &parser): Box(parser) {
        parser.get(*this, data);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ITEM_DATA_BOX__

