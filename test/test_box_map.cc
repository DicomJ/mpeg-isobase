#include <mpeg-isobase/box_map.h>
#include <gtest/gtest.h>


using namespace mpeg::isobase;


struct TC: Box {
    using Box::Box;
    static constexpr Type box_type = 'tc';
};

struct ATC: Box {
    using Box::Box;
    static constexpr Type box_type[] = {'atc'};
};

struct ETC: Box {
    using Box::Box;
    static constexpr Extended::Type box_type = "etc";
};

struct AETC: Box {
    using Box::Box;
    static constexpr Extended::Type box_type[] = {"aetc"};
};

struct TCT: Box {
    static constexpr Type box_type = 'tct';
    using Box::Box;
    static constexpr Type container_type = 'tct';
};

struct ATCT: Box {
    using Box::Box;
    static constexpr Type box_type[] = {'atct'};
    static constexpr Type container_type = 'atct';
};

struct ETCT: Box {
    using Box::Box;
    static constexpr Extended::Type box_type = "etct";
    static constexpr Type container_type = 'etct';
};
struct AETCT: Box {
    using Box::Box;
    static constexpr Extended::Type box_type[] = {"aetct"};
    static constexpr Type container_type = 'AE';
};

struct TCAT: Box {
    using Box::Box;
    static constexpr Type box_type = 'tcat';
    static constexpr Type container_type[] = {'tcat'};
};

struct ATCAT: Box {
    using Box::Box;
    static constexpr Type box_type[] = {'TAA1', 'TAA2'};
    static constexpr Type container_type[] = {'TAA3', 'TAA4'};
};

struct ETCAT: Box {
    using Box::Box;
    static constexpr Extended::Type box_type = "EA";
    static constexpr Type container_type[] = {'EA'};
};
struct AETCAT: Box {
    using Box::Box;
    static constexpr Extended::Type box_type[] = {"EAT1", "EAT2"};
    static constexpr Type container_type[] = {'EAT3', 'EAT4'};
};

struct TCET: Box {
    using Box::Box;
    static constexpr Type box_type = 'tcet';
    static constexpr Extended::Type container_type = "tcet";
};

struct ATCET: Box {
    using Box::Box;
    static constexpr Type box_type[] = {'ATET'};
    static constexpr Extended::Type container_type = "ATET";
};

struct ETCET: Box {
    using Box::Box;
    static constexpr Extended::Type box_type = "etet";
    static constexpr Extended::Type container_type = "etet";
};
struct AETCET: Box {
    using Box::Box;
    static constexpr Extended::Type box_type[] = {"A2ET"};
    static constexpr Extended::Type container_type = "A2ET";
};

struct TCAET: Box {
    using Box::Box;
    static constexpr Type box_type = 'acet';
    static constexpr Extended::Type container_type[] = {"tcet"};
};

struct ATCAET: Box {
    using Box::Box;
    static constexpr Type box_type[] = {'AE1', 'AE2'};
    static constexpr Extended::Type container_type[] = {"FEDCBA9876543210", {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}};
};

struct ETCAET: Box {
    using Box::Box;
    static constexpr Extended::Type box_type = "etcaet";
    static constexpr Extended::Type container_type[] = {"etcaet"};
};

struct AETCAET: Box {
    using Box::Box;
    static constexpr Extended::Type box_type[] = {"aetcaet1", "aetcaet2"};
    static constexpr Extended::Type container_type[] = {"aetcaet3", "aetcaet4"};
};


TEST(BoxMap, definition) {

    Box::Map boxes;
    boxes.add<TC>();
    boxes.add<ATC>();
    boxes.add<ETC>();
    boxes.add<AETC>();

    boxes.add<TCT>();
    boxes.add<ATCT>();
    boxes.add<ETCT>();
    boxes.add<AETCT>();

    boxes.add<TCAT>();
    boxes.add<ATCAT>();
    boxes.add<ETCAT>();
    boxes.add<AETCAT>();

    boxes.add<TCET>();
    boxes.add<ATCET>();
    boxes.add<ETCET>();
    boxes.add<AETCET>();

    boxes.add<TCAET>();
    boxes.add<ATCAET>();
    boxes.add<ETCAET>();
    boxes.add<AETCAET>();
    if (true) {
        boxes.print(std::cout);
    }
}

