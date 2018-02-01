#ifndef __MPEG_ISOBASE_BINARY_XML_BOX__
#define __MPEG_ISOBASE_BINARY_XML_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct BinaryXMLBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'bxml';
    static constexpr const char *box_name = "BinaryXMLBox";

    UInt8<>::Array data;

    BinaryXMLBox(Composer &composer, const std::vector<uint8_t> &data, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->data, data);
    }

    BinaryXMLBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(*this, data);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_BINARY_XML_BOX__

