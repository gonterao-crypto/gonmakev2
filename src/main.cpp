#include <iostream>
#include "types.hpp"
#include "arg.hpp"
#include "optout.hpp"
#include "gmakefile.hpp"
#include "build.hpp"
#include "../includes/gonconsolecolor.hpp"

int main(int argc, char *argv[]) {
    arg mainarg;
    bool sr = true;
    sr = argparse(mainarg, argc, argv);
    if (!sr) return 1;
    if (mainarg.isargecho) {
        std::cout << "==ARGS(OPTIONS)===" << std::endl;
        std::cout << "isargecho: "; cout_bool(mainarg.isargecho); std::cout << std::endl;
        std::cout << "isfilesearchcardsecho: "; cout_bool(mainarg.isfilesearchcardsecho); std::cout << std::endl;
        std::cout << "isbuildfilesecho: "; cout_bool(mainarg.isbuildfilesecho); std::cout << std::endl;
        std::cout << "isdemo: "; cout_bool(mainarg.isdemo); std::cout << std::endl;
        std::cout << "isdiff: "; cout_bool(mainarg.isdiff); std::cout << std::endl;
        std::cout << "gonmakefilepath: " << '"' << mainarg.gonmakefilepath.string() << '"' << std::endl; 
        std::cout << "outprogrampath: " << '"' << mainarg.outprogrampath.string() << '"' << std::endl; 
        std::cout << "threadcount: " << mainarg.threadcount << std::endl; 
    }




    std::vector<buildqueue> b;
    css_t cssl;
    std::vector<commandstringobj>& css = cssl.eachcss;
    std::vector<commandstringobj>& linkcss = cssl.linkcss;
    std::string objectfilenames;
    
    css.emplace_back(commandstringobj_type::planetext, "clang++ -c ");
    css.emplace_back(commandstringobj_type::filepath, "");
    css.emplace_back(commandstringobj_type::planetext, " -o ");
    css.emplace_back(commandstringobj_type::outpath, "");

    linkcss.emplace_back(commandstringobj_type::planetext, "clang++ ");
    linkcss.emplace_back(commandstringobj_type::temppaths, "");
    linkcss.emplace_back(commandstringobj_type::planetext, " -o ");
    linkcss.emplace_back(commandstringobj_type::outpath, "");
    sr = loadqueues(mainarg, b, cssl, objectfilenames);
    if (!sr) return 1;
    if (mainarg.isfilesearchcardsecho) {
        std::cout << "==FILESERCHCARDS===" << std::endl;
        for (int i = 0; i < mainarg.filesearchcards.size(); ++i) {
            filesearch_card& now = mainarg.filesearchcards[i];
            std::cout << "filesearchcards [" << i << "] wildcard:" << now.wildcard << std::endl;
            std::cout << "filesearchcards [" << i << "] type:" << (now.type == filesearch_type::whitelist ? "white" : "black") << std::endl;
        }
    }
    if (mainarg.isbuildfilesecho) {
        std::cout << "==BUILDFILES===" << std::endl;
        for (int i = 0; i < b.size(); ++i) {
            buildqueue& now = b[i];
            std::cout << "buildqueues [" << i << "] filepath:" << now.filepath.string() << std::endl;
        }
    }
    if (mainarg.isbuildcommandecho) {
        std::cout << "==BUILDCOMMANDS==" << std::endl;
    }
    sr = build(mainarg, b, cssl, objectfilenames);
    if (!sr) return 1;
    return 0;
}