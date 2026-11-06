#include "error.hpp"
#include <iostream>
#include "../includes/gonconsolecolor.hpp"

void commandlineerror(const char* n) {
    std::cout << gon::CDECO_HIGH << gon::CCOLOR_RED << "Command Line Error!" << gon::CDC_DEFAULT << " at " << n << std::endl;
}
void genericerror(const char* n) {
    std::cout << gon::CDECO_HIGH << gon::CCOLOR_RED << "Generic Error!" << gon::CDC_DEFAULT << " " << n << std::endl;
}
void genericwarn(const char* n) {
    std::cout << gon::CDECO_HIGH << gon::CCOLOR_YELLOW << "Generic Warn!" << gon::CDC_DEFAULT << " " << n << std::endl;
}