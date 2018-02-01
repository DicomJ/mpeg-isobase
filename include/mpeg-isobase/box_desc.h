#ifndef __MPEG_ISOBASE_BOX_DESC__
#define __MPEG_ISOBASE_BOX_DESC__

#include <unordered_set>
#include <type_traits>      // std::void_t/true_type
#include <bitstream/sstream.h>
#include <mpeg-isobase/box.h>


namespace mpeg {
namespace isobase {


struct Box::Name: std::string {
    Name(const decltype(nullptr) &) {}
    Name(const char *name) : std::string(name) {}
    Name(const std::string &name): std::string(name) {}
    Name(const Box &box);
    Name(const Box &box, const Box::Desc &desc);
};


struct Box::UnifiedType {
    enum {
        NotSpecified,
        Type,
        ExtendedType,
        Types,
        ExtendedTypes,
    } kind;

    Box::Type type;
    std::string extended_type;
    std::vector<Box::Type> types;
    std::vector<std::string> extended_types;

    bool has(Box::Type type) const;
    bool has(const std::string &extended_type) const;

    bool operator == (const UnifiedType &rvalue) const {
        return kind == rvalue.kind && (
                kind == Type ? type == rvalue.type:
                kind == ExtendedType ? extended_type == rvalue.extended_type:
                kind == Types ? types == rvalue.types:
                kind == ExtendedTypes ? extended_types == rvalue.extended_types: false);
    }

    bool operator != (const UnifiedType &rvalue) const {
        return !(*this == rvalue);
    }

    std::string to_string() const;

    template <typename Stream, typename Type>
    static void types_to_stream(Stream &stream, const Type &type) {
        stream << mpeg::isobase::Box::UnifiedType(type).to_string();
    }
    template <typename Stream, typename Type, typename ...Args>
    static void types_to_stream(Stream &stream, const Type &type, const Args &...args) {
        stream << mpeg::isobase::Box::UnifiedType(type).to_string();
        stream << ", ";
        types_to_stream(stream, args...);
    }

    template <typename ...Args>
    static std::string types_to_string(const Args &...args){
        SStream ss;
        types_to_stream(ss, args...);
        return ss;
    }

    UnifiedType(const decltype(nullptr) &): kind(NotSpecified) {}
    UnifiedType(const Box::Type &type): kind(Type), type(type) {}
    UnifiedType(const std::vector<Box::Type> &types): kind(Types), types(types) {}

    UnifiedType(const Box::Extended::Type &extended_type): kind(ExtendedType), extended_type(extended_type) {}

    UnifiedType(const Box::Extended::CStrType &extended_type): kind(ExtendedType), extended_type(extended_type) {}
    UnifiedType(const std::vector<Box::Extended::CStrType> &extended_types): kind(ExtendedTypes), extended_types(extended_types.begin(), extended_types.end()) {}

    UnifiedType(const std::string &extended_type): kind(ExtendedType), extended_type(extended_type) {}
    UnifiedType(const std::vector<std::string> &extended_types): kind(ExtendedTypes), extended_types(extended_types) {}
};


struct Box::Constructor {
    using method_t = Box * (*)(void *, Box::Parser &);
    method_t method = nullptr;
    unsigned long size = 0;

    bool operator == (const Constructor &rv) const { return method == rv.method; }
    bool operator != (const Constructor &rv) const { return !(*this == rv); }

    operator bool () const { return method != nullptr; }

    template <typename BoxClass, typename Parser_>
    static Constructor for_box() {
        struct _ { static Box *method(void *p, Box::Parser &parser) {
            return new (p) BoxClass(
                // Upcasing parser
                static_cast<Parser_ &>(parser)
                //dynamic_cast<Parser_ &>(parser) // TODO: use dynamic for debug version
            );
        }}; return Constructor{_::method, sizeof(BoxClass)};
    }
};


struct Box::Desc {
    Box::Name name;
    Box::Constructor constructor;
    Box::UnifiedType box_type, container_type;

    struct References {
        std::unordered_set<Box::Type> types;
        std::unordered_set<std::string> extended_types;

        bool empty() const { return types.empty() && extended_types.empty(); }

        void insert(Box::Type type) { types.insert(type); }
        void insert(const std::string &extended_type) { extended_types.insert(extended_type); }

        void erase(Box::Type type) { types.erase(type); }
        void erase(const std::string &extended_type) { extended_types.erase(extended_type); }
        uint64_t size() const { return types.size() + extended_types.size(); }

    } containers, children;


    template <typename Box, typename Parser>
    static Desc for_box() {
         return Desc(BoxName<Box>::name(), BoxType<Box>::type(), BoxContainerType<Box>::type(), Box::Constructor::template for_box<Box, Parser>());
    }

