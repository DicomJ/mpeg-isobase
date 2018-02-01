#ifndef __MPEG_ISOBASE_ADDITIONAL_METADATA_CONTAINER_BOX__
#define __MPEG_ISOBASE_ADDITIONAL_METADATA_CONTAINER_BOX__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct AdditionalMetadataContainerBox: Box {

    static constexpr Type container_type[] = {'moov', 'trak'};
    static constexpr Type box_type = 'meco';
    static constexpr const char *box_name = "AdditionalMetadataContainerBox";

    AdditionalMetadataContainerBox(Composer &composer, bool largesize = false): Box(composer, box_type, largesize) {}
    AdditionalMetadataContainerBox(Parser &parser): Box(parser) {}
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_ADDITIONAL_METADATA_CONTAINER_BOX__

