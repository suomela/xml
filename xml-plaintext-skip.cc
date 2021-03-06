#include <iostream>
#include <string>
#include <vector>
#include "pugixml/src/pugixml.hpp"

struct Walker : pugi::xml_tree_walker
{
    Walker(std::string skipped_) : skipped{skipped_}, depth_seen{-1} {}

    virtual bool for_each(pugi::xml_node& node) {
        using namespace pugi;
        if (depth_seen != -1 && depth() <= depth_seen) {
            depth_seen = -1;
        }
        if (depth_seen == -1 && node.type() == node_element && skipped == node.name()) {
            depth_seen = depth();
        }
        if (depth_seen == -1 && node.type() == node_pcdata) {
            std::cout << node.value();
        }
        return true;
    }

    std::string skipped;
    int depth_seen;
};

static void parse_path(std::vector<std::string>& path, const char* p) {
    const char* q = p;
    while (true) {
        bool end = (*q == 0);
        bool delim = (*q == '/');
        if (end || delim) {
            if (p != q) {
                path.push_back(std::string(p, q-p));
            }
        }
        if (end) {
            return;
        }
        ++q;
        if (delim) {
            p = q;
        } 
    }
}

int main(int argc, const char** argv) {
    std::ios_base::sync_with_stdio(0);
    if (argc < 3) {
        std::cerr << "usage: ./xml-plaintext PATH SKIPPED FILE ..." << std::endl;
        std::exit(1);
    }
    std::vector<std::string> path;
    parse_path(path, argv[1]);
    std::string skipped{argv[2]};
    for (int i = 3; i < argc; ++i) {
        const char* filename = argv[i];
        using namespace pugi;
        unsigned opt = parse_default | parse_ws_pcdata;
        xml_document doc;
        xml_parse_result result = doc.load_file(filename, opt, encoding_utf8);
        if (!result) {
            std::cerr << filename << ": error: " << result.description() << std::endl;
            std::exit(1);
        }
        xml_node node = doc;
        for (auto& elem : path) {
            node = node.child(elem.c_str());
            if (!node) {     
                std::cerr << filename << ": could not find element " << elem << std::endl;
                std::exit(1);
            }
        }
        Walker w(skipped);
        node.traverse(w);
    }
}
