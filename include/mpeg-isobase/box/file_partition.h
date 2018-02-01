#ifndef __MPEG_ISOBASE_FILE_PARTITION_BOX__
#define __MPEG_ISOBASE_FILE_PARTITION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct FilePartitionBox: FullBox {

    static constexpr Type container_type = 'paen';
    static constexpr Type box_type = 'fpar';
    static constexpr const char *box_name = "FilePartitionBox";

    template <FullBox::Version version, int w = (version == 0 ? 16: 32)>
    struct ItemIDField {
        UInt<w> item_ID;
        ItemIDField() {}

        template <typename Composer>
        void Field(Composer &composer) {
            composer.get(item_ID);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    ItemIDField<0> iiv0;
    ItemIDField<1> iiv1;

    union { Footprint<5*16+2*8> header;
        UInt16<0> packet_payload_size;
        UInt8<16> reserved;   // = 0;
        UInt8<24> FEC_encoding_ID;
        UInt16<32> FEC_instance_ID;
        UInt16<48> max_source_block_length;
        UInt16<64> encoding_symbol_length;
        UInt16<80> max_number_of_encoding_symbols;
    };
    String scheme_specific_info;

    template <FullBox::Version version, int w = (version == 0 ? 16: 32)>
    struct EntryCountField {
        UInt<w> entry_count;
        EntryCountField() {}

        template <typename Composer>
        void Field(Composer &composer) {
            composer.get(entry_count);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    EntryCountField<0> v0;
    EntryCountField<1> v1;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<FilePartitionBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<FilePartitionBox, Entry>;
    };

    uint32_t item_ID() const        { return version == 0 ? uint32_t(iiv0.item_ID):     uint32_t(iiv1.item_ID);     }
    uint32_t entry_count() const    { return version == 0 ? uint32_t(v0.entry_count):   uint32_t(v1.entry_count);   }

    template <typename Composer, typename SchemeSpecificInfo, bool verify = std::is_base_of<Parser, Composer>::value>
    void FilePartitionBox_(Composer &composer, const SchemeSpecificInfo &scheme_specific_info) {
        if (version == 0) {
            iiv0.Field(composer);
        } else {
            iiv1.Field(composer);
        }

        composer.get(header);
        scheme_specific_info();

        if (version == 0) {
            v0.Field(composer);
        } else {
            v1.Field(composer);
        }
    }

    FilePartitionBox(Composer &composer, const std::string &scheme_specific_info, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        FilePartitionBox_(composer, [&]() {
            composer.get(this->scheme_specific_info, scheme_specific_info);
        });
    }

    FilePartitionBox(Parser &parser): FullBox(parser, Flags(0)) {
        FilePartitionBox_(parser, [&] () {
            parser.get(*this, scheme_specific_info);
        });
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
    virtual void parse_payload(Remainder left_payload);
};

struct FilePartitionBox::Entry : Box::Entry {
    union { Footprint<16 + 32> header;
        UInt16< 0> block_count;
        UInt32<16> block_size;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const FilePartitionBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_FILE_PARTITION_BOX__

