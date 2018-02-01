# **MPEG-ISOBASE** media file format bitstream processing library

## Key features

1. The most accurate translation of ISO/IEC 14496-12 specification into C++, based on unique declarative bitstream processing technics;
2. Totally use-case agnostic. Provides pure bitstream parsing/composing abilities without superfluous functionality;
3. Properly layered and hidden bitstream processing complexity allows to focus on use-case specifics at very high-level without sacrificing access to the low-level bitstream data;
4. Asynchronous, event-driven design with focus to process network streams not just files off the drive;
5. Fine-grained control over parsing process;
6. Leverages zero-copy technics and ultrasmart-pointers to process only required/requested bits of data what gives a huge performance boost when processing Big Endian format on Little Endian machines;
7. Doesn't allocate heap memory during bitstream processing;
8. Zero level of debug overhead when processing bitstream in production mode;
9. Builtin capabilities to debug processed stream in development mode;
11. Relies only on pure C++ and C++ Standard Library.

All that leads to bitstream processing with **extremely high-performance, low latency, small footprint and speed close to the transferring speed of data between volume (network or drive) and memory**;

## What it's not

It's not a full featured media stream processing solution which does profiling or repackaging. Nevertheless, it's very easy to build such solutions based on MPEG-ISOBASE library. For example:
https://github.com/DicomJ/mpeg-profiler
https://github.com/DicomJ/mpeg-packager

## Known limitations:

The code is mostly based on very generic C++ and some trivial C++11 features with a few places which incorporate C++17. Consequently it requires modern C++ compiler.
For instance:

* GCC >=7.1.0
* Clang >= 8.0

## Building

If you would like to check out and build MPEG-ISOBASE library, the procedure is as follows:

1. Consider setting up appropriate compiler with support of C++17 standard, if it's not installed as default one: (optional)

    ```bash
    export CXX=/opt/gcc-7.1/bin/g++     # for example
    ```

1. Check out MPEG-ISOBASE

    ```bash
    git clone git@github.com:DicomJ/mpeg-isobase.git mpeg-isobase
    ```

1. Build MPEG-ISOBASE

    ```bash
    mkdir mpeg-isobase/build
    cd mpeg-isobase/build
    cmake -DCMAKE_BUILD_TYPE=Release -DCXX_STD=c++17 -DCMAKE_INSTALL_PREFIX=$(pwd)/install $(pwd)/../../mpeg-isobase
    make install
    ```

## Usage

### Composing

// TBD

### Parsing just by calling the `Parser::parse` method

1. Checkout an example:

    At first the `Stream` object is created for the "sample.mp4" file. Then `isobase::Parser` object is instanciated by that `Stream` object and an `Observer` object. At the last line the `parse.parse()` method is called which traverses all the boxes off the `Stream`, parses all known ones and yields them via `Observer` object in form of events.
    The `Observer` object in the current example handles just a single 'ftyp' box.

    ```bash
    cat ../../mpeg-isobase/demo/ftyp.cc
    ```

    ```C++
    struct: bitstream::Parser::Observer {
        virtual void event(const bitstream::Parser::Event::Header &event) {
            auto box = dynamic_cast<const mpeg::isobase::FileTypeBox *>(&event.header);
            if (box != nullptr) {
                assert(box->type() == 'ftyp');
                std::cout << "Found 'ftyp' box of " << box->size() << " bytes size." << std::endl;
            }
        }
    } observer;
    bitstream::input::file::Stream stream("sample.mp4");
    mpeg::isobase::Parser parser(stream, observer);
    parser.parse();
    ```

1. Consider setting up right path to the stdlib if it's not installed in the system by default: (optional)

    ```bash
    export LD_LIBRARY_PATH=/opt/gcc-7.1/lib64   # for example
    ```

