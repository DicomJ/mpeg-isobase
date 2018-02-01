#ifndef __MPEG_ISOBASE_BOX__
#define __MPEG_ISOBASE_BOX__

#include <string>
#include <bitstream/field.h>
#include <bitstream/string.h>
#include <bitstream/header.h>
#include <bitstream/omheader.h>
#include <bitstream/remainder.h>


namespace bitstream {
    struct Composer;
}

namespace mpeg {
namespace isobase {

using namespace bitstream;
using namespace bitstream::be;
using bitstream::be::Field;


struct Box: bitstream::Header, bitstream::output::meta::Header {

    struct Composer;
    struct Parser;

    using Type = uint32_t;
    struct Extended {
        using Type = char [16 + sizeof('\0')];  // '\0' makes it a "C" string (zero-ended) complient
        using CStrType = char const *;
    };

    union { Footprint<64> header;
        UInt32< 0> smallsize;
        UInt32<32> htype;
    };
    UInt64<64> largesize;
    Static::String<16> husertype;

private:
    struct {
        uint64_t size;
        Type type;
        std::string usertype;
    } _optimized;

    struct Header {
        template <typename Composer_>
        Header(Box &box, Composer_ &composer);
    } _header;

    struct Size {
        inline Size(Box &box, Parser &parser);
        inline Size(Box &box, Composer &composer, bool islarge);
        inline Size(Box &box, Composer &composer, uint32_t size);
        inline Size(Box &box, Composer &composer, uint64_t size);
        bool islarge;
    } _size;


public:
    bool is_large() const { return _size.islarge; }
    uint64_t size() const { return _optimized.size; }

    void size(uint64_t size) {
        assert(size >= decltype(Box::header)::bytes_occupied);
        assert(_size.islarge == bool(size > uint32_t(-1L)));
        if (_size.islarge) {
            largesize = (_optimized.size = size);
        } else {
            smallsize = (_optimized.size = size);
        }
    }

    Type type() const { return _optimized.type; }
    bool is_usertype() const { return (_optimized.type == 'uuid'); }
    std::string usertype() const { return _optimized.usertype; }

private:
    struct _Type {
        inline _Type(Box &box, Parser &parser);
        inline _Type(Box &box, Composer &composer, Box::Type type);
        inline _Type(Box &box, Composer &composer, const std::string &usertype);
    } _type;

public:     // String types
    using String = bitstream::CString<>; // To the first '\0' symbol and strips the rest
    struct Static {
        template <long offset, long items>
        using String = bitstream::Static::CString<items, offset>; // To the first '\0' symbol and strips the rest
    };

protected:  // Entries base types
    struct Entry;
    template <typename Box, typename Entry, typename Composer>
    struct Entries;
    struct Parsed {
        template <typename Box, typename Entry>
        struct Entries;
    };
    struct Composed {
        template <typename Box, typename Entry>
        struct Entries;
    };

public:    // For top-level boxes container is a file it self
    static constexpr Type top_level_box = 0;

public:
    struct Desc;
    struct Name;
    struct UnifiedType;
    struct Constructor;
    struct Map;
protected:
    struct Deleter;
    struct Parsing {
        struct Control;
        struct Context;
    };

public:
    static const Desc &undeclared;

protected:
    bitstream::Composer &bitstream_composer;
    inline Box(bitstream::Composer &bitstream_composer);

    Parser &parser() { return reinterpret_cast<Box::Parser &>(bitstream_composer); }    // TODO: make it dynamic_cast for debug version
    const Parser &parser() const { return reinterpret_cast<const Box::Parser &>(bitstream_composer); }    // TODO: make it dynamic_cast for debug version
    Composer &composer() { return reinterpret_cast<Box::Composer &>(bitstream_composer); }    // TODO: make it dynamic_cast for debug version

protected:  // Parsing
    Box(Box::Parser &parser);
    virtual void parse_payload(Remainder left_payload);
    Parsing::Context *parsing_ctx;

public:
    inline const Desc &desc() const;
    inline Parsing::Control &parsing_control() const;

protected:  // Composing
    template <typename BoxType, typename BoxSize=bool>
    Box(Box::Composer &composer, const BoxType &type, const BoxSize &size = false);

protected:
    // Implement bitstream::output::meta::Header interface
    virtual void output_header(bitstream::output::meta::header::Stream &) const;
    virtual void output_fields(bitstream::output::meta::field::Stream &) const;
    virtual void output_payload(bitstream::output::meta::payload::Stream &, const bitstream::Blob &, bitstream::Parser &) const;
};


struct Box::Parsing::Control {
    // Default values which are set for each parsed box
    // Observer may set the following flags during handling header event
    bool skip_payload_parsing = false;
    bool ignore_payload_parsing_error = true;   // TODO: make true by default only in debug mode
};


struct Box::Parsing::Context : Box::Parsing::Control {
    Parser &parser;
    const Box::Desc &desc;
};


inline const Box::Desc &Box::desc() const { return parsing_ctx->desc; }
inline Box::Parsing::Control &Box::parsing_control() const { return const_cast<Box::Parsing::Context &>(*parsing_ctx); }

}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_BOX__

