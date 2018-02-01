#include <bitstream/omftag.h>
#include <mpeg-isobase/box/producer_reference_time.h>


namespace mpeg {
namespace isobase {


void ProducerReferenceTimeBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(reference_track_ID)  << "reference_track_ID";
    stream.tag(ntp_timestamp)       << "ntp_timestamp";
    if (version == 0) {
        v0.output(stream_);
    } else {
        v1.output(stream_);
    }
}


template <FullBox::Version version, int w>
void ProducerReferenceTimeBox::MediaTime<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(media_time) << "media_time";
}

template struct ProducerReferenceTimeBox::MediaTime<0>;
template struct ProducerReferenceTimeBox::MediaTime<1>;

}} // namespace mpeg::isobase

