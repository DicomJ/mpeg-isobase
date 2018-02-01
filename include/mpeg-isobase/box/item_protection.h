#ifndef __MPEG_ISOBASE_ITEM_PROTECTION_BOX__
#define __MPEG_ISOBASE_ITEM_PROTECTION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ItemProtectionBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'ipro';
    static constexpr const char *box_name = "ItemProtectionBox";

    UInt16<> protection_count;

    ItemProtectionBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(protection_count);
    }

    ItemProtectionBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(protection_count);
    }

    // TODO: PROCESS PAYLOAD (check count)

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ITEM_PROTECTION_BOX__

