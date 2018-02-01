#include <bitstream/omftag.h>
#include <mpeg-isobase/box/file_reservoir.h>


namespace mpeg {
namespace isobase {


void FileReservoirBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.entry_count(); }};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

template <FullBox::Version version, int w>
void FileReservoirBox::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count) << "entry_count";
}

template struct FileReservoirBox::Header<0>;
template struct FileReservoirBox::Header<1>;


void FileReservoirBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    FullBox::output_fields(stream_);
    if (version == 0) {
        v0.output(stream_);
    } else {
        v1.output(stream_);
    }
}

template <FullBox::Version version, int w>
void FileReservoirBox::Entry::Header<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(item_ID)         << "item_ID";
    stream.tag(symbol_count)    << "symbol_count";
}

template struct FileReservoirBox::Entry::Header<0>;
template struct FileReservoirBox::Entry::Header<1>;


void FileReservoirBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    if (box.version == 0) {
        v0.output(stream_);
    } else {
        v1.output(stream_);
    }
}


}} // namespace mpeg::isobase

