#include <bitstream/omftag.h>
#include <mpeg-isobase/box/sample_dependency_type.h>


namespace mpeg {
namespace isobase {


void SampleDependencyTypeBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.entries(); }};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void SampleDependencyTypeBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
}


void SampleDependencyTypeBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(is_leading) << "is_leading";
    stream.tag(sample_depends_on) << "sample_depends_on";
    stream.tag(sample_is_depended_on) << "sample_is_depended_on";
    stream.tag(sample_has_redundancy) << "sample_has_redundancy";
}


}} // namespace mpeg::isobase

