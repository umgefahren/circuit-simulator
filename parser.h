//
// Created by Hannes Furmans on 14.03.22.
//

#ifndef CIRCUIT_SIMULATOR_PARSER_H
#define CIRCUIT_SIMULATOR_PARSER_H

#include "gates.h"
#include <string>
#include <memory>

using GateIndex = std::unordered_map<std::string, std::shared_ptr<BasicGate>>;

void parse_file(const std::string& filename, const std::shared_ptr<InputIndex>& input_index, const std::shared_ptr<OutputIndex>& output_index, const std::shared_ptr<GateIndex>& total_index);

class ParserGate {
public:
    std::string name;
    std::shared_ptr<BasicGate> gate;
    ParserGate(std::string name, std::shared_ptr<BasicGate> gate) {
        this->name = std::move(name);
        this->gate = std::move(gate);
    };
    ~ParserGate() = default;
};

ParserGate parse_declaration(const std::string& line, const std::shared_ptr<InputIndex>& input_index, const std::shared_ptr<OutputIndex>& output_index);
void parse_relation(const std::string& line, const std::shared_ptr<GateIndex>& total_index);

#endif //CIRCUIT_SIMULATOR_PARSER_H