1. Build and run it:

    it's assumed that the current working directory is still `mpeg-isobase/build`

    ```bash
    g++ \
        -o ftyp \
        -std=c++17 -Wno-multichar \
        -I install/include \
        -Wl,-rpath,$(pwd)/install/lib64 install/lib64/*.so \
        ../../mpeg-isobase/demo/ftyp.cc
    time ./ftyp sample.mp4
    ```

    ```
    Found 'ftyp' box of 28 bytes size.

    real    0m0.007s
    user    0m0.003s
    sys     0m0.003s
    ```

### Event handling

`Parser::parse` method yields just a few parsing events defined by the `Observer`'s interface.

```C++
struct Parser::Observer {
    virtual void event(const Event::Header &) {}
    virtual void event(const Event::Payload::Boundary::Begin &) {}
    virtual void event(const Event::Payload::Data &) {}
    virtual void event(const Event::Payload::Boundary::End &) {}
    virtual void event(const Event::Exception &) {}
};

```

### Another example

It might look like `Parser::parse` method and `Observer` interface with a such short list of methods is not enough. So let's instantiate `Parser` object with an `Observer` instance (`bitstream::output::print::to_stdout`) which prints all the events into stdout.

1. Checkout an example:

    ```bash
    cat ../../mpeg-isobase/demo/dump.cc
    ```

    ```C++
    bitstream::input::file::Stream stream("sample.mp4");
    mpeg::isobase::Parser parser(stream, bitstream::output::print::to_stdout);
    parser.parse();
    ```

2. Build and run it:

    ```bash
    g++ \
        -o dump \
        -std=c++17 -Wno-multichar \
        -I install/include \
        -Wl,-rpath,$(pwd)/install/lib64 install/lib64/*.so \
        ../../mpeg-isobase/demo/dump.cc
    time ./dump sample.mp4
    ```

