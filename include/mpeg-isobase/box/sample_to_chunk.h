#ifndef __MPEG_ISOBASE_SAMPLE_TO_CHUNK_BOX__
#define __MPEG_ISOBASE_SAMPLE_TO_CHUNK_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SampleToChunkBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stsc';
    static constexpr const char *box_name = "SampleToChunkBox";

    UInt32<0> entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<SampleToChunkBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<SampleToChunkBox, Entry>;
    };

    SampleToChunkBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    SampleToChunkBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct SampleToChunkBox::Entry : Box::Entry {
    union { Footprint<3 * 32> header;
        UInt32< 0> first_chunk;
        UInt32<32> samples_per_chunk;
        UInt32<64> sample_description_index;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const SampleToChunkBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SAMPLE_TO_CHUNK_BOX__

