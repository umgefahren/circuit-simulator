//
// Created by Hannes Furmans on 14.03.22.
//

#include "parser.h"
#include "gates.h"
#include <regex>
#include <iostream>
#include <fstream>
#include <memory>

static std::regex input_matcher() {
    static auto matcher = std::regex("%([a-zA-Z]+)=([I|O|&|\\||!])", std::regex::ECMAScript | std::regex::optimize);
    return matcher;
}

static std::regex relation_matcher() {
    static auto matcher = std::regex("([a-zA-Z]+)->([a-zA-Z]+)([0-1]+)", std::regex::ECMAScript | std::regex::optimize);
    return matcher;
}

ParserGate parse_declaration(const std::string& line, const std::shared_ptr<InputIndex>& input_index, const std::shared_ptr<OutputIndex>& output_index) {
    std::smatch matches;
    auto matcher = input_matcher();
    if (std::regex_search(line, matches, matcher)) {
        std::string name = matches[1].str();
        std::string type_string = matches[2].str();
        std::shared_ptr<BasicGate> pointer = nullptr;
        if (type_string == "I") {
            InputGate input_gate = InputGate(name);
            auto input_pointer = std::make_shared<InputGate>(input_gate);
            pointer = std::dynamic_pointer_cast<BasicGate>(input_pointer);
            input_index->insert(pointer);
        } else if (type_string == "O") {
            OutputGate output_gate = OutputGate(name);
            auto output_pointer = std::make_shared<OutputGate>(output_gate);
            pointer = std::dynamic_pointer_cast<BasicGate>(output_pointer);
            output_index->insert(pointer);
        } else if (type_string == "|") {
            OrGate or_gate = OrGate(name);
            auto or_pointer = std::make_shared<OrGate>(or_gate);
            pointer = std::dynamic_pointer_cast<BasicGate>(or_pointer);
        } else if (type_string == "&") {
            AndGate and_gate = AndGate(name);
            auto and_pointer = std::make_shared<AndGate>(and_gate);
            pointer = std::dynamic_pointer_cast<BasicGate>(and_pointer);
        } else if (type_string == "!") {
            NotGate not_gate = NotGate(name);
            auto not_pointer = std::make_shared<NotGate>(not_gate);
            pointer = std::dynamic_pointer_cast<BasicGate>(not_pointer);
        } else {
            std::cerr << "Unallowed type string during parsing" << std::endl;
            exit(1);
        }
        ParserGate parser_gate = ParserGate(name, pointer);
        return parser_gate;
    } else {
        std::cerr << "Error during reading of decleration => " << line  << std::endl;
        exit(1);
    }
};

void parse_file(const std::string& filename, const std::shared_ptr<InputIndex>& input_index, const std::shared_ptr<OutputIndex>& output_index, const std::shared_ptr<GateIndex>& total_index) {
    std::string line;
    std::ifstream ReadFile(filename);
    bool is_open = ReadFile.is_open();
    std::cout << "File is => " << is_open << std::endl;
    std::getline(ReadFile, line);
    std::cout << "Line => '" << line << "'" << std::endl;
    uint decleration_nums = std::stoi(line);
    std::getline(ReadFile, line);
    uint relations = std::stoi(line);
    for (auto i = 0; i < decleration_nums; i++) {
        bool readline_result = std::getline(ReadFile, line).good();
        if (!readline_result) {
            std::cerr << "Error during file reading (declarations)" << std::endl;
            exit(1);
        };
        auto parsed_gate = parse_declaration(line, input_index, output_index);
        total_index->insert({parsed_gate.name, parsed_gate.gate});
    }
    for (auto i = 0; i < relations; i++) {
        bool readline_result = std::getline(ReadFile, line).good();
        if (!readline_result) {
            std::cerr << "Error during file reading (relations)" << std::endl;
            exit(1);
        };
        parse_relation(line, total_index);
    }
    ReadFile.close();
}

void parse_relation(const std::string& line, const std::shared_ptr<GateIndex>& total_index) {
    std::smatch matches;
    auto matcher = relation_matcher();
    if (std::regex_search(line, matches, matcher)) {
        std::string out_name = matches[1].str();
        std::string in_name = matches[2].str();
        uint in_num = std::stoi(matches[3].str());
        std::shared_ptr<BasicGate> out_gate = total_index->find(out_name)->second;
        std::shared_ptr<BasicGate> in_gate = total_index->find(in_name)->second;
        out_gate->attach_output(std::move(in_gate), in_num);
    } else {
        std::cerr << "illegal expression encountered" << std::endl;
        exit(1);
    }
}
