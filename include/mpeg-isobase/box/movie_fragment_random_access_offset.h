#ifndef __MPEG_ISOBASE_MOVIE_FRAGMENT_RANDOM_ACCESS_OFFSET_BOX__
#define __MPEG_ISOBASE_MOVIE_FRAGMENT_RANDOM_ACCESS_OFFSET_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct MovieFragmentRandomAccessOffsetBox: FullBox {

    static constexpr Type container_type = 'edts';
    static constexpr Type box_type = 'mfro';
    static constexpr const char *box_name = "MovieFragmentRandomAccessOffsetBox";

    UInt32<0> size;

    MovieFragmentRandomAccessOffsetBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0)) {
        composer.get(size);
    }

    MovieFragmentRandomAccessOffsetBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(size);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_MOVIE_FRAGMENT_RANDOM_ACCESS_OFFSET_BOX__

