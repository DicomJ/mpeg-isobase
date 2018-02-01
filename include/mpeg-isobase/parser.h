#ifndef __MPEG_ISOBASE_PARSER__
#define __MPEG_ISOBASE_PARSER__

#include <bitstream/parser.h>
#include <mpeg-isobase/box.h>


namespace mpeg {
namespace isobase {


struct Box::Parser: bitstream::Parser {

    const Box::Map &boxes;

    Parser(bitstream::Stream &stream,
           bitstream::Parser::Observer &observer,
           const Box::Map &boxes)
        : bitstream::Parser(stream, observer) , boxes(boxes) {}

    virtual void parse(Remainder = Remainder(), bool raise_oes=false);
    uint64_t parse_box(Remainder left_within_container, const Box::Desc &default_desc = Box::undeclared) { return parse_box(left_within_container, boxes, default_desc); }
    uint64_t parse_box(Remainder left_within_container, const Box::Map &boxes, const Box::Desc &default_desc = Box::undeclared);
    inline unsigned long parse_boxes(Remainder left_within_container, const Box::Desc &default_desc = Box::undeclared) { return parse_boxes(left_within_container, boxes, default_desc); }
    unsigned long parse_boxes(Remainder left_within_container, const Box::Map &boxes, const Box::Desc &default_desc = Box::undeclared);
    inline void handle_payload_parsing_exception(const Box &box);

    uint64_t box_header_size() const { return hstream.consumed(); }
    uint64_t box_payload_size(const Box &box) const { return box.size() - box_header_size(); }


    // Parsing-wide flags
    Box::Parsing::Control default_parsing_control;
    bool ignore_box_pyload_missalignment = true;
    bool ignore_wrong_data_reference_entry_count = false;
    bool ignore_wrong_sample_description_entry_count = false;
    bool ignore_wrong_item_protection_count = false;
    bool ignore_wrong_item_info_entry_count = false;

    Box::Type handler_type = 0;

    // Observer inteface
    inline auto make_parsing_context(const Box::Desc &desc);

    template <typename Type>
    auto &get(Type &value) {
        return bitstream::Parser::get(value);
    }

    template <typename ...Types>
    void get(const Box &box, Types &...values) {
        auto limit = box_payload_size(box);
        auto left = bitstream::Parser::get(limit, values...);
        if (!ignore_box_pyload_missalignment && left != 0) {
            payload_misalignment(limit, left);
        }
    }

    void payload_misalignment(long limit, long left);
};


inline auto Box::Parser::make_parsing_context(const Box::Desc &desc) {
    return std::move(Box::Parsing::Context{default_parsing_control, *this, desc});
}


struct Parser: Box::Parser {

    static const Box::Map &all_boxes;

    Parser(bitstream::Stream &stream,
           bitstream::Parser::Observer &observer,
           const Box::Map &boxes = all_boxes)
        : Box::Parser(stream, observer, boxes) {}
};


}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_PARSER__

