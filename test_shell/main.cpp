#include <iostream>
#include <string>
#include <cstdlib>

#ifdef _DEBUG
#include <gmock/gmock.h>

TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else
int main() {
    std::string command;
    while (true) {
        std::cout << " SHELL : ";
        std::getline(std::cin, command);

        if (command == "done") {
            std::cout << "PROGRAM DONE" << std::endl;
            break;
        }
        int result = system(command.c_str());
        std::cout << "SHELL COMMAND : " << result << std::endl;
    }
    return 0;
}
#endif
