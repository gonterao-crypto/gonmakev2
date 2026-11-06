#pragma once

namespace gon {
#ifdef GONLIB_TYPO
    inline
#endif
    namespace v1 {
        constexpr const char* CDECO_HIGH = "\033[1m";
        constexpr const char* CDECO_UNDERLINE = "\033[4m";
        constexpr const char* CDECO_FLASHING = "\033[5m";
        constexpr const char* CDECO_IREKAE = "\033[7m";
        constexpr const char* CDECO_DEFAULT = "\033[0m";
        constexpr const char* CCOLOR_BLACK = "\033[30m";
        constexpr const char* CCOLOR_RED = "\033[31m";
        constexpr const char* CCOLOR_GREEN = "\033[32m";
        constexpr const char* CCOLOR_YELLOW = "\033[33m";
        constexpr const char* CCOLOR_BLUE = "\033[34m";
        constexpr const char* CCOLOR_MAZENDA = "\033[35m";
        constexpr const char* CCOLOR_SYAN = "\033[36m";
        constexpr const char* CCOLOR_WHITE = "\033[37m";
        constexpr const char* CCOLOR_DEFAULT = "\033[39m";
        constexpr const char* CBG_RED = "\033[41m";
        constexpr const char* CBG_GREEN = "\033[42m";
        constexpr const char* CBG_YELLOW = "\033[43m";
        constexpr const char* CBG_BLUE = "\033[44m";
        constexpr const char* CBG_MAZENDA = "\033[45m";
        constexpr const char* CBG_SYAN = "\033[46m";
        constexpr const char* CBG_WHITE = "\033[47m";
        constexpr const char* CBG_DEFAULT = "\033[49m";
        constexpr const char* CDC_DEFAULT = "\033[0m\033[39m";
        constexpr const char* CDB_DEFAULT = "\033[0m\033[49m";
        constexpr const char* CCB_DEFAULT = "\033[39m\033[49m";
        constexpr const char* CDCB_DEFAULT = "\033[0m\033[39m\033[49m";
    }
#ifndef GONLIB_TYPO
    inline
#endif
    namespace v2 {
        constexpr const char* CDECO_HIGH = "\033[1m";
        constexpr const char* CDECO_UNDERLINE = "\033[4m";
        constexpr const char* CDECO_FLASHING = "\033[5m";
        constexpr const char* CDECO_REVERSE = "\033[7m";
        constexpr const char* CDECO_DEFAULT = "\033[0m";
        constexpr const char* CCOLOR_BLACK = "\033[30m";
        constexpr const char* CCOLOR_RED = "\033[31m";
        constexpr const char* CCOLOR_GREEN = "\033[32m";
        constexpr const char* CCOLOR_YELLOW = "\033[33m";
        constexpr const char* CCOLOR_BLUE = "\033[34m";
        constexpr const char* CCOLOR_MAGENTA = "\033[35m";
        constexpr const char* CCOLOR_CYAN = "\033[36m";
        constexpr const char* CCOLOR_WHITE = "\033[37m";
        constexpr const char* CCOLOR_DEFAULT = "\033[39m";
        constexpr const char* CBG_RED = "\033[41m";
        constexpr const char* CBG_GREEN = "\033[42m";
        constexpr const char* CBG_YELLOW = "\033[43m";
        constexpr const char* CBG_BLUE = "\033[44m";
        constexpr const char* CBG_MAGENTA = "\033[45m";
        constexpr const char* CBG_CYAN = "\033[46m";
        constexpr const char* CBG_WHITE = "\033[47m";
        constexpr const char* CBG_DEFAULT = "\033[49m";
        constexpr const char* CDC_DEFAULT = "\033[0m\033[39m";
        constexpr const char* CDB_DEFAULT = "\033[0m\033[49m";
        constexpr const char* CCB_DEFAULT = "\033[39m\033[49m";
        constexpr const char* CDCB_DEFAULT = "\033[0m\033[39m\033[49m";
    }
}