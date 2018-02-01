#include <bitstream/omftag.h>
#include <mpeg-isobase/boxes.h>
#include <mpeg-isobase/box/track_reference.h>
#include <mpeg-isobase/box/track_reference_type.h>


namespace mpeg {
namespace isobase {

extern template struct TrackReferenceTypeBox<'hint'>;
extern template struct TrackReferenceTypeBox<'cdsc'>;
extern template struct TrackReferenceTypeBox<'font'>;
extern template struct TrackReferenceTypeBox<'hind'>;
extern template struct TrackReferenceTypeBox<'vdep'>;
extern template struct TrackReferenceTypeBox<'vplx'>;
extern template struct TrackReferenceTypeBox<'subt'>;


struct References: Boxes {

    References() {
        add<TrackReferenceTypeBox<'hint'>>();
        add<TrackReferenceTypeBox<'cdsc'>>();
        add<TrackReferenceTypeBox<'font'>>();
        add<TrackReferenceTypeBox<'hind'>>();
        add<TrackReferenceTypeBox<'vdep'>>();
        add<TrackReferenceTypeBox<'vplx'>>();
        add<TrackReferenceTypeBox<'subt'>>();
    }

} static references;

void TrackReferenceBox::parse_payload(Remainder left) {
    parser().parse_boxes(left, references);
}

}} // namespace mpeg::isobase

    // TODO: PROCESS PAYLOAD (handle particular box types)

