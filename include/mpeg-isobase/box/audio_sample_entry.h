#ifndef __MPEG_ISOBASE_AUDIO_SAMPLE_ENTRY__
#define __MPEG_ISOBASE_AUDIO_SAMPLE_ENTRY__

#include <mpeg-isobase/box/sample_entry.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/composer.h>


namespace mpeg {
namespace isobase {


struct AudioSampleEntry: SampleEntry {

    static constexpr Type box_type = 'soun';
    static constexpr const char *box_name = "AudioSampleEntry";

    union { Footprint<2 * 32 + 4 * 16 + 32> header;
        UInt32<  0>::Static::Array<2> reserved;    // = 0;
        UInt16< 64> channelcount;   // = 2;
        UInt16< 80> samplesize;     // = 16;
        UInt16< 96> pre_defined;    // = 0;
        UInt16<112> reserved2;      // = 0;
        UInt32<128> samplerate;     // {timescale of media}<<16;
    };

    template <typename Composer, bool verify = std::is_base_of<Parser, Composer>::value>
    void AudioSampleEntry_(Composer &composer) {
        composer.get(header);
        const_field<verify>(reserved,             {0, 0});
        //const_field<verify>assert(channelcount, 2);
        const_field<verify>(samplesize,           16);
        const_field<verify>(pre_defined,          0);
        const_field<verify>(reserved2,            0);
    }

    AudioSampleEntry(Composer &composer, bool largesize = false): SampleEntry(composer, box_type, largesize) {
        AudioSampleEntry_(composer);
    }

    AudioSampleEntry(Parser &parser): SampleEntry(parser) {
        AudioSampleEntry_(parser);
    }

    virtual void output_fields(bitstream::output::meta::field::Stream &stream) const;

};


}} // namespace mpeg::isobase

#endif // __MPEG_ISOBASE_AUDIO_SAMPLE_ENTRY__

