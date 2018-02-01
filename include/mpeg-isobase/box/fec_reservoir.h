#ifndef __MPEG_ISOBASE_FEC_RESERVOIR_BOX__
#define __MPEG_ISOBASE_FEC_RESERVOIR_BOX__

#include <mpeg-isobase/box/file_reservoir.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct FECReservoirBox: FileReservoirBox {

    static constexpr Type box_type = 'fecr';
    static constexpr const char *box_name = "FECReservoirBox";

    using FileReservoirBox::FileReservoirBox;
};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_FEC_RESERVOIR_BOX__

