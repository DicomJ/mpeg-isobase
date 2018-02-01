#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/box.h>
#include <mpeg-isobase/box_desc.h>
#include <mpeg-isobase/box_map.h>
#include <bitstream/sstream.h>
#include <bitstream/omftag.h>


namespace mpeg {
namespace isobase {


using Distance = bitstream::Stream::Distance;



struct Box::Deleter {
    Box *box = nullptr;
    Deleter(Box &box): box(&box) {}
    ~Deleter() { box->~Box(); }
};

uint64_t Box::Parser::parse_box(Remainder left, const Box::Map &boxes, const Box::Desc &default_desc) {

    Box::Desc desc(boxes.get(Box(*this), default_desc)); // peak the box header to find out type of the box
    Distance size(stream);

    // 1. Parse the header
    Box &box = *desc.constructor.method(alloca(desc.constructor.size), *this);
    Box::Deleter deleter{box};
    auto parsing_ctx = make_parsing_context(desc);
    box.parsing_ctx = &parsing_ctx;

    // Determine how much is left for this particular box
    auto box_size = box.size();
    if (left < box_size) {
        throw Parser::Exception(SStream() <<
                "Size of " << desc.name << " (" << uint64_t(box_size) << " bytes) outreaches "
                "what's left within container's box payload (" << uint64_t(left) << " bytes).");
    } else {
        left = box_size;
    }

    uint64_t header_size = box_header_size();
    left.reduce(header_size, [&]() {
        return Parser::Exception(SStream() <<
                "Header size of " << desc.name  << " (" << header_size << " bytes) outreaches "
                "what's left within container's box payload (" << int64_t(left) << " bytes).");
    });
    Event::Header{*this, box};
    stream.get_blob(header_size);   //  just skip the box

    // 2. Parse the payload of this box
    // observer's logic may direct the parser's logic to do not parse the
    // payoad by setting this flag during handling header event sent a few
    // lines above
    Event::Payload::Boundary::Scope payload_boundary_scope(*this, box, left);
    if (!box.parsing_ctx->skip_payload_parsing) {
        Distance payload(stream);
        try {
            box.parse_payload(left);
        } catch (...) {
            handle_payload_parsing_exception(box);
        }
        left.reduce(payload, [&]() {
            return Parser::Exception(SStream() <<
                    "Payload size of " << desc.name  << " (" << payload << " bytes) outreaches "
                    "what's left within container's box payload (" << uint64_t(left) << " bytes).");
        });
    }

    // 2.1. Skip the rest of payload as a blob
    if (left) {
        Blob &data = stream.peak_blob(left);
        Distance payload(stream);
        Event::Payload::Data{*this, box, data};
        left.reduce(payload, [&]() {
            return Parser::Exception(SStream() <<
                    "Parsed payload size of " << desc.name  << " (" << payload << " bytes) outreaches "
                    "what's left within container's box payload (" << uint64_t(left) << " bytes).");
        });
        stream.get_blob(left);
    }

    return size;
}

void Box::parse_payload(Remainder left) {
    // by default payload is parsed as contained boxes
    if (parsing_ctx->desc.is_container_box()) {
        // Only boxes that are known for having children could be parsed
        // otherwise parsing of the box not being a container box would
        // definetly yield parser errors
        parser().parse_boxes(left);
    }
}

unsigned long Box::Parser::parse_boxes(Remainder left, const Box::Map &boxes, const Box::Desc &default_desc) {
    // Recursively iterate over all boxes
    unsigned long count;
    for (count = 0; left > 0; ++count) {
        uint64_t size = parse_box(left, boxes, default_desc);
        left.reduce(size, [&]() {
            return Parser::Exception(SStream() <<
                    "!!! NOTE this is a bug in the parser:"
                    "The size of parsed subbox (" << uint64_t(size) << " bytes) outreaches "
                    "what's left within container's box payload (" << uint64_t(left) << " bytes).");
        });
    }
    return count;
}

void Box::Parser::handle_payload_parsing_exception(const Box &box) {

    try {
        throw;
    } catch (const bitstream::Stream::EndOfStream &) {
        throw; // Just rethrow EndOfStream. It's a "good" exception:)
    } catch (...) {
        if (box.parsing_ctx->ignore_payload_parsing_error) {
            Event::Exception{*this};
        } else {
            throw;
        }
    }
}

void Box::Parser::payload_misalignment(long limit, long left) {
    std::string message = SStream() <<
        "Detected box payload misalignment: left " << left << " bytes out"
        " of " << limit << " bytes of box' payload";
    throw Parser::Exception(message);
}

void Box::Parser::parse(Remainder left, bool raise_eos) {
    try {
        parse_boxes(left);
    } catch(const bitstream::Stream::EndOfStream &eof) {
        if (raise_eos) {
            throw;
        }
    }
}

}} // namespace mpeg::isobase

