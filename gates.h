//
// Created by Hannes Furmans on 14.03.22.
//

#ifndef CIRCUIT_SIMULATOR_GATES_H
#define CIRCUIT_SIMULATOR_GATES_H

#include <utility>
#include <vector>
#include <memory>
#include <optional>
#include <list>
#include <string>
#include <unordered_map>

class OutputAssociation;

class BasicGate {
public:
    std::string name;
    std::vector<OutputAssociation> attached_outputs;
    std::optional<bool> input_0 = std::nullopt;
    std::optional<bool> input_1 = std::nullopt;
    explicit BasicGate(std::string name) {
        this->name = std::move(name);
    };
    ~BasicGate() = default;
    virtual bool computable() = 0;
    virtual bool compute() = 0;

    void attach_output(std::shared_ptr<BasicGate> gate, unsigned int input);
    void compute_recursive();
};

class InputIndex {
private:
    std::unordered_map<std::string, std::shared_ptr<BasicGate>> internal;
public:
    InputIndex() = default;
    ~InputIndex() = default;
    void insert(const std::shared_ptr<BasicGate>& gate) {
        this->internal.insert({gate->name, gate});
    };
    std::shared_ptr<BasicGate> get(const std::string& name) {
        auto result= this->internal.find(name);
        return result->second;
    };
    std::string to_string() {
        std::string ret;
        for (const auto& pair: this->internal) { ret += ", " + pair.first; }
        return ret;
    }
};

class OutputIndex {
protected:
    std::unordered_map<std::string, std::shared_ptr<BasicGate>> internal;
public:
    OutputIndex() = default;
    ~OutputIndex() = default;
    void insert(const std::shared_ptr<BasicGate>& gate) {
        this->internal.insert({gate->name, gate});
    };
    std::shared_ptr<BasicGate> get(const std::string& name) {
        auto result= this->internal.find(name);
        return result->second;
    };
    std::string to_string() {
        std::string ret;
        for (const auto& pair: this->internal) { ret += ", " + pair.first; }
        return ret;
    }
};

class AndGate : public BasicGate {
public:
    explicit AndGate(std::string name) : BasicGate(std::move(name)) {};
    ~AndGate() = default;
    bool computable() override;
    bool compute() override;
};

class OrGate : public BasicGate {
public:
    explicit OrGate(std::string name) : BasicGate(std::move(name)) {};
    ~OrGate() = default;

    bool computable() override;
    bool compute() override;
};

class NotGate : public BasicGate {
public:
    explicit NotGate(std::string name) : BasicGate(std::move(name)) {};
    ~NotGate() = default;

    bool computable() override;
    bool compute() override;
};

class InputGate : public BasicGate {
public:
    explicit InputGate(std::string name) : BasicGate(std::move(name)) {};
    ~InputGate() = default;

    bool computable() override;
    bool compute() override;

    void set(bool value) {
        this->input_0 = std::optional{value};
    };
};

class OutputGate : public BasicGate {
public:
    explicit OutputGate(std::string name) : BasicGate(std::move(name)) {};
    ~OutputGate() = default;

    bool computable() override {
        return false;
    };
    bool compute() override {
        return false;
    };

    bool get() {
        return this->input_0.value();
    }
};

class OutputAssociation {
public:
    std::shared_ptr<BasicGate> gate;
    unsigned char input;
    OutputAssociation(std::shared_ptr<BasicGate> gate, unsigned char input) {
        this->gate = std::move(gate);
        this->input = input;
    }
    ~OutputAssociation() = default;
};

#endif //CIRCUIT_SIMULATOR_GATES_H
