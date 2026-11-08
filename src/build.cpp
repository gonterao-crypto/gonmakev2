#include "build.hpp"


#include <iostream>
#include <thread>
#include <atomic>
#include <syncstream>


struct workerobj {
    bool isecho;
    std::vector<commandstringobj>* commandstrings;
    std::vector<buildqueue>* objs;
    std::atomic<int>* index;
    std::atomic<int>* errorcount;
};


void compiletask(workerobj *wo, int id) {
    std::osyncstream bout{std::cout};
    if (wo->isecho) bout << "[" << id << "]" << "worker: start" << std::endl;
    int mindex = 0;
    while (true) {
        mindex = wo->index->fetch_add(1);
        if (mindex >= wo->objs->size()) break;
        if (wo->errorcount->load() != 0) break;
        buildqueue& bb = (*(wo->objs))[mindex];
        std::string inname = bb.filepath.string();
        std::string outname = bb.outpath.string();

        std::string command = "";
        for (int i = 0; i < wo->commandstrings->size(); ++i) {
            commandstringobj& now = (*wo->commandstrings)[i];
            if (now.type == commandstringobj_type::planetext) command += now.text;
            else if (now.type == commandstringobj_type::filepath) command += inname;
            else if (now.type == commandstringobj_type::outpath) command += outname;
        }

        if (wo->isecho) {
            bout << "[" << id << "]" << command << std::endl;
        }
        int er = 0;
        er = std::system(command.c_str());
        if (er != 0) {
            wo->errorcount->fetch_add(1);
            break;
        }
        bout.emit();
    }
    if (wo->isecho) bout << "[" << id << "]" << "worker: end" << std::endl;
}

void compiletaskdemo(workerobj *wo, int id) {
    std::osyncstream bout{std::cout};
    if (wo->isecho) bout << "[" << id << "]" << "worker: start" << std::endl;
    int mindex = 0;
    while (true) {
        mindex = wo->index->fetch_add(1);
        if (mindex >= wo->objs->size()) break;
        if (wo->errorcount->load() != 0) break;
        buildqueue& bb = (*(wo->objs))[mindex];
        std::string inname = bb.filepath.string();
        std::string outname = bb.outpath.string();

        std::string command = "";
        for (int i = 0; i < wo->commandstrings->size(); ++i) {
            commandstringobj& now = (*wo->commandstrings)[i];
            if (now.type == commandstringobj_type::planetext) command += now.text;
            else if (now.type == commandstringobj_type::filepath) command += inname;
            else if (now.type == commandstringobj_type::outpath) command += outname;
        }

        if (wo->isecho) {
            bout << "[" << id << "]" << command << std::endl;
        }
        int er = 0;
        // er = std::system(command.c_str());
        if (er != 0) {
            wo->errorcount->fetch_add(1);
            break;
        }
        bout.emit();
    }
    if (wo->isecho) bout << "[" << id << "]" << "worker: end" << std::endl;
}



bool build(arg& aru, std::vector<buildqueue>& queue, css_t& cssl, std::string& objectfilenames) {
    if (aru.threadcount <= 0) return false;
    if (queue.size() <= 0) return true;
    std::vector<commandstringobj>& css = cssl.eachcss;
    std::vector<commandstringobj>& linkcss = cssl.linkcss;

    fs::create_directory(maketemppath);

    std::atomic<int> index(0);
    std::atomic<int> errorcount(0);

    workerobj wo;
    wo.isecho = aru.isbuildcommandecho;
    wo.commandstrings = &css;
    wo.objs = &queue;
    wo.index = &index;
    wo.errorcount = &errorcount;
    std::vector<std::thread> threads;
    for (int i = 0; i < aru.threadcount; i++) {
        if (aru.isdemo) {
            threads.emplace_back(compiletaskdemo, &wo, i);
        } else {
            threads.emplace_back(compiletask, &wo, i);
        }
    }
    for (int i = 0; i < aru.threadcount; i++) {
        threads[i].join();
    }
    std::string& gattai = objectfilenames;
    if (wo.errorcount->load() == 0) {
        std::string command = "";
        for (int i = 0; i < linkcss.size(); ++i) {
            commandstringobj& now = (linkcss)[i];
            if (now.type == commandstringobj_type::planetext) command += now.text;
            else if (now.type == commandstringobj_type::temppaths) command += gattai;
            else if (now.type == commandstringobj_type::outpath) command += aru.outprogrampath.string();
        }
        if (wo.isecho) {
            std::cout << "[m]" << command << std::endl;
        }
        if (!aru.isdemo) {
            int er = std::system(command.c_str());
            if (er != 0) return false;
        }
    } else {
        return false;
    }
    /*
    if (wo.errork->load() == 0) {
        std::string command = linkcommandmae;
        command += " ";
        command += awasete;
        command += linkcommandusiro;
        command += " -o ";
        command += (p/outname).string();
        if (isecho) {
            std::cout << command << std::endl;
        }
        int er = std::system(command.c_str());
        if (er != 0) return 1;
        return 0;
    }
        */

    return true;
}