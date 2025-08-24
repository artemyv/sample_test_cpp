#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <array>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
namespace
{
    // Helper to run a process and provide input via stdin
    std::string run_client_with_input(const std::filesystem::path& exe, std::string_view input)
    {
        if(!std::filesystem::exists(exe))
            throw std::invalid_argument("Executable not found: " + exe.string());

        std::array<int, 2> inPipe;
        std::array<int, 2> outPipe;
        if(pipe(inPipe.data()) != 0 || pipe(outPipe.data()) != 0) {
            throw std::invalid_argument(std::string("pipe() failed: ") + std::to_string(errno));
        }

        pid_t pid = fork();
        if(pid < 0) {
            throw std::invalid_argument(std::string("fork() failed: ") + std::to_string(errno));
        }
        if(pid == 0) {
            // Child
            // Redirect stdin
            dup2(inPipe[0], STDIN_FILENO);
            // Redirect stdout & stderr
            dup2(outPipe[1], STDOUT_FILENO);
            dup2(outPipe[1], STDERR_FILENO);

            // Close unused fds
            close(inPipe[0]); 
            close(inPipe[1]);
            close(outPipe[0]); 
            close(outPipe[1]);

            execl(exe.c_str(), exe.filename().c_str(), (char*)nullptr);
            _exit(127); // exec failed
        }

        // Parent
        close(inPipe[0]);
        close(outPipe[1]);

        // Send input then close stdin
        if(!input.empty()) {
            ssize_t written = write(inPipe[1], input.data(), input.size());
            (void)written;
        }
        close(inPipe[1]);

        // Read all output
        std::string output;
        char buf[4096];
        for(;;) {
            ssize_t n = read(outPipe[0], buf, sizeof(buf));
            if(n > 0) {
                output.append(buf, static_cast<size_t>(n));
            }
            else if(n == 0) {
                break;
            }
            else {
                if(errno == EINTR) continue;
                break;
            }
        }
        close(outPipe[0]);

        int status = 0;
        waitpid(pid, &status, 0);
        // (Optional) append exit code info for debugging
        output.append("\n[exit-status]=" + std::to_string(status));
        return output;
    }
}
namespace ClientExeTest
{

    TEST(ClientExeTest, IncorrectPathShowsError)
    {
        auto exe_path = std::filesystem::absolute(CLIENT_TARGET_NAME);
        std::string incorrect_path = "/this/path/does/not/exist.dll\n";
        std::string output = run_client_with_input(exe_path, incorrect_path);
        // Should mention file does not exist or similar error
        EXPECT_NE(output.find("File does not exist"), std::string::npos);
    }

    TEST(ClientExeTest, EmptyPathShowsError)
    {
        auto exe_path = std::filesystem::absolute(CLIENT_TARGET_NAME);
        std::string empty_path = "\n";
        std::string output = run_client_with_input(exe_path, empty_path);
        // Should mention path cannot be empty
        EXPECT_NE(output.find("Path cannot be empty"), std::string::npos);
    }

    TEST(ClientExeTest, DirPath)
    {
        auto exe_path = std::filesystem::absolute(CLIENT_TARGET_NAME);
        std::string dir_path = exe_path.parent_path().string() + "\n";
        std::string output = run_client_with_input(exe_path, dir_path);
        // Should mention path cannot be empty
        EXPECT_NE(output.find("Path is not a regular file"), std::string::npos);
    }

    TEST(ClientExeTest, NormalPath)
    {
        auto exe_path = std::filesystem::absolute(CLIENT_TARGET_NAME);
        std::string normal_path = std::filesystem::absolute(COMPONENT_TARGET_NAME).string() + "\n";
        std::string output = run_client_with_input(exe_path, normal_path);
        // Should mention path cannot be empty
        EXPECT_NE(output.find("GetVersion succeeded"), std::string::npos);
    }

    TEST(ClientExeTest, NormalPathQuotes)
    {
        auto exe_path = std::filesystem::absolute(CLIENT_TARGET_NAME);
        std::string normal_path = '"' + std::filesystem::absolute(COMPONENT_TARGET_NAME).string() + "\"\n";
        std::string output = run_client_with_input(exe_path, normal_path);
        // Should mention path cannot be empty
        EXPECT_NE(output.find("GetVersion succeeded"), std::string::npos);
    }

    TEST(ClientExeTest, NormalPathBadComponent)
    {
        auto exe_path = std::filesystem::absolute(CLIENT_TARGET_NAME);
        std::string normal_path = std::filesystem::absolute(BAD_COMPONENT_TARGET_NAME).string() + "\n";
        std::string output = run_client_with_input(exe_path, normal_path);
        // Should mention path cannot be empty
        EXPECT_NE(output.find("GetVersion failed with error"), std::string::npos);
    }

    TEST(ClientExeTest, NormalPathBadComponentNewLine)
    {
        auto exe_path = std::filesystem::absolute(CLIENT_TARGET_NAME);
        std::string normal_path = std::filesystem::absolute(BAD_COMPONENT_TARGET_NAME).string() + "\n\n";
        std::string output = run_client_with_input(exe_path, normal_path);
        // Should mention path cannot be empty
        EXPECT_NE(output.find("GetVersion failed with error"), std::string::npos);
    }
}