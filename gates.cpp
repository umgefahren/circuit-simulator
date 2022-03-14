//
// Created by Hannes Furmans on 14.03.22.
//

#include "gates.h"
#include <iostream>

bool two_input_computable(BasicGate * gate) {
    return gate->input_0.has_value() && gate->input_1.has_value();
}

void BasicGate::attach_output(std::shared_ptr<BasicGate> gate, unsigned int input) {
    OutputAssociation association = OutputAssociation(std::move(gate), input);
    this->attached_outputs.push_back(association);
}

void BasicGate::compute_recursive() {
    bool compute_result = this->compute();
    for (const OutputAssociation& association : this->attached_outputs) {
        auto gate = association.gate;
        switch (association.input) {
            case 0:
                gate->input_0 = std::optional<bool>{compute_result};
            case 1:
                gate->input_1 = std::optional<bool>{compute_result};
        }
    }
    for (const OutputAssociation& association : this->attached_outputs) {
        if (association.gate->computable()) {
            association.gate->compute_recursive();
        }
    }
}

bool AndGate::computable() {
    return two_input_computable(this);
};

bool AndGate::compute() {
    auto value_0 = this->input_0.value();
    auto value_1 = this->input_1.value();

    return value_0 && value_1;
};

bool OrGate::computable() {
    return two_input_computable(this);
};

bool OrGate::compute() {
    auto value_0 = this->input_0.value();
    auto value_1 = this->input_1.value();

    return value_0 || value_1;
};

bool NotGate::computable() {
    return this->input_0.has_value();
}

bool NotGate::compute() {
    return !this->input_0.value();
}

bool InputGate::computable() {
    return this->input_0.has_value();
}

bool InputGate::compute() {
    return this->input_0.value();
}
