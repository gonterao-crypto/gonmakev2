#include "optout.hpp"

void cout_bool(bool value) {
    std::cout << gon::CDECO_HIGH << (value ? gon::CCOLOR_GREEN : gon::CCOLOR_RED) << (value ? "TRUE" : "FALSE") << gon::CDC_DEFAULT;
}