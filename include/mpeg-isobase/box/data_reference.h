#ifndef __MPEG_ISOBASE_DATA_REFERENCE_BOX__
#define __MPEG_ISOBASE_DATA_REFERENCE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct DataReferenceBox: FullBox {

    static constexpr Type container_type[] = {'dinf', 'minf'};
    static constexpr Type box_type = 'dref';
    static constexpr const char *box_name = "DataReferenceBox";

    UInt32<0> entry_count;

    DataReferenceBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    DataReferenceBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    //virtual void parse_payload(Box::Parser &parser, Remainder left) {
    //    // by default payload is parsed as contained boxes
    //    auto count = parse_subboxes(left);
    //    if (!parser.ignore_wrong_data_reference_entry_count && count != entry_count) {
    //        throw Parser::Exception(SStream() << "Wrong number of DataReferenceBox::entry_count(" << (uint32_t)entry_count << ") but got " << count);
    //    }
    //}

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_DATA_REFERENCE_BOX__

