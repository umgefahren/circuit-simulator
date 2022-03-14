//
// Created by Hannes Furmans on 14.03.22.
//

#ifndef CIRCUIT_SIMULATOR_VISUALIZER_H
#define CIRCUIT_SIMULATOR_VISUALIZER_H

#include <string>
#include <memory>

#include "parser.h"

std::string visualize(const std::shared_ptr<GateIndex>& index);

#endif //CIRCUIT_SIMULATOR_VISUALIZER_H
