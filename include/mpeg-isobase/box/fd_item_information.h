#ifndef __MPEG_ISOBASE_FD_ITEM_INFORMATION_BOX__
#define __MPEG_ISOBASE_FD_ITEM_INFORMATION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct FDItemInformationBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'fiin';
    static constexpr const char *box_name = "FDItemInformationBox";

    UInt16<> entry_count;
    //PartitionEntry partition_entries[ entry_count ];
    //FDSessionGroupBox session_info; //optional
    //GroupIdToNameBox group_id_to_name; //optional

    FDItemInformationBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    FDItemInformationBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    //virtual void parse_payload(Box::Parser &parser, Remainder left) {
    //    // by default payload is parsed as contained boxes
    //    auto count = parse_subboxes(left, {&PartitionEntry});
    //    if (!parser.ignore_wrong_fd_item_information_partition_entry_count && count != entry_count) {
    //        throw Parser::Exception(SStream() << "Wrong number of FDItemInformationBox::entry_count(" << (uint32_t)entry_count << ") but got " << count);
    //    }
    //}
    //
    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_FD_ITEM_INFORMATION_BOX__

