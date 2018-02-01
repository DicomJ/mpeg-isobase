#ifndef __MPEG_ISOBASE_SCHEME_TYPE_BOX__
#define __MPEG_ISOBASE_SCHEME_TYPE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SchemeTypeBox: FullBox {

    static constexpr Type container_type[] = {'sinf', 'srpp'};
    static constexpr Type box_type = 'schm';
    static constexpr const char *box_name = "SchemeTypeBox";

    union { Footprint<2 * 32> header;
        UInt32< 0> scheme_type;
        UInt32<32> scheme_version;
    };
    String scheme_uri;

    SchemeTypeBox(Composer &composer, const std::string &scheme_uri, Flags flags = 0, bool largesize = false)
        : FullBox(composer, box_type, Version(0), flags | (scheme_uri.empty() ? 0: 0x000001), largesize) {
        composer.get(header);
        if (flags & 0x000001) {
            composer.get(this->scheme_uri, scheme_uri);
        }
    }

    SchemeTypeBox(Parser &parser): FullBox(parser, Version(0)) {
        parser.get(header);
        if (flags & 0x000001) {
            parser.get(*this, scheme_uri);
        }
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SCHEME_TYPE_BOX__

