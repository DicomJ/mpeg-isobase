#include <bitstream/omftag.h>
#include <mpeg-isobase/box/time_to_sample.h>


namespace mpeg {
namespace isobase {


void TimeToSampleBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void TimeToSampleBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}


void TimeToSampleBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(sample_count) << "sample_count";
    stream.tag(sample_delta) << "sample_delta";
}


}} // namespace mpeg::isobase

