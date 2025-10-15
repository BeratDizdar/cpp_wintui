#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences

namespace winapi {

    class tui {
        int cur_x = 1;
        int cur_y = 1;
    public:
        tui() {
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            GetConsoleMode(handle, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
            SetConsoleMode(handle, dwMode);
        }

        ~tui() {
            std::cout << "ADIOS\n";
        }

        // == Cursor Positioning ===
        tui& at(int x, int y) {
            std::cout << "\033[" << y << ";" << x << "H";
            cur_x = x;
            cur_y = y;
            return *this;
        }

        // == Cursor Visibility ====
        tui& blink_on() {
            std::cout << "\033[?12h";
            return *this;
        }
        tui& blink_off() {
            std::cout << "\033[?12l";
            return *this;
        }
        tui& show_cursor() {
            std::cout << "\033[?25h";
            return *this;
        }
        tui& hide_cursor() {
            std::cout << "\033[?25l";
            return *this;
        }

        // == Cursor Shape =========
        // TODO

        // == Viewport Positioning =
        tui& scroll_up(int n) {
            std::cout << "\033[" << n << "S";
            return *this;
        }
        tui& scroll_down(int n) {
            std::cout << "\033[" << n << "T";
            return *this;
        }

        // == Text Modification ====
        tui& clear() {
            std::cout << "\033[2J";
            return *this;
        }
        tui& clear_line() {
            std::cout << "\033[2K";
            return *this;
        }

        // == Text Formatting ======
        tui& reset() {
            std::cout << "\033[0m";
            return *this;
        }
        tui& bold() {
            std::cout << "\033[1m";
            return *this;
        }
        tui& underline() {
            std::cout << "\033[4m";
            return *this;
        }
        tui& negative() {
            std::cout << "\033[7m";
            return *this;
        }
        tui& fg(int r, int g, int b) {
            std::cout << "\033[38;2;"<< r << ";" << g << ";" << b <<"m";
            return *this;
        }
        tui& bg(int r, int g, int b) {
            std::cout << "\033[48;2;"<< r << ";" << g << ";" << b <<"m";
            return *this;
        }

        // == Mode Changes =========
        // TODO

        // == Query State ==========
        // TODO

        // == Tabs =================
        // TODO

        // == Designate Char. Set ==
        tui& set_ascii(int r, int g, int b) {
            std::cout << "\033(B";
            return *this;
        }
        tui& set_dec(int r, int g, int b) {
            std::cout << "\033(0";
            return *this;
        }

        // == Scrolling Margins ====
        // TODO

        // == Window Title =========
        tui& title(std::string title) {
            std::cout << "\033]2;" << title << "\x07";
            return *this;
        }

        // == Alternate Scr. Buf. ==
        // TODO

        // == Window Width =========
        // TODO
        
        // == Soft Reset ===========
        // TODO

        // == Input Sequences ======
        // TODO

        tui& info() {
            //todo
            return *this;
        }

        // == QoL ==================
        tui& print_directory() {
            std::vector<std::string> files;
            WIN32_FIND_DATAA fd;
            HANDLE hFind = FindFirstFileA("*.*", &fd);
            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                        files.push_back(fd.cFileName);
                    }
                } while (FindNextFileA(hFind, &fd));
                FindClose(hFind);
            }
            size_t maxfilelen = 0;
            for (auto& f : files) if (f.size() > maxfilelen) maxfilelen = f.size();

            clear().at(1,1).rect(2 + maxfilelen, 2 + files.size());
            for (int i = 0; i < files.size(); i++) {
                at(2,2 + i) << files[i];
            }
            return *this;
        }

        tui& rect(int w, int h) {
            if (w < 2 || h < 2) return *this;
            std::cout << "\033(0";

            // Üst kenar
            std::cout << "\033[" << cur_y << ";" << cur_x << "H";
            std::cout << "l";
            for (int i = 0; i < w - 2; ++i) std::cout << "q"; 
            std::cout << "k";
            
            // Yanlar
            for (int i = 1; i < h - 1; ++i) {
                std::cout << "\033[" << (cur_y + i) << ";" << cur_x << "H";
                std::cout << "x"; 
                for (int j = 0; j < w - 2; ++j) std::cout << " "; 
                std::cout << "x"; 
            }

            // Alt kenar
            std::cout << "\033[" << (cur_y + h - 1) << ";" << cur_x << "H";
            std::cout << "m";          
            for (int i = 0; i < w - 2; ++i) std::cout << "q"; 
            std::cout << "j";          

            std::cout << "\033(B";
            return *this;
        }

        template <typename type>
        tui& write(type value) {
            std::cout << value;
            return *this;
        }

        template <typename type>
        tui& operator<<(type value) {
            std::cout << value;
            return *this;
        }
    };
}