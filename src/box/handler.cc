#include <bitstream/omftag.h>
#include <mpeg-isobase/box/handler.h>


namespace mpeg {
namespace isobase {



void HandlerBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(pre_defined)     << "pre_defined";
    stream.tag(handler_type)    << "handler_type" << Format("fourcc");
    stream.tag(reserved)        << "reserved";
    stream.tag(name)            << "name";
}


}} // namespace mpeg::isobase

