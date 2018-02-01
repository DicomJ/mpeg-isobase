#ifndef __MPEG_ISOBASE_SUB_TRACK_INFORMATION__
#define __MPEG_ISOBASE_SUB_TRACK_INFORMATION__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SubTrackInformation: FullBox {

    static constexpr Type container_type = 'strk';
    static constexpr Type box_type = 'stri';
    static constexpr const char *box_name = "SubTrackInformation";


    union { Footprint<2*16+32> header;
        UInt16<>    switch_group;       // = 0;
        UInt16<16>  alternate_group;    // = 0;
        UInt32<32>  sub_track_ID;       // = 0;
    };
    UInt32<>::Array attribute_list; // to the end of box

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void SubTrackInformation_(Composer &composer) {
        composer.get(header);

        const_field<verify>(switch_group, 0);
        const_field<verify>(alternate_group, 0);
        const_field<verify>(sub_track_ID, 0);
    }

    template <typename AttributeList>
    SubTrackInformation(Composer &composer, const AttributeList attribute_list, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        SubTrackInformation_(composer);
        composer.get(this->attribute_list, attribute_list);
    }

    SubTrackInformation(Parser &parser): FullBox(parser, Flags(0)) {
        SubTrackInformation_(parser);
        parser.get(*this, attribute_list);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SUB_TRACK_INFORMATION__

