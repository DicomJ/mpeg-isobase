#include <iostream>
#include <bitstream/ifstream.h>
#include <bitstream/opstream.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/boxes.h>

int main(int argc, const char **argv) {
    for (int i = 1; i < argc; ++i) {
        try {
            bitstream::input::file::Stream stream(argv[i]);
            mpeg::isobase::Parser parser(stream, bitstream::output::print::to_stdout);
            parser.parse();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

