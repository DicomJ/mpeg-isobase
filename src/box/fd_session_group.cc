#include <bitstream/omftag.h>
#include <mpeg-isobase/box/fd_session_group.h>


namespace mpeg {
namespace isobase {


void FDSessionGroupBox::parse_payload(Remainder left_payload) {
    for(auto i = 0; i < num_session_groups; ++i) {
        {
            Group group{*this, parser()};
            Parser::Event::Header{parser(), group};
            Group::Parsed::Entries entries{group, parser(), left_payload, [](const auto &group) -> uint32_t { return group.entry_count; }};
            for(auto entry : entries) {
                Parser::Event::Header{parser(), entry};
            }
        }
        {
            Channel channel{*this, parser()};
            Parser::Event::Header{parser(), channel};
            Channel::Parsed::Entries entries{channel, parser(), left_payload, [](const auto &channel) -> uint32_t { return channel.num_channels_in_session_group; }};
            for(auto entry : entries) {
                Parser::Event::Header{parser(), entry};
            }
        }
    }
}

void FDSessionGroupBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    Box::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(num_session_groups)  << "num_session_groups";
}


void FDSessionGroupBox::Group::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count) << "entry_count";
}

void FDSessionGroupBox::Group::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(group_ID) << "group_ID";
}

void FDSessionGroupBox::Channel::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(num_channels_in_session_group) << "num_channels_in_session_group";
}

void FDSessionGroupBox::Channel::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(hint_track_id) << "hint_track_id";
}


}} // namespace mpeg::isobase

