#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sample_to_chunk.h>


namespace mpeg {
namespace isobase {


void SampleToChunkBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void SampleToChunkBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}


void SampleToChunkBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(first_chunk)                 << "first_chunk";
    stream.tag(samples_per_chunk)           << "samples_per_chunk";
    stream.tag(sample_description_index)    << "sample_description_index";
}


}} // namespace mpeg::isobase

