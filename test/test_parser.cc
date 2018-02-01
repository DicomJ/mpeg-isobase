#include <gtest/gtest.h>
#include <bitstream/field.h>
#include <bitstream/ifstream.h>
#include <bitstream/opstream.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/boxes.h>
#include "file_list_processing.h"


TEST(Parser, dev) {
    struct: mpeg::isobase::test::FileListProcessing {
        virtual void process(const std::string &file) {
            std::cout << std::endl << file << ":" << std::endl;
            bitstream::input::file::Stream stream(file);
            mpeg::isobase::Parser parser(stream, bitstream::output::print::to_stdout);
            parser.parse();
        }
    } processing;
    processing.process_files();
}

