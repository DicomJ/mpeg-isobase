#include <bitstream/omftag.h>
#include <mpeg-isobase/box/audio_sample_entry.h>


namespace mpeg {
namespace isobase {


void AudioSampleEntry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    SampleEntry::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(reserved)  << "reserved";
    stream.tag(channelcount)  << "channelcount";
    stream.tag(samplesize)  << "samplesize";
    stream.tag(pre_defined)  << "pre_defined";
    stream.tag(reserved2)  << "reserverd";
    stream.tag(samplerate)  << "samplerate" << Format("fixed-point");
}


}} // namespace mpeg::isobase

