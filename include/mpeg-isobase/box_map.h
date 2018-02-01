#ifndef __MPEG_ISOBASE_BOX_MAP__
#define __MPEG_ISOBASE_BOX_MAP__

#include <unordered_map>
#include <bitstream/sstream.h>
#include <mpeg-isobase/box.h>
#include <mpeg-isobase/box_desc.h>
#include <mpeg-isobase/parser.h>


namespace mpeg {
namespace isobase {


struct Box::Map {

    template <typename Box, typename Parser=typename Box::Parser, typename ...Args>
    void add(const Args &... args) { add_box(mpeg::isobase::Box::Desc::for_box<Box, Parser>(args...)); }

    template <typename BoxType>
    const Box::Desc &operator [] (const BoxType &box_type) const { return boxes[box_type]; }

    template <typename BoxType>
    const Box::Desc &get(const BoxType &box_type, const Box::Desc &default_desc) const { return boxes.get(box_type, default_desc); }

    void print(std::ostream &stream, bool with_top_level=true) const;

    Map() { boxes[Box::top_level_box].name = "<top level box>"; }
    virtual ~Map() {}

private:

    struct Boxes {
        template <typename Key>
        struct Index: std::unordered_map<Key, Box::Desc> {
            const Box::Desc &get(const Key &key, const Box::Desc &default_desc) const {
                auto it = this->find(key);
                return it != this->end() ? it->second: default_desc;
            }
        };
        Index<Box::Type> types;
        Index<std::string> extended_types;

        Box::Desc &operator[](Box::Type type) { return types[type]; }
        Box::Desc &operator[](const std::string &extended_type) { return extended_types[extended_type]; }

        const Box::Desc &operator[](Box::Type type) const {
            return get(type, Box::undeclared);
        }
        const Box::Desc &operator[](const std::string &extended_type) const {
            return get(extended_type, Box::undeclared);
        }
        const Box::Desc &operator[](const Box &box) const {
            return get(box, Box::undeclared);
        }

        const Box::Desc &get(Box::Type type, const Box::Desc &default_desc) const {
            return types.get(type, default_desc);
        }
        const Box::Desc &get(const std::string &extended_type, const Box::Desc &default_desc) const {
            return extended_types.get(extended_type, default_desc);
        }
        const Box::Desc &get(const Box &box, const Box::Desc &default_desc) const {
            return box.is_usertype() ?
                get(std::string(box.usertype()), default_desc):
                get(Box::Type(box.type()), default_desc);
        }
    } boxes;

    void add_box(const mpeg::isobase::Box::Desc &desc) {
        switch (desc.box_type.kind) {
            case Box::UnifiedType::NotSpecified:
                throw std::runtime_error("It's not allowed to register box without box_type");
                break;
            case Box::UnifiedType::Type:
                add_box(desc.box_type.type, desc);
                break;
            case Box::UnifiedType::ExtendedType:
                add_box(desc.box_type.extended_type, desc);
                break;
            case Box::UnifiedType::Types:
                if (desc.box_type.types.size() == 0) {
                    throw std::runtime_error("It's not allowed to register box with empty list of box_type");
                }
                for (const Type box_type: desc.box_type.types) {
                    add_box(box_type, desc);
                }
                break;
            case Box::UnifiedType::ExtendedTypes:
                if (desc.box_type.extended_types.size() == 0) {
                    throw std::runtime_error("It's not allowed to register box with empty list of box_type");
                }
                for (const std::string &box_type: desc.box_type.extended_types) {
                    add_box(box_type, desc);
                }
                break;
        }
    }

    template<typename BoxType>
    void add_box(const BoxType &box_type, const mpeg::isobase::Box::Desc &desc) {

        switch (desc.container_type.kind) {
            case Box::UnifiedType::NotSpecified:
                add_box(box_type, Box::top_level_box, desc);
                break;
            case Box::UnifiedType::Type:
                add_box(box_type, desc.container_type.type, desc);
                break;
            case Box::UnifiedType::ExtendedType:
                add_box(box_type, desc.container_type.extended_type, desc);
                break;
            case Box::UnifiedType::Types:
                if (desc.container_type.types.size() == 0) {
                    throw std::runtime_error("It's not allowed to register box with empty list of container_type");
                }
                for (const Type container_type: desc.container_type.types) {
                    add_box(box_type, container_type, desc);
                }
                break;
            case Box::UnifiedType::ExtendedTypes:
                if (desc.container_type.extended_types.size() == 0) {
                    throw std::runtime_error("It's not allowed to register box with empty list of container_type");
                }
                for (const std::string &container_type: desc.container_type.extended_types) {
                    add_box(box_type, container_type, desc);
                }
                break;
        }
    }

    template<typename BoxType, typename ContainerType>
    void add_box(const BoxType &box_type, const ContainerType &container_type, const mpeg::isobase::Box::Desc &desc) {
        Desc &box = boxes[box_type];
        Desc &top_level = boxes[Box::top_level_box];
        Desc &container = boxes[container_type];
        assert(&box != &top_level);

        if (box.declared) {
            if (box != desc) {
                throw std::runtime_error(SStream() <<
                        "An attempt to overwrite box has been detected: "
                        "existing " << box.to_string() << ", " << "new: " << desc.to_string());
            }
        } else {
            box = desc;
            box.declared = true;
        }

        // box -> container
        box.containers.insert(container_type);
        container.children.insert(box_type);

        if (&container != &top_level) {
            // top_level -> box(former container)
            top_level.children.erase(box_type);
            box.containers.erase(Box::top_level_box);

            // top_level -> container
            if (container.is_abandoned()) {
                top_level.children.insert(container_type);
            }
        }
    }

};


}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_BOX_MAP__

