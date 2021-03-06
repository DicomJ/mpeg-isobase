#include <bitstream/omftag.h>
#include <mpeg-isobase/box/shadow_sync_sample.h>


namespace mpeg {
namespace isobase {


void ShadowSyncSampleBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void ShadowSyncSampleBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}


void ShadowSyncSampleBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(shadowed_sample_number) << "shadowed_sample_number";
    stream.tag(sync_sample_number) << "sync_sample_number";
}


}} // namespace mpeg::isobase

