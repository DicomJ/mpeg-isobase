#ifndef __MPEG_ISOBASE_SUBSEGMENT_INDEX_BOX__
#define __MPEG_ISOBASE_SUBSEGMENT_INDEX_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/box_entries.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct SubsegmentIndexBox: FullBox {

    static constexpr Type box_type = 'ssix';
    static constexpr const char *box_name = "SubsegmentIndexBox";

    UInt32<0> subsegment_count;

    struct Subsegment;
    struct Parsed {
        using Subsegments = Box::Parsed::Entries<SubsegmentIndexBox, Subsegment>;
    };
    struct Composed {
        using Subsegments = Box::Composed::Entries<SubsegmentIndexBox, Subsegment>;
    };

    SubsegmentIndexBox(Composer &composer, uint32_t subsegment_count, bool largesize = false): FullBox(composer, box_type, Version(0), Flags(0), largesize) {
        composer.get(this->subsegment_count) = subsegment_count;
    }

    SubsegmentIndexBox(Parser &parser): FullBox(parser, Version(0), Flags(0)) {
        parser.get(subsegment_count);
        //assert(subsegment_count > 0);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

    virtual void parse_payload(Remainder left_payload);
};


struct SubsegmentIndexBox::Subsegment : Box::Entry {
    UInt32<> range_count;

    virtual const char *name() const { return "Subsegment"; }

    Subsegment(const Subsegment &) = default;

    template <typename Composer>
    Subsegment(const SubsegmentIndexBox &, Composer &composer) {
        composer.get(range_count);
    }

    struct Range;
    struct Parsed {
        using Ranges = Box::Parsed::Entries<Subsegment, Range>;
    };
    struct Composed {
        using Ranges = Box::Composed::Entries<Subsegment, Range>;
    };

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


struct SubsegmentIndexBox::Subsegment::Range : Box::Entry {
    union { Footprint<8+24> header;
        UInt<8,  0> level;
        UInt<24, 8> range_size;
    };

    virtual const char *name() const { return "Range"; }

    Range(const Range &range): header(range.header), Box::Entry(range) {}

    template <typename Composer>
    Range(const Subsegment &, Composer &composer) {
        composer.get(header);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_SUBSEGMENT_INDEX_BOX__

