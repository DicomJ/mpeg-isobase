#ifndef __MPEG_ISOBASE_PARTITION_ENTRY__
#define __MPEG_ISOBASE_PARTITION_ENTRY__

#include <mpeg-isobase/box.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct PartitionEntry: Box {

    static constexpr Type container_type = 'fiin';
    static constexpr Type box_type = 'paen';
    static constexpr const char *box_name = "PartitionEntry";

    //FilePartitionBox blocks_and_symbols;
    //FECReservoirBox FEC_symbol_locations; //optional
    //FileReservoirBox File_symbol_locations; //optional

    //TODO: PROCESS PAYLOAD (control children)

    using Box::Box;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_PARTITION_ENTRY__

