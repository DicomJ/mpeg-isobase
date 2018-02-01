#include <bitstream/omftag.h>
#include <mpeg-isobase/box_desc.h>
#include <mpeg-isobase/boxes.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/box/sample_description.h>
#include <mpeg-isobase/box/visual_sample_entry.h>
#include <mpeg-isobase/box/audio_sample_entry.h>
#include <mpeg-isobase/box/hint_sample_entry.h>


namespace mpeg {
namespace isobase {


struct SampleDescriptions: Boxes {
    SampleDescriptions() {
        add<VisualSampleEntry>();
        add<AudioSampleEntry>();
        add<HintSampleEntry>();
    }

} static sample_descriptions;

static const Box::Desc sample_entry = Box::Desc::for_box<SampleEntry, mpeg::isobase::Box::Parser>();


void SampleDescriptionBox::parse_payload(Remainder left) {
    const auto &default_desc = sample_descriptions.get(parser().handler_type, sample_entry);
    parser().parse_boxes(left, default_desc);
}

void SampleDescriptionBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count)  << "entry_count";
}

}} // namespace mpeg::isobase

