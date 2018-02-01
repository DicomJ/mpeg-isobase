#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_fragment_random_access.h>


namespace mpeg {
namespace isobase {


void TrackFragmentRandomAccessBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(track_ID)  << "track_ID";
    stream.tag(reserved)  << "reserved";
    stream.tag(length_size_of_traf_num)  << "length_size_of_traf_num";
    stream.tag(length_size_of_trun_num)  << "length_size_of_trun_num";
    stream.tag(length_size_of_sample_num)  << "length_size_of_sample_num";
    stream.tag(number_of_entry)  << "number_of_entry";
}


}} // namespace mpeg::isobase

