#include <bitstream/omftag.h>
#include <mpeg-isobase/box/edit_list.h>


namespace mpeg {
namespace isobase {


void EditListBox::parse_payload(Remainder left_payload) {
    if (version == 1) {
        V1::Parsed::Entries entries{*this, parser(), left_payload};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    } else { // version == 0
        V0::Parsed::Entries entries{*this, parser(), left_payload};
        for(auto entry : entries) {
            Parser::Event::Header{parser(), entry};
        }
    }
}

void EditListBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}


template <FullBox::Version version, int w>
void EditListBox::Entry<version, w>::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(segment_duration) << "segment_duration";
    stream.tag(media_time) << "media_time";
    //stream.tag(media_rate_integer) << "media_rate_integer";
    //stream.tag(media_rate_fraction) << "media_rate_fraction";
    stream.tag(media_rate) << "media_rate" << Format("fixed-point");
}


template struct EditListBox::Entry<0>;
template struct EditListBox::Entry<1>;

}} // namespace mpeg::isobase

