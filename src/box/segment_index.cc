#include <bitstream/omftag.h>
#include <mpeg-isobase/box/segment_index.h>


namespace mpeg {
namespace isobase {


void SegmentIndexBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.reference_count; }};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

template <FullBox::Version version, int w>
void SegmentIndexBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(earliest_presentation_time)  << "earliest_presentation_time";
    stream.tag(first_offset)                << "first_offset";
}

template struct SegmentIndexBox::Header<0>;
template struct SegmentIndexBox::Header<1>;


void SegmentIndexBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    FullBox::output_fields(stream_);
    stream.tag(reference_ID)    << "reference_ID";
    stream.tag(timescale)       << "timescale";
    if (version == 0) {
        v0.output(stream_);
    } else {
        v1.output(stream_);
    }
    stream.tag(reserved)        << "reserved";
    stream.tag(reference_count) << "reference_count";
}


void SegmentIndexBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(reference_type)      << "reference_type";
    stream.tag(referenced_size)     << "referenced_size";
    stream.tag(subsegment_duration) << "subsegment_duration";
    stream.tag(starts_with_SAP)     << "starts_with_SAP";
    stream.tag(SAP_type)            << "SAP_type";
    stream.tag(SAP_delta_time)      << "SAP_delta_time";
}


}} // namespace mpeg::isobase

