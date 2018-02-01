#ifndef __MPEG_ISOBASE_MOVIE_EXTENDS_HEADER_BOX__
#define __MPEG_ISOBASE_MOVIE_EXTENDS_HEADER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct MovieExtendsHeaderBox: FullBox {

    static constexpr Type container_type = 'mvex';
    static constexpr Type box_type = 'mehd';
    static constexpr const char *box_name = "MovieExtendsHeaderBox";

    UInt32<0> fragment_duration32;
    UInt64<0> fragment_duration64;

    uint64_t fragment_duration() const { return version == 1 ? uint64_t(fragment_duration64): uint64_t(fragment_duration32); }

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void MovieExtendsHeaderBox_(Composer &composer) {

        if (version == 1) {
            composer.get(fragment_duration64);
        } else { // version==0
            composer.get(fragment_duration32);
        }

        const_field<verify>(flags, 0);
    }

    MovieExtendsHeaderBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0)) {
        MovieExtendsHeaderBox_(composer);
    }

    MovieExtendsHeaderBox(Parser &parser): FullBox(parser, Flags(0)) {
        MovieExtendsHeaderBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_MOVIE_EXTENDS_HEADER_BOX__

