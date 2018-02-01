#ifndef __MPEG_ISOBASE_FILE_TYPE_BOX__
#define __MPEG_ISOBASE_FILE_TYPE_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct FileTypeBox: Box {

    static constexpr Type box_type = 'ftyp';
    static constexpr const char *box_name = "FileTypeBox";

    union { Footprint<64> header;
        UInt32< 0> major_brand;
        UInt32<32> minor_version;
    };
    UInt32<>::Array compatible_brands; // to end of the box

    template<typename CompatibleBrands>
    FileTypeBox(Composer &composer, const CompatibleBrands &compatible_brands, bool largesize = false): Box(composer, box_type, largesize) {
        composer.get(header);
        composer.get(this->compatible_brands, compatible_brands);
    }

    FileTypeBox(Parser &parser) : Box(parser) {
        parser.get(header);
        parser.get(*this, compatible_brands);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_FILE_TYPE_BOX__

