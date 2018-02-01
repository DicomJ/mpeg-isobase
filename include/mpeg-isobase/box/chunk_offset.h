#ifndef __MPEG_ISOBASE_CHUNK_OFFSET_BOX__
#define __MPEG_ISOBASE_CHUNK_OFFSET_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ChunkOffsetBox: FullBox {

    static constexpr Type container_type = 'stbl';
    static constexpr Type box_type = 'stco';
    static constexpr const char *box_name = "ChunkOffsetBox";

    UInt32<0> entry_count;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<ChunkOffsetBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<ChunkOffsetBox, Entry>;
    };

    ChunkOffsetBox(Composer &composer, uint32_t entry_count, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        composer.get(this->entry_count) = entry_count;
    }

    ChunkOffsetBox(Parser &parser): FullBox(parser, Flags(0)) {
        parser.get(entry_count);
        //assert(entry_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};


struct ChunkOffsetBox::Entry : Box::Entry {
    union { Footprint<32> header;
        UInt32<0> chunk_offset;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const ChunkOffsetBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_CHUNK_OFFSET_BOX__

