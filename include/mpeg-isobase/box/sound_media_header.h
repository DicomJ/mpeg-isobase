#ifndef __MPEG_ISOBASE_SOUND_MEDIA_HEADER_BOX__
#define __MPEG_ISOBASE_SOUND_MEDIA_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SoundMediaHeaderBox: FullBox {

    static constexpr Type container_type = 'minf';
    static constexpr Type box_type = 'smhd';
    static constexpr const char *box_name = "SoundMediaHeaderBox";

    union { Footprint<2 * 16> header;
         Int16< 0> balance;     // = 0;
        UInt16<16> reserved;    // = 0;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void SoundMediaHeaderBox_(Composer &composer) {
        composer.get(header);

        const_field<verify>(balance, 0);
        const_field<verify>(reserved, 0);
    }

    SoundMediaHeaderBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        SoundMediaHeaderBox_(composer);
    }

    SoundMediaHeaderBox(Parser &parser): FullBox(parser, Flags(0)) {
        SoundMediaHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SOUND_MEDIA_HEADER_BOX__

