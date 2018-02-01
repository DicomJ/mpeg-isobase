#include <ostream>
#include <functional>
#include <mpeg-isobase/box_map.h>
#include <bitstream/sstream.h>


namespace mpeg {
namespace isobase {


void Box::Map::print(std::ostream &stream, bool with_top_level) const {
    std::string indent;

    std::function<void (const Box::Desc &desc, bool)> print_box =
        [&] (const Box::Desc &desc, bool traverse=true) {

        stream << indent << "| ";

        stream << desc.to_string();
        //stream << (desc.name.empty() ? "<empty!!!>":  desc.name);
        //stream << " [";
        //for (const auto &type : desc.containers.types) {
        //    stream << (*this)[type].name << ", ";
        //}
        //for (const auto &extended_type : desc.containers.extended_types) {
        //    stream << (*this)[extended_type].name << ", ";
        //}
        //stream << "]";
        stream << std::endl;

        struct Indent {
            std::string &indent;
            Indent(std::string &indent): indent(indent) {
                indent += "  ";
            }
            ~Indent() {
                indent.erase(indent.end() - 2, indent.end());
            }
        } _(indent);

        if (traverse) {
            for (const auto &type : desc.children.types) {
                auto &child = (*this)[type];
                print_box(child, &desc != &child);
            }
            for (const auto &extended_type : desc.children.extended_types) {
                auto &child = (*this)[extended_type];
                print_box(child, &desc != &child);
            }
        }
    };

    if (with_top_level) {
        print_box(boxes[Box::top_level_box], true);
    } else {
        auto &top_level = boxes[Box::top_level_box];
        for (const auto &type : top_level.children.types) {
            print_box((*this)[type], true);
        }
        for (const auto &extended_type : top_level.children.extended_types) {
            print_box((*this)[extended_type], true);
        }

    }
}


}} // namespace mpeg::isobase

