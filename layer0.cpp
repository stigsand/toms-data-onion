#include "main_common.hpp"

int main()
{
    return main_common([] (auto & encoded) {
        return encoded;
    });
}
