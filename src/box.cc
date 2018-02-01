#include <cctype>
#include <mpeg-isobase/box.h>
#include <mpeg-isobase/box_desc.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>
#include <bitstream/omftag.h>
#include <bitstream/sstream.h>
#include <bitstream/blob.h>


namespace mpeg {
namespace isobase {


const Box::Desc& Box::undeclared = Box::Desc();


void Box::output_header(bitstream::output::meta::header::Stream &stream) const {
    bitstream::output::meta::Stream::Tag tag = {
        .name = Box::Name(*this, parsing_ctx->desc),
        .size = 8 * parser().box_header_size()};
    stream.header(tag, parser().hstream.data);
}

void Box::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    if (is_large()) {
        stream.typed_tag<decltype(largesize)>(_optimized.size) << "largesize";
    } else {
        stream.typed_tag<decltype(smallsize)>(_optimized.size) << "size";
    }
    if (is_usertype()) {
        stream.typed_tag<decltype(husertype)>(_optimized.usertype) << "usertype";
    } else {
        stream.typed_tag<decltype(htype)>(_optimized.type) << "type" << Format("fourcc");
    }
}

void Box::output_payload(bitstream::output::meta::payload::Stream &stream, const bitstream::Blob &payload, bitstream::Parser &parser) const {
    bitstream::output::meta::Stream::Tag tag = {
        .name = "<Payload>",
        .size = 8 * payload.size()};
    stream.payload(tag, payload);
}

template <typename Composer_>
Box::Header::Header(Box &box, Composer_ &composer) {
    composer.hstream.reset(); // Reset header stream
    composer.get(box.header);
}

Box::Size::Size(Box &box, Box::Parser &parser) {
    box._optimized.size = box.smallsize;
    box._size.islarge = (box._optimized.size == 1);
    assert(box._size.islarge || box._optimized.size >= decltype(Box::header)::bytes_occupied);
    if (box._size.islarge) {
        box._optimized.size = parser.get(box.largesize);
    }
}

inline Box::Size::Size(Box &box, Box::Composer &composer, bool islarge) {
    box._size.islarge = islarge;
    if (box._size.islarge) {
        box.smallsize = (box._optimized.size = 1);
        composer.get(box.largesize);
    }
}

inline Box::Size::Size(Box &box, Box::Composer &composer, uint32_t size): Size(box, composer, false) {
    box.size(size);
}

inline Box::Size::Size(Box &box, Box::Composer &composer, uint64_t size): Size(box, composer, true) {
    box.size(size);
}

inline Box::_Type::_Type(Box &box, Box::Parser &parser) {
    box._optimized.type = box.htype;
    if (box.is_usertype()) {
        box._optimized.usertype = parser.get(box.husertype);
    }
}

inline Box::_Type::_Type(Box &box, Box::Composer &composer, Box::Type type) {
    box.htype = (box._optimized.type = type);
    assert(!box.is_usertype()); // Another constructor must have been used
}

inline Box::_Type::_Type(Box &box, Box::Composer &composer, const std::string &usertype) {
    box.htype = (box._optimized.type = 'uuid');
    composer.get(box.husertype) = (box._optimized.usertype = usertype);
}

template <typename BoxType, typename BoxSize>
Box::Box(Box::Composer &composer, const BoxType &type, const BoxSize &size)
    : bitstream_composer(composer)
    , _optimized()
    , _header(*this, composer)
    , _size(*this, composer, size)
    , _type(*this, composer, type) {}

Box::Box(Box::Parser &parser)
    : bitstream_composer(parser)
    , _optimized()
    , _header(*this, parser)
    , _size(*this, parser)
    , _type(*this, parser) {}

}} // namespace mpeg::isobase

