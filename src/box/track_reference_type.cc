#include <bitstream/omftag.h>
#include <mpeg-isobase/box/track_reference_type.h>


namespace mpeg {
namespace isobase {


template <uint32_t reference_type>
void TrackReferenceTypeBox<reference_type>::output_fields(bitstream::output::meta::field::Stream &stream_) const {
    Box::output_fields(stream_);
    bitstream::output::meta::field::tag::Stream(stream_)
    .tag(track_IDs) << "track_IDs";
}

template struct TrackReferenceTypeBox<'hint'>;
template struct TrackReferenceTypeBox<'cdsc'>;
template struct TrackReferenceTypeBox<'font'>;
template struct TrackReferenceTypeBox<'hind'>;
template struct TrackReferenceTypeBox<'vdep'>;
template struct TrackReferenceTypeBox<'vplx'>;
template struct TrackReferenceTypeBox<'subt'>;


}} // namespace mpeg::isobase

