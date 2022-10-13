#pragma once
#include <cstdint>
#include <string>

namespace Base {
    struct Utils {

        static constexpr uint32_t hash(const char *data, size_t const size) noexcept {
            uint32_t hash = 5381;

            for (const char *c = data; c < data + size; ++c)
                hash = ((hash << 5) + hash) + (unsigned char) *c;

            return hash;

        }

        static bool stringReplace(std::string& str, const std::string& from, const std::string& to) {
            size_t start_pos = str.find(from);
            if(start_pos == std::string::npos)
                return false;
            str.replace(start_pos, from.length(), to);
            return true;
        }


    static int strpos(char *haystack, char *needle, int nth)
    {
        char *res = haystack;
        for(int i = 1; i <= nth; i++)
        {
            res = strstr(res, needle);
            if (!res)
                return -1;
            else if(i != nth)
                res++;
        }
        return res - haystack;
    }
    };
}