#include "arg.hpp"
#include "error.hpp"
#include <iostream>
#include <string_view>


bool to_10sinsu(const std::string& string, int& number) {
    int kurai = 1;
    int min = 0;
    if (string[0] == '-') {
        kurai = -kurai;
        min = 1;
    }
    for (int i = string.length() - 1; i >= min; --i) {
        char n = string[i];
        if (n == '0') number += kurai * 0;
        else if (n == '1') number += kurai * 1;
        else if (n == '2') number += kurai * 2;
        else if (n == '3') number += kurai * 3;
        else if (n == '4') number += kurai * 4;
        else if (n == '5') number += kurai * 5;
        else if (n == '6') number += kurai * 6;
        else if (n == '7') number += kurai * 7;
        else if (n == '8') number += kurai * 8;
        else if (n == '9') number += kurai * 9;
        else {
            commandlineerror("Invalid characters");
            return false;
        }
        kurai *= 10;
    }
    return true;
}


enum class argparse_step {
    first,
    gonmakefilewait,
    builddirwait,
    outprogrampathwait,
    threadcountwait
};

bool argparse(arg& rg, int argc, char *argv[]) {
    argparse_step step = argparse_step::first;
    for (int i = 1; i < argc; ++i) {
        char *n = argv[i];
        std::string_view strn(n);
        if (step == argparse_step::first) {
            if (strn == "--gmakefile") {
                step = argparse_step::gonmakefilewait;
            } else if (strn == "--dir") {
                step = argparse_step::builddirwait;
            } else if (strn == "--outpath") {
                step = argparse_step::outprogrampathwait;
            } else if (strn == "--thread") {
                step = argparse_step::threadcountwait;
            } else if (strn == "--optionecho") {
                rg.isargecho = true;
            } else if (strn == "--buildfileecho") {
                rg.isbuildfilesecho = true;
            } else if (strn == "--buildcommandecho") {
                rg.isbuildcommandecho = true;
            } else if (strn == "--fileserachcardsecho") {
                rg.isfileserachcardsecho = true;
            } else if (strn == "--diff") {
                rg.isdiff = true;
            } else if (strn == "--demo") {
                rg.isdemo = true;
            } else { commandlineerror(n); }
        } else if (step == argparse_step::gonmakefilewait) {
            rg.gonmakefilepath = n;
            step = argparse_step::first;
        } else if (step == argparse_step::builddirwait) {
            rg.builddir = n;
            step = argparse_step::first;
        } else if (step == argparse_step::outprogrampathwait) {
            rg.outprogrampath = n;
            step = argparse_step::first;
        } else if (step == argparse_step::threadcountwait) {
            {
                int h = 0;
                if (to_10sinsu(n, h)) {
                    if (h <= 0) {
                        genericwarn("The threads count is 0 or less. The build will not be executed.");
                        h = 0;
                    }
                    rg.threadcount = h;
                }
            }
            step = argparse_step::first;
        }
    }
    if (rg.gonmakefilepath.empty()) {
        genericerror("GMake file is none.");
        return false;
    }
    if (rg.builddir.empty()) {
        genericerror("Build directory is none.");
        return false;
    }
    return true;
}