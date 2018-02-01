#include <bitstream/omftag.h>
#include <mpeg-isobase/box/progressive_download_info.h>


namespace mpeg {
namespace isobase {


void ProgressiveDownloadInfoBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.entries(); }};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}


void ProgressiveDownloadInfoBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
}


void ProgressiveDownloadInfoBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(rate) << "rate";
    stream.tag(initial_delay) << "initial_delay";
}

}} // namespace mpeg::isobase

