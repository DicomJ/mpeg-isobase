#ifndef __MPEG_ISOBASE_FILE_RESERVOIR_BOX__
#define __MPEG_ISOBASE_FILE_RESERVOIR_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct FileReservoirBox: FullBox {

    static constexpr Type container_type = 'paen';
    static constexpr Type box_type = 'fire';
    static constexpr const char *box_name = "FileReservoirBox";

    template <FullBox::Version version, int w = (version == 0 ? 16 : 32)>
    struct Header {
        UInt<w> entry_count;
        Header() {}

        template <typename Composer>
        void Header_(Composer &composer) {
            composer.get(entry_count);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    Header<0> v0;
    Header<1> v1;

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<FileReservoirBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<FileReservoirBox, Entry>;
    };

    uint32_t entry_count() const { return version == 0 ? uint32_t(v0.entry_count): uint32_t(v1.entry_count); }

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void FileReservoirBox_(Composer &composer) {
        if (version == 0) {
            v0.Header_(composer);
        } else {
            v1.Header_(composer);
        }
    }

    FileReservoirBox(Composer &composer, Flags flags, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        FileReservoirBox_(composer);
    }

    FileReservoirBox(Parser &parser): FullBox(parser, Flags(0)) {
        FileReservoirBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct FileReservoirBox::Entry : Box::Entry {

    template <FullBox::Version version, int w = (version == 0 ? 16 : 32)>
    struct Header {
        union { Footprint<w + 32> header;
            UInt<w>     item_ID;
            UInt32<w>   symbol_count;
        };
        Header() {}
        Header(const Header &h): header(h.header) {}

        template <typename Composer>
        void Header_(Composer &composer) {
            composer.get(header);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    Header<0> v0;
    Header<1> v1;

    uint64_t item_ID() const        { return box.version == 0 ? uint32_t(v0.item_ID):       uint32_t(v1.item_ID);       }
    uint64_t symbol_count() const   { return box.version == 0 ? uint32_t(v0.symbol_count):  uint32_t(v1.symbol_count);  }

    const FileReservoirBox &box;

    Entry(const Entry &) = default;

    template <typename Composer>
    Entry(const FileReservoirBox &box, Composer &composer): box(box) {
        if (box.version == 0) {
            v0.Header_(composer);
        } else {
            v1.Header_(composer);
        }
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_FILE_RESERVOIR_BOX__

