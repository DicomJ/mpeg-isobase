#ifndef __MPEG_ISOBASE_VIDEO_MEDIA_HEADER_BOX__
#define __MPEG_ISOBASE_VIDEO_MEDIA_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct VideoMediaHeaderBox: FullBox {

    static constexpr Type container_type = 'minf';
    static constexpr Type box_type = 'vmhd';
    static constexpr const char *box_name = "VideoMediaHeaderBox";

    union { Footprint<4 * 16>           header;
        UInt16< 0>                      graphicsmode;    // copy
        UInt16<16>::Static::Array<3>    opcolor;         // = 0;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void VideoMediaHeaderBox_(Composer &composer) {
        composer.get(header);

        const_field<verify>(graphicsmode, 0);
        const_field<verify>(opcolor, {0, 0, 0});
    }

    VideoMediaHeaderBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(1), largesize) {
        VideoMediaHeaderBox_(composer);
    }

    VideoMediaHeaderBox(Parser &parser): FullBox(parser, Flags(1)) {
        VideoMediaHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_VIDEO_MEDIA_HEADER_BOX__

