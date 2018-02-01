#include <iostream>
#include <bitstream/ifstream.h>
#include <mpeg-isobase/parser.h>
#include <mpeg-isobase/box/file_type.h>


int main(int argc, const char **argv) {
    for (int i = 1; i < argc; ++i) {
        try {
            struct: bitstream::Parser::Observer {
                virtual void event(const bitstream::Parser::Event::Header &event) {
                    auto box = dynamic_cast<const mpeg::isobase::FileTypeBox *>(&event.header);
                    if (box != nullptr) {
                        assert(box->type() == 'ftyp');
                        std::cout << "Found 'ftyp' box of " << box->size() << " bytes size." << std::endl;
                    }
                }
            } observer;
            bitstream::input::file::Stream stream(argv[i]);
            mpeg::isobase::Parser parser(stream, observer);
            parser.parse();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

