#ifndef __MPEG_ISOBASE_TEST_FILE_LIST_PROCESSING__
#define __MPEG_ISOBASE_TEST_FILE_LIST_PROCESSING__

#include <cstdlib>
#include <fstream>
#include <cstring>
#include <tuple>
#include <iostream>


namespace mpeg {
namespace isobase {
namespace test {


struct FileListProcessing {

    std::string file_list_path, file_list_env;

    FileListProcessing(const std::string file_list_path="./test_file_list.txt",
                       const std::string &file_list_env="TEST_FILE_LIST"):
        file_list_path(file_list_path), file_list_env(file_list_env) {}

    inline void process_files();

protected:
    virtual void process(const std::string &file) = 0;
};


inline void FileListProcessing::process_files() {

    bool skip_test_if_no_files = false;
    const char *file_list_path = std::getenv(file_list_env.c_str());
    if (file_list_path == nullptr) {
        file_list_path = this->file_list_path.c_str();
        skip_test_if_no_files = true;
    }

    std::ifstream files{file_list_path};
    if (!files.is_open()) {
        auto message = std::string(file_list_path) + ": " + std::strerror(errno);
        if (skip_test_if_no_files) {
            return;
        } else {
            throw std::runtime_error(message);
        }
    }

    struct VerificationFailureCounter {

        VerificationFailureCounter(): count(0) {}

        long count;
        static void increment(void *context) {
            auto *vc = reinterpret_cast<VerificationFailureCounter *>(context);
            ++vc->count;
        }
    } verification_counter;


    std::string file;
    std::vector<std::tuple<std::string, long>> successful, failed;;
    bitstream::Const::verification = {
        .skip = false,
        .failed = VerificationFailureCounter::increment,
        .context = &verification_counter,
    };
    while (std::getline(files, file)) {

        file.erase(file.find_last_not_of(" \n\r\t")+1);
        if (file.empty() || file.find_first_of('#') == 0) {
            continue;
        }

        try {
            verification_counter.count = 0;
            process(file);
            successful.push_back(std::make_tuple(file, verification_counter.count));
        } catch (...) {
            try {
                throw;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            } catch(...) {}
            failed.push_back(std::make_tuple(file, verification_counter.count));
        }
    }

    std::cout << "Test files from " << file_list_path << ": " << std::endl;

    struct Print {
        static void files(const std::string &title, const decltype(successful) &files) {
            std::cout << std::endl << title << " - " << files.size();
            if (!files.empty()) {
                std::cout << ":";
            }
            std::cout << std::endl;
            for (const auto &file: files) {
                std::cout << std::get<0>(file);
                const auto &const_verification_failed = std::get<1>(file);
                if (const_verification_failed) {
                    std::cout << " (" << const_verification_failed << " times failed to verify const values)";
                }
                std::cout<< std::endl;
            }
        }
    };

    Print::files("Successful", successful);
    Print::files("Failed", failed);
}

}}} // namespace mpeg::isobase::test


#endif // __MPEG_ISOBASE_TEST_FILE_LIST_PROCESSING__

