#ifndef __MPEG_ISOBASE_HANDLER_BOX__
#define __MPEG_ISOBASE_HANDLER_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct HandlerBox: FullBox {

    static constexpr Type container_type[] = {'mdia', 'meta'};
    static constexpr Type box_type = 'hdlr';
    static constexpr const char *box_name = "HandlerBox";

    union { Footprint<1+ 5 * 3 + 16> header;
        UInt32< 0>                      pre_defined;    // = 0;
        UInt32<32>                      handler_type;
        UInt32<64>::Static::Array<3>    reserved;       // = 0;
    };
    String  name;

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void HandlerBox_(Composer &composer) {
        composer.get(header);
        const_field<verify>(pre_defined, 0);
        const_field<verify>(reserved, {0, 0, 0});
    }

    HandlerBox(Composer &composer, const std::string &name, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        HandlerBox_(composer);
        composer.get(this->name, name);
    }

    HandlerBox(Parser &parser): FullBox(parser, Flags(0)) {
        HandlerBox_(parser);
        parser.get(*this, name);

        parser.handler_type = handler_type; // TODO: add code which resets this flag for each trak. OR remove this code at all
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_HANDLER_BOX__

