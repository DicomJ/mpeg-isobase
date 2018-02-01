#ifndef __MPEG_ISOBASE_EXTENDED_LANGUAGE_BOX__
#define __MPEG_ISOBASE_EXTENDED_LANGUAGE_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct ExtendedLanguageBox: FullBox {

    static constexpr Type container_type = 'mdia';
    static constexpr Type box_type = 'elng';
    static constexpr const char *box_name = "ExtendedLanguageBox";

    String extended_language;

    ExtendedLanguageBox(Composer &composer, const std::string &extended_language, Version version = 0, Flags flags = 0, bool largesize = false): FullBox(composer, box_type, version, flags, largesize) {
        composer.get(this->extended_language, extended_language);
    }

    ExtendedLanguageBox(Parser &parser): FullBox(parser) {
        parser.get(*this, extended_language);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_EXTENDED_LANGUAGE_BOX__

