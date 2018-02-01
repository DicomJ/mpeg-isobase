#ifndef __MPEG_ISOBASE_PRIMARY_ITEM_BOX__
#define __MPEG_ISOBASE_PRIMARY_ITEM_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct PrimaryItemBox: FullBox {

    static constexpr Type container_type = 'meta';
    static constexpr Type box_type = 'pitm';
    static constexpr const char *box_name = "PrimaryItemBox";

    template <FullBox::Version version, int w = (version == 0 ? 32 : 64)>
    struct Header {
        UInt<w> item_ID;
        Header() {}

        template <typename Composer>
        void Header_(Composer &composer) {
            composer.get(item_ID);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    Header<0> v0;
    Header<1> v1;

    uint64_t item_ID() const { return version == 0 ? uint64_t(v0.item_ID): uint64_t(v1.item_ID); }

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void PrimaryItemBox_(Composer &composer) {
        if (version == 0) {
            v0.Header_(composer);
        } else {
            v1.Header_(composer);
        }
    }

    PrimaryItemBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        PrimaryItemBox_(composer);
    }

    PrimaryItemBox(Parser &parser): FullBox(parser, Flags(0)) {
        PrimaryItemBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_PRIMARY_ITEM_BOX__

