#include "gmakefile.hpp"
#include "error.hpp"
#include <fstream>
#include <string>
#include <iostream>

enum class hantetype {
    none,
    files,
    eachbuildcommand,
    linkbuildcommand
};

bool wild_match(const char* wildcard, const char* str) {
    const char* w = wildcard;
    const char* s = str;
    while (true) {
        if (*s == 0) {
            while (*w == '*') ++w;
            return (*w == 0);
        } else if (*w == 0) {
            return false;
        } else if (*w == '*') {
            return (*(w + 1) == 0) || wild_match(w + 1, s) || wild_match(w, s + 1);
        } else if (*w == '?' || *w == *s) {
            ++w;
            ++s;
            continue;
        } else { return false; }
    }
} //参考 https://qiita.com/gyu-don/items/5a640c6d2252a860c8cd

void getdependences(fs::path dfilepath, std::vector<fs::path>& h) {
    std::ifstream file(dfilepath);
    std::string line = "";
    std::string one = "";
    while (std::getline(file, line)) {
        one += line;
    }
    bool iskoron = false;
    std::string buffer;
    for (int i = 0; i < one.size(); ++i) {
        if (iskoron) {
            if (one[i] == ' ') {
                if (!buffer.empty()) {
                    h.emplace_back(buffer);
                }
                buffer = "";
                continue;
            }
            if (one[i] == '/' || one[i] == '\\') {
                if ((i - 1) >= 0 && one[i - 1] == ' ') {
                    continue;
                }
            }
            buffer += one[i];
        } else {
            if (one[i] == ':') {
                iskoron = true;
            }
        }
    }
    for (int i = 0; i < h.size(); ++i) {
        std::cout << h[i] << std::endl;
    }

}

bool isrecompile(arg& a, fs::path&& objectfilepath, fs::path&& dfilepath, fs::path&& cfilepath) {
    if (!fs::exists(objectfilepath) || !fs::exists(dfilepath)) return true;
    fs::file_time_type objectfilelast = fs::last_write_time(objectfilepath);

    std::vector<fs::path> dependencelist;
    dependencelist.emplace_back(cfilepath);
    dependencelist.emplace_back(a.gonmakefilepath);
    getdependences(dfilepath, dependencelist);
    bool objyorinew = false;
    for (int i = 0; i < dependencelist.size(); ++i) {
        fs::path& n = dependencelist[i];
        fs::file_time_type nfilelast = fs::last_write_time(n);
        if (nfilelast > objectfilelast) {
            objyorinew = true;
        }
    }
    return objyorinew;
}

bool loadqueues(arg& a, std::vector<buildqueue>& b, css_t& cssl, std::string& objectfilenames) {
    std::vector<commandstringobj>& css = cssl.eachcss;
    std::vector<commandstringobj>& linkcss = cssl.linkcss;

    std::ifstream file(a.gonmakefilepath);
    std::string linestr;
    filesearch_type type = filesearch_type::blacklist;
    hantetype hante = hantetype::none;
    commandstringobj obj;

    while (std::getline(file, linestr)) {
        std::string_view line = linestr;
        while (line[0] == '\t') {
            line.remove_prefix(1);
        }
        if (line[0] == '^') {
            if (line == "^files") {
                hante = hantetype::files;
                continue;
            } else if (line == "^eachbuildcommand") {
                hante = hantetype::eachbuildcommand;
                continue;
            } else if (line == "^linkbuildcommand") {
                hante = hantetype::linkbuildcommand;
                continue;
            }
        }
        if (line[0] == ';') {
            continue;
        }
        line.remove_prefix(4);
        if (line[0] == ';') {
            continue;
        }
        if (hante == hantetype::files) {
            if (line == "-black") {
                type = filesearch_type::blacklist;
            } else if (line == "-white") {
                type = filesearch_type::whitelist;
            }
            if (line[0] == '@') {
                std::string_view v = line;
                v.remove_prefix(1);
                filesearch_card card;
                card.type = type;
                card.wildcard = v;
                a.filesearchcards.push_back(card);
            }
        } else if (hante == hantetype::eachbuildcommand) {
            if (line[0] == '@') {
                std::string_view v = line;
                v.remove_prefix(1);
                if (v == "clear") {
                    css.clear();
                    continue;
                } else if (v == "infilepath") {
                    obj.text = "";
                    obj.type = commandstringobj_type::filepath;
                    css.push_back(obj);
                    continue;
                } else if (v == "outfilepath") {
                    obj.text = "";
                    obj.type = commandstringobj_type::outpath;
                    css.push_back(obj);
                    continue;
                } else if (v == "space") {
                    obj.text = " ";
                    obj.type = commandstringobj_type::planetext;
                    css.push_back(obj);
                    continue;
                }
                
            }
            obj.text = line;
            obj.type = commandstringobj_type::planetext;
            css.push_back(obj);
        } else if (hante == hantetype::linkbuildcommand) {
            if (line[0] == '@') {
                std::string_view v = line;
                v.remove_prefix(1);
                if (v == "clear") {
                    linkcss.clear();
                    continue;
                } else if (v == "temppaths") {
                    obj.text = "";
                    obj.type = commandstringobj_type::temppaths;
                    linkcss.push_back(obj);
                    continue;
                } else if (v == "outfilepath") {
                    obj.text = "";
                    obj.type = commandstringobj_type::outpath;
                    linkcss.push_back(obj);
                    continue;
                } else if (v == "space") {
                    obj.text = " ";
                    obj.type = commandstringobj_type::planetext;
                    linkcss.push_back(obj);
                    continue;
                }
                
            }
            obj.text = line;
            obj.type = commandstringobj_type::planetext;
            linkcss.push_back(obj);
        }
    }





    if (a.builddir.empty()) {
        genericerror("build dir is empty.");
        return false;
    }

    unsigned int num = 0;
    for (const fs::directory_entry& x : fs::recursive_directory_iterator(a.builddir)) {
        if (x.is_directory()) continue;
        bool ismatch = false;
        std::string k = x.path().string();
        for (int j = 0; j < k.length(); ++j) {
            if (k[j] == '\\') {
                k[j] = '/';
            }
        }


        for (int i = 0; i < a.filesearchcards.size(); ++i) {
            filesearch_card& now = a.filesearchcards[i];
            bool nmatch = wild_match(now.wildcard.c_str(), k.c_str());
            if (now.type == filesearch_type::whitelist && nmatch) {
                ismatch = true;
            }
            if (now.type == filesearch_type::blacklist && nmatch) {
                ismatch = false;
            }
        }
        if (ismatch) {
            if (k.empty()) continue;
            std::string kn = k.substr(1);
            for (int j = 0; j < kn.length(); ++j) {
                if (kn[j] == '/') {
                    kn[j] = '_';
                }
            }
            kn += ".o";
            if (a.isdiff) {
                if (isrecompile(a, maketemppath / kn, maketemppath / (kn + ".d"), k)) {
                    buildqueue q;
                    q.filepath = k;
                    q.outpath = maketemppath / kn;
                    b.push_back(q);
                }
            }
            if (!a.isdiff) {
                buildqueue q;
                q.filepath = k;
                q.outpath = maketemppath / kn;
                b.push_back(q);
            }
            objectfilenames += (maketemppath / kn).string();
            objectfilenames += ' ';
        }
    }
    return true;
}