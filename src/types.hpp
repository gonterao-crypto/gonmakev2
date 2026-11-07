#pragma once
#include <filesystem>
#include <vector>


namespace fs = std::filesystem;


inline fs::path maketemppath = "./.gonmake2/";

enum class filesearch_type {
    whitelist,
    blacklist
};

struct filesearch_card {
    filesearch_type type = filesearch_type::blacklist;
    std::string wildcard;
};

struct arg {
    bool isargecho = false;
    bool isfilesearchcardsecho = false;
    bool isbuildfilesecho = false;
    bool isbuildcommandecho = false;
    bool isdemo = false;
    bool isdiff = false;
    unsigned int threadcount = 2;
    fs::path gonmakefilepath = "";
    fs::path builddir = "";
    fs::path outprogrampath = "./default.exe";

    std::vector<filesearch_card> filesearchcards;
};

struct buildqueue {
    fs::path filepath = "";
    fs::path outpath = "";
};

enum class commandstringobj_type {
    planetext,
    filepath,
    outpath,
    temppaths,
};

struct commandstringobj {
    commandstringobj_type type = commandstringobj_type::planetext;
    std::string text = "";
};

struct css_t {
    std::vector<commandstringobj> eachcss;
    std::vector<commandstringobj> linkcss;
};