#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct NodeInfo
{
    std::string text;
    unsigned text_pos, text_end; // half-open range
};

typedef std::vector<NodeInfo> LevelInfo;
std::string node_text(indice const keys[], unsigned key_count, unsigned page);

void before_traversal();
void visit(page const *node, unsigned level = 0, unsigned child_index = 0);
void after_traversal();
void print(page const *root);
void print_blanks(unsigned n);