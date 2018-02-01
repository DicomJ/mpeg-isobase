#ifndef __MPEG_ISOBASE_ITEM_INFO_ENTRY__
#define __MPEG_ISOBASE_ITEM_INFO_ENTRY__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ItemInfoEntry: FullBox {

    static constexpr Type container_type = 'iinf';
    static constexpr Type box_type = 'infe';
    static constexpr const char *box_name = "ItemInfoEntry";

    // TODO: not up to date structure of the box: these guys are quite crazy with such structures of the box
    union { Footprint<2 * 16> header;
        UInt16< 0> item_ID;
        UInt16<16> item_protection_index;
    };
    String item_name;
    String content_type;
    String content_encoding;

    ItemInfoEntry(Composer &composer,
                  const std::string &item_name,
                  const std::string &content_type,
                  const std::string &content_encoding,
                  Version version, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->item_name, item_name,
                     this->content_type, content_type,
                     this->content_encoding, content_encoding);
    }

    ItemInfoEntry(Parser &parser): FullBox(parser) {
        parser.get(header);
        parser.get(*this, item_name, content_type, content_encoding);

        assert(flags == 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ITEM_INFO_ENTRY__

