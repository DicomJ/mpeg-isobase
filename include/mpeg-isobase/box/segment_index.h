#ifndef __MPEG_ISOBASE_SEGMENT_INDEX_BOX__
#define __MPEG_ISOBASE_SEGMENT_INDEX_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SegmentIndexBox: FullBox {

    static constexpr Type box_type = 'sidx';
    static constexpr const char *box_name = "SegmentIndexBox";

    union { Footprint<2 * 32> header0;
        UInt32< 0> reference_ID;
        UInt32<32> timescale;
    };

    template <FullBox::Version version, int w = (version == 0 ? 32 : 64)>
    struct Header {
        union { Footprint<2 * 32> header;
            UInt<w, 0> earliest_presentation_time;
            UInt<w, w> first_offset;
        };
        Header() {}

        template <typename Composer>
        void Header_(Composer &composer) {
            composer.get(header);
        }

        void output(bitstream::output::meta::field::Stream &stream_) const;
    };
    Header<0> v0;
    Header<1> v1;

    uint64_t earliest_presentation_time() const { return version == 0 ? uint64_t(v0.earliest_presentation_time):    uint64_t(v1.earliest_presentation_time);    }
    uint64_t first_offset() const               { return version == 0 ? uint64_t(v0.first_offset):                  uint64_t(v1.first_offset);                  }

    union { Footprint<2* 16> header1;
        UInt16< 0> reserved;            // = 0
        UInt16<16> reference_count;
    };

    struct Entry;
    struct Parsed {
        using Entries = Box::Parsed::Entries<SegmentIndexBox, Entry>;
    };
    struct Composed {
        using Entries = Box::Composed::Entries<SegmentIndexBox, Entry>;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void SegmentIndexBox_(Composer &composer) {
        composer.get(header0);
        if (version == 0) {
            v0.Header_(composer);
        } else {
            v1.Header_(composer);
        }
        composer.get(header1);
        const_field<verify>(reserved, 0);
    }

    SegmentIndexBox(Composer &composer, Flags flags, Version version, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        SegmentIndexBox_(composer);
    }

    SegmentIndexBox(Parser &parser): FullBox(parser, Flags(0)) {
        SegmentIndexBox_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};

struct SegmentIndexBox::Entry : Box::Entry {

    union { Footprint<1+31+32+1+3+28> header;
        UInt<1,   0> reference_type;
        UInt<31,  1> referenced_size;
        UInt32<  32> subsegment_duration;
        UInt<1,  64> starts_with_SAP;
        UInt<3,  65> SAP_type;
        UInt<28, 68> SAP_delta_time;
    };

    Entry(const Entry &entry): header(entry.header), Box::Entry(entry) {}

    template <typename Composer>
    Entry(const SegmentIndexBox &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SEGMENT_INDEX_BOX__

