#include <algorithm>
#include <mpeg-isobase/box_desc.h>
#include <bitstream/sstream.h>

namespace bitstream {
namespace output {
namespace print {

extern void fourcc(std::ostream &stream, const uint32_t value);

}}} // namespace bitstream::output::print


namespace mpeg {
namespace isobase {


struct Print {
    template <typename Items, typename Lambda>
    static void array(std::ostream &ss, const Items &items, Lambda print) {
        ss << "[";
        for (auto i = 0;;) {
            //print(items[i]);
            if (++i != items.size()) {
                ss << ", ";
            } else {
                break;
            }
        }
        ss << "]";
    }

    static void box_type(std::ostream &ss, Box::Type type) {
        if (type == Box::top_level_box) {
            ss << "<top_level_box>";
        } else if (type == -1) {
            ss << "-1";
        } else {
            output::print::fourcc(ss, type);
        }
    }
};

std::string Box::UnifiedType::to_string() const {
    std::ostringstream ss;

    switch (kind) {
    case Type:
        Print::box_type(ss, type);
        break;
    case ExtendedType:
        ss << extended_type;
        break;
    case Types:
        Print::array(ss, types, [&](const auto &type) { Print::box_type(ss, type); });
        break;
    case ExtendedTypes:
        Print::array(ss, extended_types, [&](const auto &extended_type) { ss << extended_type; });
        break;
    case NotSpecified:
        ss << "<not defined>";
        break;
    }

    return ss.str();
}


bool Box::UnifiedType::has(Box::Type type) const {
    return  kind == Type ?
                this->type == type :
            kind == Types ?
                std::find(types.begin(), types.end(), type) != types.end() :
            false;
}

bool Box::UnifiedType::has(const std::string &extended_type) const {
    return  kind == ExtendedType ?
                this->extended_type == extended_type :
            kind == ExtendedTypes ?
                std::find(extended_types.begin(), extended_types.end(), extended_type) != extended_types.end() :
            false;
}

std::string Box::Desc::to_string() const {
    std::ostringstream ss;

    ss << "Desc(name: ";
    if (name.empty()) { ss << "<empty!!!>"; } else { ss << name; }
    ss << ", box_type: " << box_type.to_string();
    ss << ", container_type: " << container_type.to_string();
    ss << ", constructor: " << (bool(constructor) ? "not null": "nullptr");
    ss <<")";

    return ss.str();
}

static std::string from(Box::Type box_type) {
    std::ostringstream ss;
    ss << "Box('";
    Print::box_type(ss, box_type);
    ss << "')";
    return ss.str();
}

static std::string from(const std::string &box_type) {
    return "Box('" + box_type + "')";
}

Box::Name::Name(const Box &box):
    std::string(box.is_usertype() ?
                    from(std::string(box.usertype(), 16)) :
                    from(Box::Type(box.type()))) {
}

Box::Name::Name(const Box &box, const Box::Desc &desc):
    std::string(desc.name.empty()?
                    "<undeclared>::" + Name(box):
                    static_cast<const std::string &>(desc.name)) {
}


}} // namespace mpeg::isobase

