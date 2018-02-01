#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/box_map.h>
#include <gtest/gtest.h>

TEST(Boxes, print) {
    mpeg::isobase::Parser::all_boxes.print(std::cout);
}

