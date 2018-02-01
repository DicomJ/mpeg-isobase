#ifndef __MPEG_ISOBASE_DATA_ENTRY_URL_BOX__
#define __MPEG_ISOBASE_DATA_ENTRY_URL_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct DataEntryUrlBox: FullBox {

    static constexpr Type container_type = 'dref';
    static constexpr Type box_type = 'url ';
    static constexpr const char *box_name = "DataEntryUrlBox";

    static constexpr Flags SameFileFlag = 0x000001;

    String location;

    DataEntryUrlBox(Composer &composer, const std::string &location = std::string(), Version version = 0, Flags flags = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {
        if (flags & SameFileFlag) {
            assert(location.empty());
        } else {
            composer.get(this->location, location);
        }
    }

    DataEntryUrlBox(Parser &parser): FullBox(parser) {
        //if (!(flags & SameFileFlag)) {    // TODO: make it properly work
            parser.get(*this, location);
        //}
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_DATA_ENTRY_URL_BOX__

