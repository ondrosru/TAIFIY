#pragma once
#include "AutomatHeader.h"
#include <string>

const std::string SaveInputMealyFile = "in_mealy";
const std::string SaveOutputMealyFile = "out_mealy";
const std::string SaveInputMooreFile = "in_moore";
const std::string SaveOutputMooreFile = "out_moore";

const SaveFormat MEALY_SAVE_FORMAT = SaveFormat::DOT;
const SaveFormat MOORE_SAVE_FORMAT = SaveFormat::DOT;

const std::string ERROR_WRONG_DATA = "Wrong data.";
const std::string ERROR_UNKNOWN_MACHINE = "Unknown machine.";
const std::string ERROR_FILE_NOT_FOUND = "This file was not found.";
const std::string ERROR_NOT_ENOUGH_ARGUMENTS = "The number of arguments does not match the task condition.\n"
											   "Input should look: MinimizationAutomat.exe <input file>";

const std::string SYMBOL_Z = "Z";
