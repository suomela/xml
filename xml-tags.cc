#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "pugixml/src/pugixml.hpp"

struct TopWalker : pugi::xml_tree_walker
{
    TopWalker(bool indent_) : indent{indent_}
    {}

    virtual bool for_each(pugi::xml_node& node) {
        using namespace pugi;
        if (node.type() == node_element) {
            if (indent) {
                int d = depth();
                for (int i = 0; i < d; ++i) {
                    std::cout << ' ';
                }
            }
            std::cout << node.name() << '\n';
        }
        return true;
    }

    bool indent;
};

struct StackWalker : pugi::xml_tree_walker
{
    StackWalker(bool reverse_) : reverse{reverse_}
    {}

    virtual bool for_each(pugi::xml_node& node) {
        using namespace pugi;
        if (node.type() == node_element) {
            stack.resize(depth());
            stack.push_back(node.name());
            int n = stack.size();
            for (int i = 0; i < n; ++i) {
                if (i) {
                    std::cout << ' ';
                }
                if (reverse) {
                    std::cout << stack[n-i-1];
                } else {
                    std::cout << stack[i];
                }
            }
            std::cout << '\n';
        }
        return true;
    }

    std::vector<std::string> stack;
    bool reverse;
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
    bool tree = false;
    bool reverse = false;
    bool indent = false;
    int i = 1;
    while (i < argc) {
        if (argv[i] == std::string("--indent")) {
            indent = true;
        } else if (argv[i] == std::string("--tree")) {
            tree = true;
        } else if (argv[i] == std::string("--reverse")) {
            reverse = true;
        } else {
            break;
        }
        ++i;
    }
    if (i == argc) {
        std::cerr << "usage: ./xml-tags [--tree] [--reverse] [--indent] PATH FILE ..." << std::endl;
        std::exit(1);
    }
    std::vector<std::string> path;
    parse_path(path, argv[i]);
    ++i;
    for (; i < argc; ++i) {
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
        if (tree || reverse) {
            StackWalker w(reverse);
            node.traverse(w);
        } else {
            TopWalker w(indent);
            node.traverse(w);
        }
    }
}
