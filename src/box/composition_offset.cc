#include <bitstream/omftag.h>
#include <mpeg-isobase/box/composition_offset.h>


namespace mpeg {
namespace isobase {


void CompositionOffsetBox::parse_payload(Remainder left_payload) {
    if (version == 0) {
        V0::Parsed::Entries entries{*this, parser(), left_payload};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    } else if (version == 1) {
        V1::Parsed::Entries entries{*this, parser(), left_payload};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    }
}

void CompositionOffsetBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}


template <FullBox::Version version>
void CompositionOffsetBox::Entry<version>::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(sample_count) << "sample_count";
    stream.tag(sample_offset) << "sample_offset";
}

template struct CompositionOffsetBox::Entry<0>;
template struct CompositionOffsetBox::Entry<1>;

}} // namespace mpeg::isobase

