#include <bitstream/omftag.h>
#include <bitstream/sstream.h>
#include <mpeg-isobase/box_entries.h>


namespace mpeg {
namespace isobase {

bool Box::Entry::output_ellipses(bitstream::output::meta::header::Stream &stream) const {
    return stream.ellipses(index, count);
}

void Box::Entry::output_header(bitstream::output::meta::header::Stream &stream) const {
    bitstream::output::meta::Stream::Tag tag = {
        .name = SStream() << name() << " #" << index,
        .size = 8 * composer->hstream.consumed()};
    stream.header(tag, composer->hstream.data);
}

}} // namespace mpeg::isobase
