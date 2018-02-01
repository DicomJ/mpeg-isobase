#ifndef __MPEG_ISOBASE_SAMPLE_SIZE_BOX__
#define __MPEG_ISOBASE_SAMPLE_SIZE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleSizeBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stsz';
    static constexpr const char *box_name = "SampleSizeBox";

    union { Footprint<2 * 32> header;
        UInt32< 0> sample_size;
        UInt32<32> sample_count;
    };

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<SampleSizeBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<SampleSizeBox, Entry>;
    };

    SampleSizeBox(Composer &composer, uint32_t sample_size, uint32_t sample_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(header);
        this->sample_size = sample_size;
        this->sample_count = sample_count;
    }

    SampleSizeBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(header);

        assert(sample_size == 0 || parser.box_payload_size(*this) / sizeof(uint32_t) == sample_count);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct SampleSizeBox::Entry : Box::Entry {
    union { Footprint<32> header;
        UInt32<0> entry_size;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const SampleSizeBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_SIZE_BOX__

