#ifndef __MPEG_ISOBASE_SAMPLE_GROUP_DESCRIPTION_BOX__
#define __MPEG_ISOBASE_SAMPLE_GROUP_DESCRIPTION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleGroupDescriptionBox: FullBox {

    static constexpr Type container_type[] = {'stbl', 'traf'};
    static constexpr Type box_type = 'sgpd';
    static constexpr const char *box_name = "SampleGroupDescriptionBox";

    UInt32<> grouping_type;
    UInt32<> default_length;    // version==1
    UInt32<> default_sample_description_index;    // version>=2
    UInt32<> entry_count;

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void SampleGroupDescriptionBox_(Composer &composer) {
        composer.get(grouping_type);

        if (version == 1) {
            composer.get(default_length);
        }
        if (version >= 2) {
            composer.get(default_sample_description_index);
        }
        composer.get(entry_count);
    }

    SampleGroupDescriptionBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        SampleGroupDescriptionBox_(composer);
    }

    SampleGroupDescriptionBox(Parser &parser): FullBox(parser, Flags(0)) {
        SampleGroupDescriptionBox_(parser);
    }

    // TODO: PROCESS PAYLOAD

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_GROUP_DESCRIPTION_BOX__

