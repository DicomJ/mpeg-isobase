#ifndef __MPEG_ISOBASE_MOVIE_FRAGMENT_HEADER_BOX__
#define __MPEG_ISOBASE_MOVIE_FRAGMENT_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct MovieFragmentHeaderBox: FullBox {

    static constexpr Type container_type = 'moof';
    static constexpr Type box_type = 'mfhd';
    static constexpr const char *box_name = "MovieFragmentHeaderBox";

    UInt32<> sequence_number;

    MovieFragmentHeaderBox(Composer &composer, bool largesize = false): FullBox(composer, box_type, Version(0), Flags(0), largesize) {
        composer.get(sequence_number);
    }

    MovieFragmentHeaderBox(Parser &parser): FullBox(parser, Version(0), Flags(0)) {
        parser.get(sequence_number);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_MOVIE_FRAGMENT_HEADER_BOX__

