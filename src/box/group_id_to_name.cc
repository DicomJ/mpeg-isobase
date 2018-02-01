#include <bitstream/omftag.h>
#include <mpeg-isobase/box/group_id_to_name.h>


namespace mpeg {
namespace isobase {


void GroupIdToNameBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void GroupIdToNameBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}


void GroupIdToNameBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(group_ID) << "group_ID";
    stream.tag(group_name) << "group_name";
}


}} // namespace mpeg::isobase

