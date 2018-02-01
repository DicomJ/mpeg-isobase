#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_run.h>


namespace mpeg {
namespace isobase {


void TrackRunBox::parse_payload(Remainder left_payload) {
    if (version == 0) {
        V0::Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.sample_count; }};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    } else if (version == 1) {
        V1::Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.sample_count; }};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    }
}

void TrackRunBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(sample_count)  << "sample_count";
    if (flags & 0x000001) {
        stream.tag(data_offset)             << "data_offset";
    }
    if (flags & 0x000004) {
        stream.tag(first_sample_flags)      << "first_sample_flags";
    }
}


template <FullBox::Version version>
void TrackRunBox::Entry<version>::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (box.flags & 0x000100) {
        stream.tag(sample_duration) << "sample_duration";
    }
    if (box.flags & 0x000200) {
        stream.tag(sample_size) << "sample_size";
    }
    if (box.flags & 0x000400) {
        stream.tag(sample_flags) << "sample_flags";
    }
    if (box.flags & 0x000800) {
        stream.tag(sample_composition_time_offset) << "sample_composition_time_offset";
    }
}

template struct TrackRunBox::Entry<0>;
template struct TrackRunBox::Entry<1>;

}} // namespace mpeg::isobase

