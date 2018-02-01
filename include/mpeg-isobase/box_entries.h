#ifndef __MPEG_ISOBASE_BOX_ENTRIES__
#define __MPEG_ISOBASE_BOX_ENTRIES__

#include <bitstream/dcast.h>
#include <bitstream/sstream.h>
#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>


namespace mpeg {
namespace isobase {


struct Box::Entry: bitstream::Header, bitstream::output::meta::Header {

    const bitstream::Composer *composer; // Requires casting
    unsigned long index;
    uint32_t count;

    Entry() = default;
    Entry(const Entry &entry): composer(entry.composer), index(entry.index) { }

    virtual bool output_ellipses(bitstream::output::meta::header::Stream &) const;
    virtual void output_header(bitstream::output::meta::header::Stream &stream) const;

    virtual const char *name() const { return "Entry"; }
};


template <typename BoxType, typename EntryType, typename Composer_>
struct Box::Entries {
    const BoxType &box;
    Composer_ &composer;
    uint32_t count;


    Entries(const BoxType &box, Composer_ &composer, uint32_t count)
        : box(box), composer(composer), count(count) {
    }

    struct Iterator {
        Entries &entries;
        unsigned long index;

        Iterator(Entries &entries, unsigned long index) : entries(entries), index(index) {}
        bool operator != (const Iterator &it) const { return index != it.index; }

        void operator++() {
            auto entry_size = entries.composer.hstream.consumed();
            entries.composer.stream.get_blob(entry_size);   // TODO: make it more efficient by fetching blob for many objects not one
            ++index;
        }

        const EntryType operator * () const {
            return const_cast<Entries &>(*this).operator * ();
        }

        EntryType operator * () {
            entries.composer.hstream.reset();
            EntryType entry{entries.box, entries.composer};
            entry.composer = &entries.composer;
            entry.index = index;
            entry.count = entries.count;
            return entry;
        }
    };

    Iterator begin() { return Iterator{*this, 0}; }
    Iterator end() { return Iterator{*this, count}; }
    const Iterator begin() const { return Iterator{const_cast<Entries &>(*this), 0}; }
    const Iterator end() const { return Iterator{const_cast<Entries &>(*this), count}; }
};


template <typename BoxType, typename EntryType>
struct Box::Parsed::Entries: Box::Entries<BoxType, EntryType, bitstream::Parser> {

    Remainder &left;

    Entries(const BoxType &box, bitstream::Parser &parser, Remainder &left,
            uint32_t (*entries)(const BoxType &) = [](const auto &box) -> uint32_t { return box.entry_count; } )
        : Entries(box, parser, left, entries(box)) {}

    Entries(const bitstream::Parser::Event::Payload::Boundary::Begin &event,
            uint32_t (*entries)(const BoxType &) = [](const auto &box) -> uint32_t { return box.entry_count; } )
        : Entries(event, entries(dcast<const BoxType>(event.header))) {}

    Entries(const bitstream::Parser::Event::Payload::Boundary::Begin &event, uint32_t count)
        : Entries(dcast<const BoxType>(event.header), event.parser, event.remainder, count) {}

    Entries(const BoxType &box, bitstream::Parser &parser, Remainder &left, uint32_t count)
        : Box::Entries<BoxType, EntryType, bitstream::Parser>(box, parser, count), left(left) {}

    struct Iterator: Box::Entries<BoxType, EntryType, bitstream::Parser>::Iterator {
        using Box::Entries<BoxType, EntryType, bitstream::Parser>::Iterator::Iterator;

        void operator++() {
            Entries &entries = static_cast<Entries &>(this->entries);
            auto entry_size = this->entries.composer.hstream.consumed();
            entries.left.reduce(entry_size, [&]() {
                return Parser::Exception(SStream() <<
                        "size of #" << this->index << " entry (" << entry_size << " bytes) outreaches "
                        "what's left within container's box payload (" << int64_t(entries.left) << " bytes).");
            });
            Box::Entries<BoxType, EntryType, bitstream::Parser>::Iterator::operator++();
        }
    };

    Iterator begin() { return Iterator{*this, 0}; }
    Iterator end() { return Iterator{*this, this->count}; }
    const Iterator begin() const { return Iterator{const_cast<Entries &>(*this), 0}; }
    const Iterator end() const { return Iterator{const_cast<Entries &>(*this), this->count}; }
};


template <typename BoxType, typename EntryType>
struct Box::Composed::Entries: Box::Entries<BoxType, EntryType, bitstream::Composer> {

    template <typename Size>
    Entries(BoxType &box, Size entry_count)
        : Box::Entries<BoxType, EntryType, bitstream::Composer>(box, box.composer(), uint32_t(entry_count)) {
    }
};

}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_BOX_ENTRIES__

