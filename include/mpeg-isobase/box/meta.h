#ifndef __MPEG_ISOBASE_META_BOX__
#define __MPEG_ISOBASE_META_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct MetaBox: FullBox {

    static constexpr Type container_type[] = {0, 'moov', 'trak', 'meco', 'moof', 'traf'};
    static constexpr Type box_type = 'meta';
    static constexpr const char *box_name = "MetaBox";

    //HandlerBox(handler_type) theHandler;
    //PrimaryItemBox primary_resource; // optional
    //DataInformationBox file_locations; // optional
    //ItemLocationBox item_locations; // optional
    //ItemProtectionBox protections; // optional
    //ItemInfoBox item_infos; // optional
    //IPMPControlBox IPMP_control; // optional
    //ItemReferenceBox item_refs; // optional
    //ItemDataBox item_data; // optional
    //Box other_boxes[];

    MetaBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0)) {
    }

    MetaBox(Parser &parser): FullBox(parser, Flags(0)) {
    }

    // TODO: PROCESS PAYLOAD

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_META_BOX__

