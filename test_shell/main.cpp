#include <iostream>
#include <string>
#include <cstdlib> // system �Լ�

#ifdef _DEBUG
// Debug ���(GTEST ����) 
#include <gmock/gmock.h>

TEST(SampleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else
// Release ���(��ɾ� Shell ����)
int main() {
    std::string command;
    while (true) {
        std::cout << "��ɾ� �Է� (done �Է� �� ����): ";
        std::getline(std::cin, command);

        if (command == "done") {
            std::cout << "���α׷��� �����մϴ�." << std::endl;
            break;
        }
        int result = system(command.c_str());
        std::cout << "���� ��� �ڵ�: " << result << std::endl;
    }
    return 0;
}
#endif