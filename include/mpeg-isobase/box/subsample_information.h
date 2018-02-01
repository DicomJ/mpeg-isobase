#ifndef __MPEG_ISOBASE_SUBSAMPLE_INFORMATION_BOX__
#define __MPEG_ISOBASE_SUBSAMPLE_INFORMATION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SubSampleInformationBox: FullBox {

    static constexpr Type container_type[] = {'stbl', 'traf'};
    static constexpr Type box_type = 'subs';
    static constexpr const char *box_name = "SubSampleInformationBox";

    UInt32<> entry_count;

    SubSampleInformationBox(Composer &composer, uint32_t entry_count, Version version = 0, Flags flags = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    SubSampleInformationBox(Parser &parser): FullBox(parser) {
        parser.get(entry_count);
        assert(entry_count > 0);
    }

    // TODO: PROCESS PAYLOAD

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SUBSAMPLE_INFORMATION_BOX__

