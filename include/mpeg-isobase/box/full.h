#ifndef __MPEG_ISOBASE_FULL_BOX__
#define __MPEG_ISOBASE_FULL_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {

struct FullBox: Box {

    union { Footprint<32> header;
        UInt8<   0> version;
        UInt<24, 8> flags;
    };

    using Version = uint8_t;
    using Flags = uint32_t;

    template <typename Type>
    FullBox(Composer &composer, const Type &box_type, Version version, Flags flags, bool largesize = false): Box(composer, box_type, largesize) {
        composer.get(header);
        const_field<false>(this->version, version);
        const_field<false>(this->flags, flags);
    }

    inline FullBox(Parser &parser): Box(parser) {
        parser.get(header);
    }

    inline FullBox(Parser &parser, Version version, Flags flags): FullBox(parser) {
        const_field<true>(this->version, version);
        const_field<true>(this->flags, flags);
    }

    inline FullBox(Parser &parser, Version version): FullBox(parser) {
        const_field<true>(this->version, version);
    }

    inline FullBox(Parser &parser, Flags flags): FullBox(parser) {
        const_field<true>(this->flags, flags);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream_) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_FULL_BOX__

