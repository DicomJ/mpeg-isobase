#include <mpeg-isobase/boxes.h>

#include <mpeg-isobase/box/file_type.h>
#include <mpeg-isobase/box/movie_header.h>
#include <mpeg-isobase/box/track_header.h>
#include <mpeg-isobase/box/track_reference_type.h>
#include <mpeg-isobase/box/track_reference.h>
#include <mpeg-isobase/box/media_header.h>
#include <mpeg-isobase/box/handler.h>
#include <mpeg-isobase/box/video_media_header.h>
#include <mpeg-isobase/box/sound_media_header.h>
#include <mpeg-isobase/box/hint_media_header.h>
#include <mpeg-isobase/box/null_media_header.h>
#include <mpeg-isobase/box/extended_language.h>
#include <mpeg-isobase/box/data_information.h>
#include <mpeg-isobase/box/data_reference.h>
#include <mpeg-isobase/box/data_entry_url.h>
#include <mpeg-isobase/box/data_entry_urn.h>
#include <mpeg-isobase/box/time_to_sample.h>
#include <mpeg-isobase/box/composition_offset.h>
#include <mpeg-isobase/box/subtitle_media_header.h>
#include <mpeg-isobase/box/sample_description.h>
#include <mpeg-isobase/box/sample_size.h>
#include <mpeg-isobase/box/compact_sample_size.h>
#include <mpeg-isobase/box/sample_to_chunk.h>
#include <mpeg-isobase/box/chunk_offset.h>
#include <mpeg-isobase/box/chunk_large_offset.h>
#include <mpeg-isobase/box/sync_sample.h>
#include <mpeg-isobase/box/shadow_sync_sample.h>
#include <mpeg-isobase/box/degradation_priority.h>
#include <mpeg-isobase/box/padding_bits.h>
#include <mpeg-isobase/box/edit_list.h>
#include <mpeg-isobase/box/user_data.h>
#include <mpeg-isobase/box/copyright.h>
#include <mpeg-isobase/box/movie_extends_header.h>
#include <mpeg-isobase/box/track_extends.h>
#include <mpeg-isobase/box/movie_fragment_header.h>
#include <mpeg-isobase/box/track_fragment_header.h>
#include <mpeg-isobase/box/track_run.h>
#include <mpeg-isobase/box/track_fragment_random_access.h>
#include <mpeg-isobase/box/movie_fragment_random_access_offset.h>
#include <mpeg-isobase/box/sample_dependency_type.h>
#include <mpeg-isobase/box/sample_to_group.h>
#include <mpeg-isobase/box/sample_group_description.h>
#include <mpeg-isobase/box/sample_scale.h>
#include <mpeg-isobase/box/subsample_information.h>
#include <mpeg-isobase/box/progressive_download_info.h>
#include <mpeg-isobase/box/meta.h>
#include <mpeg-isobase/box/xml.h>
#include <mpeg-isobase/box/binary_xml.h>
#include <mpeg-isobase/box/item_location.h>
#include <mpeg-isobase/box/original_format.h>
#include <mpeg-isobase/box/protection_scheme_info.h>
#include <mpeg-isobase/box/primary_item.h>
#include <mpeg-isobase/box/item_protection.h>
#include <mpeg-isobase/box/item_info.h>
#include <mpeg-isobase/box/ipmpinfo.h>
#include <mpeg-isobase/box/ipmpcontrol.h>
#include <mpeg-isobase/box/scheme_type.h>
#include <mpeg-isobase/box/scheme_information.h>
#include <mpeg-isobase/box/composition_to_decode.h>
#include <mpeg-isobase/box/sample_auxiliary_information_sizes.h>
#include <mpeg-isobase/box/level_assignment.h>
#include <mpeg-isobase/box/track_fragment_base_media_decode_time.h>
#include <mpeg-isobase/box/track_selection.h>
#include <mpeg-isobase/box/fd_item_information.h>
#include <mpeg-isobase/box/file_reservoir.h>
#include <mpeg-isobase/box/file_partition.h>
#include <mpeg-isobase/box/fec_reservoir.h>
#include <mpeg-isobase/box/additional_metadata_container.h>
#include <mpeg-isobase/box/metabox_relation.h>
#include <mpeg-isobase/box/segment_type.h>
#include <mpeg-isobase/box/segment_index.h>
#include <mpeg-isobase/box/subsegment_index.h>
#include <mpeg-isobase/box/producer_reference_time.h>
#include <mpeg-isobase/box/item_data.h>
#include <mpeg-isobase/box/fd_session_group.h>
#include <mpeg-isobase/box/group_id_to_name.h>
#include <mpeg-isobase/box/item_reference.h>
#include <mpeg-isobase/box/sample_entry.h>
#include <mpeg-isobase/box/visual_sample_entry.h>
#include <mpeg-isobase/box/audio_sample_entry.h>
#include <mpeg-isobase/box/hint_sample_entry.h>
#include <mpeg-isobase/box/item_info_entry.h>
#include <mpeg-isobase/box/sub_track.h>
#include <mpeg-isobase/box/sub_track_information.h>
#include <mpeg-isobase/box/partition_entry.h>


