#ifndef __MPEG_ISOBASE_ITEM_LOCATION_BOX__
#define __MPEG_ISOBASE_ITEM_LOCATION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ItemLocationBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'iloc';
    static constexpr const char *box_name = "ItemLocationBox";

    union { Footprint<4 *4 + 16> header;
        UInt<4,  0> offset_size;
        UInt<4,  4> length_size;
        UInt<4,  8> base_offset_size;
        UInt<4, 12> index_size;     // ((version == 1) || (version == 2)
        UInt<4, 12> reserved;
        UInt16< 16> item_count;
    };

    ItemLocationBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(header);
    }

    ItemLocationBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(header);
    }

    // TODO: PROCESS PAYLOAD

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ITEM_LOCATION_BOX__