3. Check out the output

    ```
    +- FileTypeBox { // [28 bytes] @0
    |      uint32     size                : 28
    |      uint32     type                : ftyp
    |      uint32     major_brand         : mp42
    |      uint32     minor_version       : 0
    |      uint32[3]  compatible_brands   : [mp42, isom, avc1]
    |  }
    +- FreeSpaceBox('free') { // [8 bytes] @28
    |      uint32     size                : 132
    |      uint32     type                : free
    |  }
    |  |  <Payload>[124 bytes]
    +- MediaDataBox { // [8 bytes] @160
    |      uint32     size                : 379880
    |      uint32     type                : mdat
    |  }
    |  |  <Payload>[379872 bytes]
    +- MovieBox { // [8 bytes] @380040
    |      uint32     size                : 3459
    |      uint32     type                : moov
    |  }
    |  +- MovieHeaderBox { // [108 bytes] @380048
    |  |      uint32     size                : 108
    |  |      uint32     type                : mvhd
    |  |      uint8      version             : 0
    |  |      uint24     flags               : 0
    |  |      uint32     creation_time       : Sat Mar 20 21:29:11 2010
    |  |      uint32     modification_time   : Sat Mar 20 21:29:12 2010
    |  |      uint32     timescale           : 90000
    |  |      uint32     duration            : 501120
    |  |      int32      rate                : 1
    |  |      int16      volume              : 1
    |  |      uint16     reserved            : 0
    |  |      uint32[2]  reserved            : [0, 0]
    |  |      int32[9]   matrix              : [65536, 0, 0, 0, 65536, 0, 0, 0, 1073741824]
    |  |      uint32[6]  pre_defined         : [0, 0, 0, 0, 0, 0]
    |  |      uint32     next_track_ID       : 3
    |  |  }
    |  +- <undeclared>::Box('iods') { // [8 bytes] @380156
    |  |      uint32     size                : 24
    |  |      uint32     type                : iods
    |  |  }
    |  |  |  <Payload>[16 bytes]
    |  +- TrackBox { // [8 bytes] @380180
    |  |      uint32     size                : 1546
    |  |      uint32     type                : trak
    |  |  }
    |  |  +- TrackHeaderBox { // [92 bytes] @380188
    |  |  |      uint32     size                : 92
    |  |  |      uint32     type                : tkhd
    |  |  |      uint8      version             : 0
    |  |  |      uint24     flags               : 1
    |  |  |      uint32     creation_time       : Sat Mar 20 21:29:11 2010
    |  |  |      uint32     modification_time   : Sat Mar 20 21:29:12 2010
    |  |  |      uint32     track_ID            : 1
    |  |  |      uint32     reserved            : 0
    |  |  |      uint32     duration            : 498000
    |  |  |      uint32[2]  reserved            : [0, 0]
    |  |  |      int16      layer               : 0
    |  |  |      int16      alternate_group     : 0
    |  |  |      int16      volume              : 0
    |  |  |      uint16     reserved            : 0
    |  |  |      int32[9]   matrix              : [65536, 0, 0, 0, 65536, 0, 0, 0, 1073741824]
    |  |  |      uint32     width               : 560
    |  |  |      uint32     height              : 320
    |  |  |  }
    |  |  +- MediaBox { // [8 bytes] @380280
    |  |  |      uint32     size                : 1446
    |  |  |      uint32     type                : mdia
    |  |  |  }
    |  |  |  +- MediaHeaderBox { // [32 bytes] @380288
    |  |  |  |      uint32     size                : 32
    |  |  |  |      uint32     type                : mdhd
    |  |  |  |      uint8      version             : 0
    |  |  |  |      uint24     flags               : 0
    |  |  |  |      uint32     creation_time       : Sat Mar 20 21:29:11 2010
    |  |  |  |      uint32     modification_time   : Sat Mar 20 21:29:12 2010
    |  |  |  |      uint32     timescale           : 90000
    |  |  |  |      uint32     duration            : 498000
    |  |  |  |      uint1      pad                 : 0
    |  |  |  |      uint5[3]   language            : und (Undetermined)
    |  |  |  |      uint16     pre_defined         : 0
    |  |  |  |  }
    |  |  |  +- HandlerBox { // [21 bytes] @380320
    |  |  |  |      uint32     size                : 33
    |  |  |  |      uint32     type                : hdlr
    |  |  |  |      uint8      version             : 0
    |  |  |  |      uint24     flags               : 0
    |  |  |  |      uint32     pre_defined         : 0
    |  |  |  |      uint32     handler_type        : vide
    |  |  |  |      uint32[3]  reserved            : [0, 0, 0]
    |  |  |  |      string     name                : "vide?"
    |  |  |  |  }
    |  |  |  |  |  <Payload>[12 bytes]
    |  |  |  +- MediaInformationBox { // [8 bytes] @380353
    |  |  |  |      uint32     size                : 1373
    |  |  |  |      uint32     type                : minf
    |  |  |  |  }
    |  |  |  |  +- VideoMediaHeaderBox { // [20 bytes] @380361
    |  |  |  |  |      uint32     size                : 20
    |  |  |  |  |      uint32     type                : vmhd
    |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |      uint24     flags               : 1
    |  |  |  |  |      uint16     graphicsmode        : 0
    |  |  |  |  |      uint16[3]  opcolor             : [0, 0, 0]
    |  |  |  |  |  }
    |  |  |  |  +- DataInformationBox { // [8 bytes] @380381
    |  |  |  |  |      uint32     size                : 36
    |  |  |  |  |      uint32     type                : dinf
    |  |  |  |  |  }
    |  |  |  |  |  +- DataReferenceBox { // [16 bytes] @380389
    |  |  |  |  |  |      uint32     size                : 28
    |  |  |  |  |  |      uint32     type                : dref
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 1
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- DataEntryUrlBox { // [12 bytes] @380405
    |  |  |  |  |  |  |      uint32     size                : 12
    |  |  |  |  |  |  |      uint32     type                : url
    |  |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |  |      uint24     flags               : 1
    |  |  |  |  |  |  |      string     location            : ""
    |  |  |  |  |  |  |  }
    |  |  |  |  +- SampleTableBox { // [8 bytes] @380417
    |  |  |  |  |      uint32     size                : 1309
    |  |  |  |  |      uint32     type                : stbl
    |  |  |  |  |  }
    |  |  |  |  |  +- SampleDescriptionBox { // [16 bytes] @380425
    |  |  |  |  |  |      uint32     size                : 171
    |  |  |  |  |  |      uint32     type                : stsd
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 1
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- VisualSampleEntry { // [86 bytes] @380441
    |  |  |  |  |  |  |      uint32     size                : 155
    |  |  |  |  |  |  |      uint32     type                : avc1
    |  |  |  |  |  |  |      uint8[6]   reserved            : [0, 0, 0, 0, 0, 0]
    |  |  |  |  |  |  |      uint16     data_reference_index: 1
    |  |  |  |  |  |  |      uint16     pre_defined         : 0
    |  |  |  |  |  |  |      uint16     reserved            : 0
    |  |  |  |  |  |  |      uint32[3]  pre_defined         : [0, 0, 0]
    |  |  |  |  |  |  |      uint16     width               : 560
    |  |  |  |  |  |  |      uint16     height              : 320
    |  |  |  |  |  |  |      uint32     horizresolution     : 72
    |  |  |  |  |  |  |      uint32     vertresolution      : 72
    |  |  |  |  |  |  |      uint32     reserved            : 0
    |  |  |  |  |  |  |      uint16     frame_count         : 1
    |  |  |  |  |  |  |      string     compressorname      : "JVT/AVC Coding"
    |  |  |  |  |  |  |      uint16     depth               : 24
    |  |  |  |  |  |  |      int16      pre_defined         : -1
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  |  |  <Payload>[69 bytes]
    |  |  |  |  |  +- TimeToSampleBox { // [16 bytes] @380596
    |  |  |  |  |  |      uint32     size                : 24
    |  |  |  |  |  |      uint32     type                : stts
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 1
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [8 bytes] @380612
    |  |  |  |  |  |  |      uint32     sample_count        : 166
    |  |  |  |  |  |  |      uint32     sample_delta        : 3000
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- SampleSizeBox { // [20 bytes] @380620
    |  |  |  |  |  |      uint32     size                : 684
    |  |  |  |  |  |      uint32     type                : stsz
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     sample_size         : 0
    |  |  |  |  |  |      uint32     sample_count        : 166
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [4 bytes] @380640
    |  |  |  |  |  |  |      uint32     entry_size          : 22055
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #1 { // [4 bytes] @380644
    |  |  |  |  |  |  |      uint32     entry_size          : 2848
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #2 { // [4 bytes] @380648
    |  |  |  |  |  |  |      uint32     entry_size          : 1468
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #3 { // [4 bytes] @380652
    |  |  |  |  |  |  |      uint32     entry_size          : 1506
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #4 { // [4 bytes] @380656
    |  |  |  |  |  |  |      uint32     entry_size          : 1473
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  |
    |  |  |  |  |  |  |  ...
    |  |  |  |  |  |  |
    |  |  |  |  |  |  +- Entry #161 { // [4 bytes] @381284
    |  |  |  |  |  |  |      uint32     entry_size          : 994
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #162 { // [4 bytes] @381288
    |  |  |  |  |  |  |      uint32     entry_size          : 1030
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #163 { // [4 bytes] @381292
    |  |  |  |  |  |  |      uint32     entry_size          : 742
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #164 { // [4 bytes] @381296
    |  |  |  |  |  |  |      uint32     entry_size          : 619
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #165 { // [4 bytes] @381300
    |  |  |  |  |  |  |      uint32     entry_size          : 629
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- SampleToChunkBox { // [16 bytes] @381304
    |  |  |  |  |  |      uint32     size                : 40
    |  |  |  |  |  |      uint32     type                : stsc
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 2
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [12 bytes] @381320
    |  |  |  |  |  |  |      uint32     first_chunk         : 1
    |  |  |  |  |  |  |      uint32     samples_per_chunk   : 4
    |  |  |  |  |  |  |      uint32     sample_description_index: 1
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #1 { // [12 bytes] @381332
    |  |  |  |  |  |  |      uint32     first_chunk         : 42
    |  |  |  |  |  |  |      uint32     samples_per_chunk   : 2
    |  |  |  |  |  |  |      uint32     sample_description_index: 1
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- ChunkOffsetBox { // [16 bytes] @381344
    |  |  |  |  |  |      uint32     size                : 184
    |  |  |  |  |  |      uint32     type                : stco
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 42
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [4 bytes] @381360
    |  |  |  |  |  |  |      uint32     chunk_offset        : 168
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #1 { // [4 bytes] @381364
    |  |  |  |  |  |  |      uint32     chunk_offset        : 29670
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #2 { // [4 bytes] @381368
    |  |  |  |  |  |  |      uint32     chunk_offset        : 35828
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #3 { // [4 bytes] @381372
    |  |  |  |  |  |  |      uint32     chunk_offset        : 41992
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #4 { // [4 bytes] @381376
    |  |  |  |  |  |  |      uint32     chunk_offset        : 46948
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  |
    |  |  |  |  |  |  |  ...
    |  |  |  |  |  |  |
    |  |  |  |  |  |  +- Entry #37 { // [4 bytes] @381508
    |  |  |  |  |  |  |      uint32     chunk_offset        : 350704
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #38 { // [4 bytes] @381512
    |  |  |  |  |  |  |      uint32     chunk_offset        : 359298
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #39 { // [4 bytes] @381516
    |  |  |  |  |  |  |      uint32     chunk_offset        : 365867
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #40 { // [4 bytes] @381520
    |  |  |  |  |  |  |      uint32     chunk_offset        : 373722
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #41 { // [4 bytes] @381524
    |  |  |  |  |  |  |      uint32     chunk_offset        : 378471
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- SyncSampleBox { // [16 bytes] @381528
    |  |  |  |  |  |      uint32     size                : 20
    |  |  |  |  |  |      uint32     type                : stss
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 1
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [4 bytes] @381544
    |  |  |  |  |  |  |      uint32     sample_number       : 1
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- SampleDependencyTypeBox { // [12 bytes] @381548
    |  |  |  |  |  |      uint32     size                : 178
    |  |  |  |  |  |      uint32     type                : sdtp
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [1 bytes] @381560
    |  |  |  |  |  |  |      uint2      is_leading          : 0
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #1 { // [1 bytes] @381561
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #2 { // [1 bytes] @381562
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #3 { // [1 bytes] @381563
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #4 { // [1 bytes] @381564
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  |
    |  |  |  |  |  |  |  ...
    |  |  |  |  |  |  |
    |  |  |  |  |  |  +- Entry #161 { // [1 bytes] @381721
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #162 { // [1 bytes] @381722
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #163 { // [1 bytes] @381723
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #164 { // [1 bytes] @381724
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #165 { // [1 bytes] @381725
    |  |  |  |  |  |  |      uint2      is_leading          : 1
    |  |  |  |  |  |  |      uint2      sample_depends_on   : 0
    |  |  |  |  |  |  |      uint2      sample_is_depended_on: 1
    |  |  |  |  |  |  |      uint2      sample_has_redundancy: 0
    |  |  |  |  |  |  |  }
    |  +- TrackBox { // [8 bytes] @381726
    |  |      uint32     size                : 1662
    |  |      uint32     type                : trak
    |  |  }
    |  |  +- TrackHeaderBox { // [92 bytes] @381734
    |  |  |      uint32     size                : 92
    |  |  |      uint32     type                : tkhd
    |  |  |      uint8      version             : 0
    |  |  |      uint24     flags               : 3
    |  |  |      uint32     creation_time       : Sat Mar 20 21:29:11 2010
    |  |  |      uint32     modification_time   : Sat Mar 20 21:29:12 2010
    |  |  |      uint32     track_ID            : 2
    |  |  |      uint32     reserved            : 0
    |  |  |      uint32     duration            : 501120
    |  |  |      uint32[2]  reserved            : [0, 0]
    |  |  |      int16      layer               : 0
    |  |  |      int16      alternate_group     : 0
    |  |  |      int16      volume              : 1
    |  |  |      uint16     reserved            : 0
    |  |  |      int32[9]   matrix              : [65536, 0, 0, 0, 65536, 0, 0, 0, 1073741824]
    |  |  |      uint32     width               : 0
    |  |  |      uint32     height              : 0
    |  |  |  }
    |  |  +- MediaBox { // [8 bytes] @381826
    |  |  |      uint32     size                : 1540
    |  |  |      uint32     type                : mdia
    |  |  |  }
    |  |  |  +- MediaHeaderBox { // [32 bytes] @381834
    |  |  |  |      uint32     size                : 32
    |  |  |  |      uint32     type                : mdhd
    |  |  |  |      uint8      version             : 0
    |  |  |  |      uint24     flags               : 0
    |  |  |  |      uint32     creation_time       : Sat Mar 20 21:29:11 2010
    |  |  |  |      uint32     modification_time   : Sat Mar 20 21:29:12 2010
    |  |  |  |      uint32     timescale           : 48000
    |  |  |  |      uint32     duration            : 267264
    |  |  |  |      uint1      pad                 : 0
    |  |  |  |      uint5[3]   language            : eng (English)
    |  |  |  |      uint16     pre_defined         : 0
    |  |  |  |  }
    |  |  |  +- HandlerBox { // [21 bytes] @381866
    |  |  |  |      uint32     size                : 33
    |  |  |  |      uint32     type                : hdlr
    |  |  |  |      uint8      version             : 0
    |  |  |  |      uint24     flags               : 0
    |  |  |  |      uint32     pre_defined         : 0
    |  |  |  |      uint32     handler_type        : soun
    |  |  |  |      uint32[3]  reserved            : [0, 0, 0]
    |  |  |  |      string     name                : "soun?"
    |  |  |  |  }
    |  |  |  |  |  <Payload>[12 bytes]
    |  |  |  +- MediaInformationBox { // [8 bytes] @381899
    |  |  |  |      uint32     size                : 1467
    |  |  |  |      uint32     type                : minf
    |  |  |  |  }
    |  |  |  |  +- SoundMediaHeaderBox { // [16 bytes] @381907
    |  |  |  |  |      uint32     size                : 16
    |  |  |  |  |      uint32     type                : smhd
    |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |      int16      balance             : 0
    |  |  |  |  |      uint16     reserved            : 0
    |  |  |  |  |  }
    |  |  |  |  +- DataInformationBox { // [8 bytes] @381923
    |  |  |  |  |      uint32     size                : 36
    |  |  |  |  |      uint32     type                : dinf
    |  |  |  |  |  }
    |  |  |  |  |  +- DataReferenceBox { // [16 bytes] @381931
    |  |  |  |  |  |      uint32     size                : 28
    |  |  |  |  |  |      uint32     type                : dref
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 1
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- DataEntryUrlBox { // [12 bytes] @381947
    |  |  |  |  |  |  |      uint32     size                : 12
    |  |  |  |  |  |  |      uint32     type                : url
    |  |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |  |      uint24     flags               : 1
    |  |  |  |  |  |  |      string     location            : ""
    |  |  |  |  |  |  |  }
    |  |  |  |  +- SampleTableBox { // [8 bytes] @381959
    |  |  |  |  |      uint32     size                : 1407
    |  |  |  |  |      uint32     type                : stbl
    |  |  |  |  |  }
    |  |  |  |  |  +- SampleDescriptionBox { // [16 bytes] @381967
    |  |  |  |  |  |      uint32     size                : 103
    |  |  |  |  |  |      uint32     type                : stsd
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 1
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- AudioSampleEntry { // [36 bytes] @381983
    |  |  |  |  |  |  |      uint32     size                : 87
    |  |  |  |  |  |  |      uint32     type                : mp4a
    |  |  |  |  |  |  |      uint8[6]   reserved            : [0, 0, 0, 0, 0, 0]
    |  |  |  |  |  |  |      uint16     data_reference_index: 1
    |  |  |  |  |  |  |      uint32[2]  reserved            : [0, 0]
    |  |  |  |  |  |  |      uint16     channelcount        : 1
    |  |  |  |  |  |  |      uint16     samplesize          : 16
    |  |  |  |  |  |  |      uint16     pre_defined         : 0
    |  |  |  |  |  |  |      uint16     reserverd           : 0
    |  |  |  |  |  |  |      uint32     samplerate          : 48000
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  |  |  <Payload>[51 bytes]
    |  |  |  |  |  +- TimeToSampleBox { // [16 bytes] @382070
    |  |  |  |  |  |      uint32     size                : 24
    |  |  |  |  |  |      uint32     type                : stts
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 1
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [8 bytes] @382086
    |  |  |  |  |  |  |      uint32     sample_count        : 261
    |  |  |  |  |  |  |      uint32     sample_delta        : 1024
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- SampleSizeBox { // [20 bytes] @382094
    |  |  |  |  |  |      uint32     size                : 1064
    |  |  |  |  |  |      uint32     type                : stsz
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     sample_size         : 0
    |  |  |  |  |  |      uint32     sample_count        : 261
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [4 bytes] @382114
    |  |  |  |  |  |  |      uint32     entry_size          : 247
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #1 { // [4 bytes] @382118
    |  |  |  |  |  |  |      uint32     entry_size          : 219
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #2 { // [4 bytes] @382122
    |  |  |  |  |  |  |      uint32     entry_size          : 225
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #3 { // [4 bytes] @382126
    |  |  |  |  |  |  |      uint32     entry_size          : 229
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #4 { // [4 bytes] @382130
    |  |  |  |  |  |  |      uint32     entry_size          : 233
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  |
    |  |  |  |  |  |  |  ...
    |  |  |  |  |  |  |
    |  |  |  |  |  |  +- Entry #256 { // [4 bytes] @383138
    |  |  |  |  |  |  |      uint32     entry_size          : 222
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #257 { // [4 bytes] @383142
    |  |  |  |  |  |  |      uint32     entry_size          : 203
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #258 { // [4 bytes] @383146
    |  |  |  |  |  |  |      uint32     entry_size          : 205
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #259 { // [4 bytes] @383150
    |  |  |  |  |  |  |      uint32     entry_size          : 212
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #260 { // [4 bytes] @383154
    |  |  |  |  |  |  |      uint32     entry_size          : 109
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- SampleToChunkBox { // [16 bytes] @383158
    |  |  |  |  |  |      uint32     size                : 40
    |  |  |  |  |  |      uint32     type                : stsc
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 2
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [12 bytes] @383174
    |  |  |  |  |  |  |      uint32     first_chunk         : 1
    |  |  |  |  |  |  |      uint32     samples_per_chunk   : 7
    |  |  |  |  |  |  |      uint32     sample_description_index: 1
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #1 { // [12 bytes] @383186
    |  |  |  |  |  |  |      uint32     first_chunk         : 38
    |  |  |  |  |  |  |      uint32     samples_per_chunk   : 2
    |  |  |  |  |  |  |      uint32     sample_description_index: 1
    |  |  |  |  |  |  |  }
    |  |  |  |  |  +- ChunkOffsetBox { // [16 bytes] @383198
    |  |  |  |  |  |      uint32     size                : 168
    |  |  |  |  |  |      uint32     type                : stco
    |  |  |  |  |  |      uint8      version             : 0
    |  |  |  |  |  |      uint24     flags               : 0
    |  |  |  |  |  |      uint32     entry_count         : 38
    |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #0 { // [4 bytes] @383214
    |  |  |  |  |  |  |      uint32     chunk_offset        : 28045
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #1 { // [4 bytes] @383218
    |  |  |  |  |  |  |      uint32     chunk_offset        : 34203
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #2 { // [4 bytes] @383222
    |  |  |  |  |  |  |      uint32     chunk_offset        : 40420
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #3 { // [4 bytes] @383226
    |  |  |  |  |  |  |      uint32     chunk_offset        : 45345
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #4 { // [4 bytes] @383230
    |  |  |  |  |  |  |      uint32     chunk_offset        : 49831
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  |
    |  |  |  |  |  |  |  ...
    |  |  |  |  |  |  |
    |  |  |  |  |  |  +- Entry #33 { // [4 bytes] @383346
    |  |  |  |  |  |  |      uint32     chunk_offset        : 357564
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #34 { // [4 bytes] @383350
    |  |  |  |  |  |  |      uint32     chunk_offset        : 364185
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #35 { // [4 bytes] @383354
    |  |  |  |  |  |  |      uint32     chunk_offset        : 372249
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #36 { // [4 bytes] @383358
    |  |  |  |  |  |  |      uint32     chunk_offset        : 377067
    |  |  |  |  |  |  |  }
    |  |  |  |  |  |  +- Entry #37 { // [4 bytes] @383362
    |  |  |  |  |  |  |      uint32     chunk_offset        : 379719
    |  |  |  |  |  |  |  }
    |  |  +- UserDataBox { // [8 bytes] @383366
    |  |  |      uint32     size                : 22
    |  |  |      uint32     type                : udta
    |  |  |  }
    |  |  |  +- <undeclared>::Box('name') { // [8 bytes] @383374
    |  |  |  |      uint32     size                : 14
    |  |  |  |      uint32     type                : name
    |  |  |  |  }
    |  |  |  |  |  <Payload>[6 bytes]
    |  +- UserDataBox { // [8 bytes] @383388
    |  |      uint32     size                : 111
    |  |      uint32     type                : udta
    |  |  }
    |  |  +- MetaBox { // [12 bytes] @383396
    |  |  |      uint32     size                : 103
    |  |  |      uint32     type                : meta
    |  |  |      uint8      version             : 0
    |  |  |      uint24     flags               : 0
    |  |  |  }
    |  |  |  +- HandlerBox { // [21 bytes] @383408
    |  |  |  |      uint32     size                : 33
    |  |  |  |      uint32     type                : hdlr
    |  |  |  |      uint8      version             : 0
    |  |  |  |      uint24     flags               : 0
    |  |  |  |      uint32     pre_defined         : 0
    |  |  |  |      uint32     handler_type        : mdir
    |  |  |  |      uint32[3]  reserved            : [0, 0, 0]
    |  |  |  |      string     name                : "mdir?"
    |  |  |  |  }
    |  |  |  |  |  <Payload>[12 bytes]
    |  |  |  +- <undeclared>::Box('ilst') { // [8 bytes] @383441
    |  |  |  |      uint32     size                : 58
    |  |  |  |      uint32     type                : ilst
    |  |  |  |  }
    |  |  |  |  |  <Payload>[50 bytes]
    +- FreeSpaceBox('free') { // [8 bytes] @383499
    |      uint32     size                : 132
    |      uint32     type                : free
    |  }
    |  |  <Payload>[124 bytes]

    real	0m0.012s
    user	0m0.010s
    sys	0m0.001s
    ```
