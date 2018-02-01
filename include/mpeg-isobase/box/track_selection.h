#ifndef __MPEG_ISOBASE_TRACK_SELECTION_BOX__
#define __MPEG_ISOBASE_TRACK_SELECTION_BOX__

#include <mpeg-isobase/box/full.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct TrackSelectionBox: FullBox {

    static constexpr Type container_type = 'udta';
    static constexpr Type box_type = 'tsel';
    static constexpr const char *box_name = "TrackSelectionBox";

    UInt32<>        switch_group;   // = 0;
    UInt32<>::Array attribute_list; // to the end of box

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void TrackSelectionBox_(Composer &composer) {
        composer.get(switch_group);
        const_field<verify>(switch_group, 0);
    }

    template <typename AttributeList>
    TrackSelectionBox(Composer &composer, const AttributeList attribute_list, Version version = 0, bool largesize = false): FullBox(composer, box_type, version, Flags(0), largesize) {
        TrackSelectionBox_(composer);
        composer.get(this->attribute_list, attribute_list);
    }

    TrackSelectionBox(Parser &parser): FullBox(parser, Flags(0)) {
        TrackSelectionBox_(parser);
        parser.get(*this, attribute_list);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_TRACK_SELECTION_BOX__

