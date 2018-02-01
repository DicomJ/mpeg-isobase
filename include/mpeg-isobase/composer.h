#ifndef __MPEG_ISOBASE_COMPOSER__
#define __MPEG_ISOBASE_COMPOSER__

#include <bitstream/composer.h>
#include <mpeg-isobase/box.h>


namespace mpeg {
namespace isobase {

struct Box::Composer: bitstream::Composer {
};

using Composer = Box::Composer;

}} // namespace mpeg::isobase


#endif // __MPEG_ISOBASE_COMPOSER__

