#include <bitstream/omftag.h>
#include <mpeg-isobase/box/compact_sample_size.h>


namespace mpeg {
namespace isobase {


void CompactSampleSizeBox::parse_payload(Remainder left_payload) {
    switch (uint32_t(field_size)) {
    case 4: {
        V4::Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t {
            const uint32_t sample_count = box.sample_count;
            return sample_count / 2 + sample_count % 2;
        }};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    } break;
    case 8: {
        V8::Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.sample_count; }};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    } break;
    case 16: {
        V16::Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.sample_count; }};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    } break;
    default:
        assert(false);
        break;
    }
}

void CompactSampleSizeBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(reserved)  << "reserved";
    stream.tag(field_size)  << "field_size";
    stream.tag(sample_count) << "sample_count";
}


void CompactSampleSizeBox::V4::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_size_44) << "entry_size";
}

void CompactSampleSizeBox::V8::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_size) << "entry_size";
}

void CompactSampleSizeBox::V16::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_size) << "entry_size";
}


}} // namespace mpeg::isobase