namespace mpeg {
namespace isobase {


const Box::Map &Parser::all_boxes = RegisterBoxes();

RegisterBoxes::RegisterBoxes() {
    add<FileTypeBox>();
    add<Box>("MovieBox", 'moov');
    add<MovieHeaderBox>();
    add<Box>("MediaDataBox", 'mdat');
    add<Box>("TrackBox", 'trak', 'moov');
    add<TrackHeaderBox>();
    add<TrackReferenceBox>();
    add<TrackReferenceTypeBox<'hint'>>("TrackReferenceTypeBox<'hint'>");
    add<TrackReferenceTypeBox<'cdsc'>>("TrackReferenceTypeBox<'cdsc'>");
    add<Box>("MediaBox", 'mdia', 'trak');
    add<MediaHeaderBox>();
    add<HandlerBox>();
    add<Box>("MediaInformationBox", 'minf', 'mdia');
    add<VideoMediaHeaderBox>();
    add<SoundMediaHeaderBox>();
    add<HintMediaHeaderBox>();
    add<NullMediaHeaderBox>();
    add<DataInformationBox>();
    add<DataReferenceBox>();
    add<DataEntryUrlBox>();
    add<DataEntryUrnBox>();
    add<Box>("SampleTableBox", 'stbl', 'trak');
    add<TimeToSampleBox>();
    add<CompositionOffsetBox>();
    add<SampleDescriptionBox>();
    add<SampleToChunkBox>();
    //add<VisualSampleEntry>();
    //add<AudioSampleEntry>();
    //add<HintSampleEntry>();
    add<SampleScaleBox>();
    add<SampleSizeBox>();
    add<CompactSampleSizeBox>();
    add<ChunkOffsetBox>();
    add<ChunkLargeOffsetBox>();
    add<SyncSampleBox>();
    add<ShadowSyncSampleBox>();
    add<DegradationPriorityBox>();
    add<PaddingBitsBox>();
    add<Box>("FreeSpaceBox('free')", 'free');
    add<Box>("FreeSpaceBox('skip')", 'skip');
    add<Box>("EditBox", 'edts', 'trak');
    add<EditListBox>();
    add<UserDataBox>();
    add<Box>("MovieExtendsBox", 'mvex', 'moov');
    add<MovieExtendsHeaderBox>();
    add<TrackExtendsBox>();
    add<Box>("MovieFragmentBox", 'moof');
    add<MovieFragmentHeaderBox>();
    add<Box>("TrackFragmentBox", 'traf', 'moof');
    add<TrackFragmentHeaderBox>();
    add<TrackRunBox>();
    add<Box>("MovieFragmentRandomAccessBox", 'mfra');
    add<TrackFragmentRandomAccessBox>();
    add<MovieFragmentRandomAccessOffsetBox>();
    add<SampleDependencyTypeBox>();
    add<SampleToGroupBox>();
    add<SampleGroupDescriptionBox>();
    add<SubSampleInformationBox>();
    add<ProgressiveDownloadInfoBox>();
    add<MetaBox>();
    add<XMLBox>();
    add<BinaryXMLBox>();
    add<ItemLocationBox>();
    add<PrimaryItemBox>();
    add<ItemProtectionBox>();
    add<ProtectionSchemeInfoBox>();
    add<ItemInfoBox>();
    add<ItemInfoEntry>();
    add<OriginalFormatBox>();
    add<IPMPInfoBox>();
    add<IPMPControlBox>();
    add<SchemeInformationBox>();
    add<ExtendedLanguageBox>();
    add<SubtitleMediaHeaderBox>();
    add<CompositionToDecodeBox>();
    add<SampleAuxiliaryInformationSizesBox>();
    add<LevelAssignmentBox>();
    add<TrackFragmentBaseMediaDecodeTimeBox>();
    add<TrackSelectionBox>();
    add<SubTrack>();
    add<SubTrackInformation>();
    add<Box>("SubTrackDefinition", 'strd', 'strk');
    add<PartitionEntry>();
    add<FDItemInformationBox>();
    add<FileReservoirBox>();
    add<FilePartitionBox>();
    add<FECReservoirBox>();
    add<AdditionalMetadataContainerBox>();
    add<MetaboxRelationBox>();
    add<SegmentTypeBox>();
    add<SegmentIndexBox>();
    add<ProducerReferenceTimeBox>();
    add<GroupIdToNameBox>();
    add<ItemDataBox>();
    add<ItemReferenceBox>();
}


}} // namespace mpeg::isobase

