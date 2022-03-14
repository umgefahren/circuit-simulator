#include <iostream>
#include <memory>
#include "parser.h"
#include "visualizer.h"
#include <fstream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto input_index = std::make_shared<InputIndex>();
    auto output_index = std::make_shared<OutputIndex>();
    auto total_index = std::make_shared<GateIndex>();
    parse_file("../sample.txt", input_index, output_index, total_index);
    auto input_gate_0 = total_index->find("AAA")->second;
    auto input_gate_1 = total_index->find("BBB")->second;
    auto output_gate = total_index->find("ZZZ")->second;
    input_gate_0->input_0 = std::optional{false};
    input_gate_1->input_0 = std::optional{false};
    input_gate_0->compute_recursive();
    input_gate_1->compute_recursive();
    auto out_value = output_gate->input_0;
    std::cout << "Out Value => ";
    printf("%d\n", out_value.value());
    std::ofstream graphviz_file;
    graphviz_file.open("graph.dot");
    graphviz_file << visualize(total_index);
    graphviz_file.close();
    return 0;
}
