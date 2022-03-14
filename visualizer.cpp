//
// Created by Hannes Furmans on 14.03.22.
//

#include "visualizer.h"

std::string visualize(const std::shared_ptr<GateIndex> &index) {
    std::string ret;
    ret += "digraph {\n";
    for (const auto& pair : *index) {
        auto out_name = pair.first;
        for (const auto& in_ass : pair.second->attached_outputs) {
            auto in_gate = in_ass.gate;
            ret += "\t" + out_name + " -> " + in_gate->name + "\n";
        }
    }
    ret += "}";
    return ret;
}
