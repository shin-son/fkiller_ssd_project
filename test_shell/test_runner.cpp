#include <filesystem>
#include <thread>
#include "runner.h"

TestRunner::TestRunner(TestShell& shell, SSDAdapter& adapter)
    : testShell(shell), ssdAdapter(adapter)
{
    shell.setSsdAdapter(&adapter);
}

bool TestRunner::isInvalidScript(const std::string& scriptPath) {
    if (!std::filesystem::exists(scriptPath) || !std::filesystem::is_regular_file(scriptPath) ||
        std::filesystem::path(scriptPath).extension() != ".txt") {
        std::cerr << "Invalid script file: " << scriptPath << "\n";
        return true;
    }
    return false;
}

bool TestRunner::cannotOpenScript(std::ifstream& script, const std::string& scriptPath) {
    if (!script) {
        std::cerr << "Cannot open: " << scriptPath << "\n";
        return true;
    }
    return false;
}

void TestRunner::runScript(std::ifstream& script, const std::string& scriptPath) {
    std::string cmd;
    while (std::getline(script, cmd)) {
        if (cmd.empty())
            continue;

        std::cerr << cmd << " RUN" << std::flush;

        std::atomic<bool> done{ false };
        std::thread dotter([&]() {
            while (!done) {
                std::cerr << '.' << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            });

        std::ostringstream hiddenBuf;
        auto* oldCout = std::cout.rdbuf(hiddenBuf.rdbuf());
        auto* oldCerr = std::cerr.rdbuf(hiddenBuf.rdbuf());

        int flag = testShell.runCommand(cmd);

        done = true;
        dotter.join();

        std::cout.rdbuf(oldCout);
        std::cerr.rdbuf(oldCerr);

        if (flag == 3) std::cerr << " PASS!\n";
        else {
            std::cerr << " FAIL!\n";
            return;
        }
    }
}

void TestRunner::verifyAndRunScript(const std::string& scriptPath) {
    if (isInvalidScript(scriptPath))
        return;

    std::ifstream script(scriptPath);
    if (cannotOpenScript(script, scriptPath))
        return;

    runScript(script, scriptPath);
    return;
}