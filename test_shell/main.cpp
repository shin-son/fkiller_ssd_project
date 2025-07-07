#include <iostream>
#include <string>
#include <cstdlib> // system 함수

#ifdef _DEBUG
// Debug 모드(GTEST 실행) 
#include <gmock/gmock.h>

TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else
// Release 모드(명령어 Shell 실행)
int main() {
    std::string command;
    while (true) {
        std::cout << "명령어 입력 (done 입력 시 종료): ";
        std::getline(std::cin, command);

        if (command == "done") {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }
        int result = system(command.c_str());
        std::cout << "실행 결과 코드: " << result << std::endl;
    }
    return 0;
}
#endif