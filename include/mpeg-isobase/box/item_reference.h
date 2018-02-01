#ifndef __MPEG_ISOBASE_ITEM_REFERENCE_BOX__
#define __MPEG_ISOBASE_ITEM_REFERENCE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ItemReferenceBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'iref';
    static constexpr const char *box_name = "ItemReferenceBox";

    ItemReferenceBox(Composer &composer, const std::string &xml, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
    }

    ItemReferenceBox(Parser &parser): FullBox(parser, Flags(0)) {
    }

    // TODO: PROCESS PAYLOAD (Introduce SingleItemTypeReferenceBox(referenceType))
};



}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ITEM_REFERENCE_BOX__

