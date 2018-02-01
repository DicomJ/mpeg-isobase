#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sample_size.h>


namespace mpeg {
namespace isobase {


void SampleSizeBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.sample_count; }};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void SampleSizeBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(sample_size)  << "sample_size";
    stream.tag(sample_count) << "sample_count";
}


void SampleSizeBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_size) << "entry_size";
}


}} // namespace mpeg::isobase

