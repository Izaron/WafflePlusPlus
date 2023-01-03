#include <gtest/gtest.h>
#include "file_impl.h"

using namespace waffle;

static std::string Slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

TEST(FileTest, Simple) {
    // write to files "test0.txt", "test1.txt", "test2.txt"
    {
        FileManager mgr{/*prefix=*/"."};
        for (int i = 0; i < 12; ++i) {
            std::string fileName = "test" + std::to_string(i % 3) + ".txt";
            auto& printer = mgr.GetOrCreateFilePrinter(fileName);
            printer << std::to_string(i) << "\n";
        }
    }

    // check files "test0.txt", "test1.txt", "test2.txt"
    {
        std::ifstream stream{"./test0.txt"};
        ASSERT_STREQ(Slurp(stream).c_str(), "0\n3\n6\n9\n");
    }
    {
        std::ifstream stream{"./test1.txt"};
        ASSERT_STREQ(Slurp(stream).c_str(), "1\n4\n7\n10\n");
    }
    {
        std::ifstream stream{"./test2.txt"};
        ASSERT_STREQ(Slurp(stream).c_str(), "2\n5\n8\n11\n");
    }
}
