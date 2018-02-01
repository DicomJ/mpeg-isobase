#ifndef __MPEG_ISOBASE_PROGRESSIVE_DOWNLOAD_INFO_BOX__
#define __MPEG_ISOBASE_PROGRESSIVE_DOWNLOAD_INFO_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ProgressiveDownloadInfoBox: FullBox {

    static constexpr Type box_type = 'pdin';
    static constexpr const char *box_name = "ProgressiveDownloadInfoBox";

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<ProgressiveDownloadInfoBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<ProgressiveDownloadInfoBox, Entry>;
    };

    inline uint32_t entries() const;

    ProgressiveDownloadInfoBox(Composer &composer, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0)) {
    }

    ProgressiveDownloadInfoBox(Parser &parser): FullBox(parser, Flags(0)) {
        assert(parser.box_payload_size(*this) % 8 == 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};


struct ProgressiveDownloadInfoBox::Entry : Box::Entry {
    union { Footprint<2 * 32> header;
        UInt32< 0> rate;
        UInt32<32> initial_delay;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}


    template <typename Composer>
    Entry(const ProgressiveDownloadInfoBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};

inline uint32_t ProgressiveDownloadInfoBox::entries() const {
    return parser().box_payload_size(*this) / (decltype(Entry::header)::size / 8);    // to the end of box
}


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_PROGRESSIVE_DOWNLOAD_INFO_BOX__

