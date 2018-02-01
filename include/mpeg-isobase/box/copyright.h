#ifndef __MPEG_ISOBASE_COPYRIGHT_BOX__
#define __MPEG_ISOBASE_COPYRIGHT_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct CopyrightBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'cprt';
    static constexpr const char *box_name = "CopyrightBox";

    union { Footprint<1+ 5 * 3> header;
        UInt<1,  0>                   pad;          // = 0;
        UInt<5,  1>::Static::Array<3> language;     // ISO-639-2/T language code
    };
    String notice;

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void CopyrightBox_(Composer &composer) {
        composer.get(header);
        const_field<verify>(pad, 0);
    }

    CopyrightBox(Composer &composer, const std::string &notice, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0)) {
        CopyrightBox_(composer);
        composer.get(this->notice, notice);
    }

    CopyrightBox(Parser &parser): FullBox(parser, Flags(0)) {
        CopyrightBox_(parser);
        parser.get(*this, notice);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_COPYRIGHT_BOX__

