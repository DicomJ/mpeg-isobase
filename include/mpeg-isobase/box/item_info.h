#ifndef __MPEG_ISOBASE_ITEM_INFO_BOX__
#define __MPEG_ISOBASE_ITEM_INFO_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ItemInfoBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'iinf';
    static constexpr const char *box_name = "ItemInfoBox";

    UInt16<> entry_count16; // version == 0
    UInt32<> entry_count32; // version != 0

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void ItemInfoBox_(Composer &composer) {
        if (version == 0) {
            composer.get(entry_count16);
        } else {
            composer.get(entry_count32);
        }
    }

    ItemInfoBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        ItemInfoBox_(composer);
    }

    ItemInfoBox(Parser &parser): FullBox(parser, Flags(0)) {
        ItemInfoBox_(parser);
    }

    // TODO: PROCESS PAYLOAD (check entry_count)

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ITEM_INFO_BOX__

