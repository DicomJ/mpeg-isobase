#ifndef __MPEG_ISOBASE_METABOX_RELATION_BOX__
#define __MPEG_ISOBASE_METABOX_RELATION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct MetaboxRelationBox: FullBox {

    static constexpr Type container_type = 'meco';
    static constexpr Type box_type = 'mere';
    static constexpr const char *box_name = "MetaboxRelationBox";


    union { Footprint<2*32+8> header;
        UInt32< 0> first_metabox_handler_type;
        UInt32<32> second_metabox_handler_type;
        UInt8 <64> metabox_relation;
    };

    MetaboxRelationBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(header);
    }

    MetaboxRelationBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_METABOX_RELATION_BOX__

