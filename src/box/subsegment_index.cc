#include <bitstream/omftag.h>
#include <mpeg-isobase/box/subsegment_index.h>


namespace mpeg {
namespace isobase {


void SubsegmentIndexBox::parse_payload(Remainder left_payload) {
    Parsed::Subsegments subsegments{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.subsegment_count; }};
    for(auto subsegment : subsegments) {
        Parser::Event::Header{parser(), subsegment};
        Subsegment::Parsed::Ranges ranges{subsegment, parser(), left_payload, [](const auto &subsegment) -> uint32_t { return subsegment.range_count; }};
        for(auto range : ranges) {
            Parser::Event::Header{parser(), range};
        }
    }
}

void SubsegmentIndexBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(subsegment_count)  << "subsegment_count";
}


void SubsegmentIndexBox::Subsegment::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(range_count) << "range_count";
}

void SubsegmentIndexBox::Subsegment::Range::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(level)       << "level";
    stream.tag(range_size)  << "range_size";
}


}} // namespace mpeg::isobase

