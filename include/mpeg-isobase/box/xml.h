#ifndef __MPEG_ISOBASE_XML_BOX__
#define __MPEG_ISOBASE_XML_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct XMLBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'xml ';
    static constexpr const char *box_name = "XMLBox";

    String xml;

    XMLBox(Composer &composer, const std::string &xml, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->xml, xml);
    }

    XMLBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(*this, xml);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_XML_BOX__

