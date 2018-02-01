#include <bitstream/omftag.h>
#include <mpeg-isobase/box/level_assignment.h>


namespace mpeg {
namespace isobase {


void LevelAssignmentBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.level_count; }};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void LevelAssignmentBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(level_count) << "level_count";
}


void LevelAssignmentBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(track_id) << "track_id";
    stream.tag(padding_flag) << "padding_flag";
    stream.tag(assignment_type) << "assignment_type";
    if (assignment_type == 0) {
        stream.tag(grouping_type) << "grouping_type";
    } else if (assignment_type == 1) {
        stream.tag(grouping_type) << "grouping_type";
        stream.tag(grouping_type_parameter) << "grouping_type_parameter";
    } else if (assignment_type == 4) {
        stream.tag(sub_track_id) << "sub_track_id";
    }
}


}} // namespace mpeg::isobase

