#include <bitstream/omftag.h>
#include <mpeg-isobase/box/file_partition.h>


namespace mpeg {
namespace isobase {


void FilePartitionBox::parse_payload(Remainder left_payload) {
    Parsed::Entries entries{*this, parser(), left_payload, [](const auto &box) -> uint32_t { return box.entry_count(); }};
    for(auto entry : entries) {
        Parser::Event::Header{parser(), entry};
    }
}

void FilePartitionBox::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    FullBox::output_fields(stream_);
    if (version == 0) {
        iiv0.output(stream_);
    } else {
        iiv1.output(stream_);
    }

    stream.tag(packet_payload_size)             << "packet_payload_size";
    stream.tag(reserved)                        << "reserved";
    stream.tag(FEC_encoding_ID)                 << "FEC_encoding_ID";
    stream.tag(FEC_instance_ID)                 << "FEC_instance_ID";
    stream.tag(max_source_block_length)         << "max_source_block_length";
    stream.tag(encoding_symbol_length)          << "encoding_symbol_length";
    stream.tag(max_number_of_encoding_symbols)  << "max_number_of_encoding_symbols";

    if (version == 0) {
        v0.output(stream_);
    } else {
        v1.output(stream_);
    }
}

void FilePartitionBox::Entry::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(block_count) << "block_count";
    stream.tag(block_size) << "block_size";
}


template <FullBox::Version version, int w>
void FilePartitionBox::ItemIDField<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(item_ID) << "item_ID";
}

template struct FilePartitionBox::ItemIDField<0>;
template struct FilePartitionBox::ItemIDField<1>;


template <FullBox::Version version, int w>
void FilePartitionBox::EntryCountField<version, w>::output(bitstream::output::meta::field::Stream &stream_) const {
    using namespace bitstream::output::meta::field::tag;
    using bitstream::output::meta::field::tag::Stream;
    const auto &stream = Stream(stream_);
    stream.tag(entry_count) << "entry_count";
}

template struct FilePartitionBox::EntryCountField<0>;
template struct FilePartitionBox::EntryCountField<1>;


}} // namespace mpeg::isobase