    template <typename Box, typename Parser, typename Name>
    static Desc for_box(const Name &name) {
        return Desc(name, BoxType<Box>::type(), BoxContainerType<Box>::type(), Box::Constructor::template for_box<Box, Parser>());
    }

    template <typename Box, typename Parser, typename Name, typename BoxType>
    static Desc for_box(const Name &name, const BoxType &box_type) {
        return Desc(name, box_type, BoxContainerType<Box>::type(), Box::Constructor::template for_box<Box, Parser>());
    }

    template <typename Box, typename Parser, typename Name, typename BoxType, typename ContainerType>
    static Desc for_box(const Name &name, const BoxType &box_type, const ContainerType &container_type) {
        return Desc(name, box_type, container_type, Box::Constructor::template for_box<Box, Parser>());
    }

    template <typename Name, typename BoxType, typename ContainerType>
    Desc(const Name &name, const BoxType &box_type, const ContainerType &container_type, const Box::Constructor &constructor)
        : name(name), box_type(box_type), container_type(container_type), constructor(constructor), declared(false) {
    }

    bool is_container_box() const { return !children.empty(); }
    bool is_abandoned() const {
        for (const auto &type: containers.types) {
            if (!box_type.has(type)) {
                return false;
            }
        }
        for (const auto &extended_type: containers.extended_types) {
            if (!box_type.has(extended_type)) {
                return false;
            }
        }
        return true;
    }

    Desc() : Desc(nullptr, Box::Type(-1), Box::Type(-1), Box::Constructor::for_box<Box, Box::Parser>()) {}

    bool operator == (const Desc &desc) const {
        return  name == desc.name &&
                constructor == desc.constructor &&
                box_type == desc.box_type &&
                container_type == desc.container_type;
    }

    bool operator != (const Desc &desc) const {
        return !(*this == desc);
    }

    std::string to_string() const;

private:
    friend class mpeg::isobase::Box::Map;
    bool declared;

private:
    // Some nasty mechanics

    template<typename Box, typename = std::void_t<>>
    struct HasName : std::false_type {};

    template<typename Box>
    struct HasName<Box, std::void_t<decltype(Box::box_name)>> : std::true_type {};

    template<typename Box, typename = std::void_t<>>
    struct HasType : std::false_type {};

    template<typename Box>
    struct HasType<Box, std::void_t<decltype(Box::box_type)>> : std::true_type {};

    template<typename Box, typename = std::void_t<>>
    struct HasContainerType : std::false_type {};

    template<typename Box>
    struct HasContainerType<Box, std::void_t<decltype(Box::container_type)>> : std::true_type {};


    template <typename Box, bool has_name = mpeg::isobase::Box::Desc::HasName<Box>::value>
    struct BoxName;

    template <typename Box>
    struct BoxName<Box, false> {
        static constexpr auto name() { return nullptr; }
    };

    template <typename Box>
    struct BoxName<Box, true> {
        static constexpr auto name() { return Box::box_name; }
    };


    template <typename Box, bool has_name = mpeg::isobase::Box::Desc::HasType<Box>::value>
    struct BoxType;

    template <typename Box>
    struct BoxType<Box, false> {
        static constexpr auto type() { return nullptr; }
    };

    template <typename Box>
    struct BoxType<Box, true> {
        static constexpr auto type() { return UnifiedType<Box, decltype(Box::box_type)>::unified(Box::box_type); }
    };


    template <typename Box, bool has_name = mpeg::isobase::Box::Desc::HasContainerType<Box>::value>
    struct BoxContainerType;

    template <typename Box>
    struct BoxContainerType<Box, false> {
        static constexpr auto type() { return nullptr; }
    };

    template <typename Box>
    struct BoxContainerType<Box, true> {
        static constexpr auto type() { return UnifiedType<Box, decltype(Box::container_type)>::unified(Box::container_type); }
    };


    template <typename _Box, typename>
    struct UnifiedType;

    template <typename _Box>
    struct UnifiedType<_Box, const Box::Type> {
        static constexpr Box::Type unified(const Box::Type type) {
            return type;
        }
    };

    template <typename _Box, long N>
    struct UnifiedType<_Box, const Box::Type[N]> {
        static std::vector<Box::Type> unified(const Box::Type *types) {
            return std::vector<Box::Type>(types, types + N);
        }
    };
    template <typename _Box>
    struct UnifiedType<_Box, const Box::Extended::Type> {
        static constexpr const Box::Extended::CStrType unified(const Box::Extended::Type &extended_type) {
            return extended_type;
        }
    };

    template <typename _Box, long N>
    struct UnifiedType<_Box, const Box::Extended::Type[N]> {
        static std::vector<Box::Extended::CStrType> unified(const Box::Extended::Type *extended_types) {
            return std::vector<Box::Extended::CStrType>(extended_types, extended_types + N);
        }
    };
};


}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_BOX_DESC__

