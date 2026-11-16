//
// Created by mrble on 09/10/2026.
//

#include "CommandParser.h"
#include "cli/data/ParsedCommand.h"
#include "cli/data/FlagRegister.h"

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace weave::cli {
    CommandParser::CommandParser(events::EventQueue& eventQueue) : _eventQueue(eventQueue) {}

    void CommandParser::parse(const std::vector<std::string> &tokens) {
        _parsedCommand.invocationPath = std::filesystem::current_path();
        _parsedCommand.programPath = std::filesystem::canonical(tokens[0]);

        if (tokens.size() == 1)
        {
            _eventQueue.pushEvent
            (
                events::EventType::Error,
                events::EventSource::Parser,
                "Missing command.\n"
            );
            _parsedCommand.command = "N/A";
            return;
        }

        // First item is always the name of COMMAND.
        _parsedCommand.command = tokens.at(1);

        for (int i = 2; i < tokens.size(); i++)
        {
            if (const std::string& item = tokens.at(i); item.starts_with('-')) {
                // Flags
                if (!_flagRegister.contains(item)) {

                    _eventQueue.pushEvent
                    (
                      events::EventType::Error,
                      events::EventSource::Parser,
                      "Invalid flag : \"" + item + "\".\n"
                    );
                    return;
                }

                _parsedCommand.flags[item] = "";
                if (_flagRegister[item] == data::FlagArgumentType::None) {}

                else if (_flagRegister[item] == data::FlagArgumentType::Verbose) {
                    _verbose = true;
                }
                else if (_flagRegister[item] == data::FlagArgumentType::Debug) {
                    _debug = true;
                }

                else if (_flagRegister[item] == data::FlagArgumentType::Required) {
                    try
                    {
                        i++;
                        _parsedCommand.flags[item] = tokens.at(i);
                    } catch (const std::out_of_range& e)
                    {
                        _eventQueue.pushEvent
                        (
                            events::EventType::Error,
                            events::EventSource::Parser,
                            "Flag : \"" + item + "\" is missing required 1 argument.\n"
                        );
                        return;
                    }
                }
            } else {
                // Arguments
                _parsedCommand.arguments.push_back(item);
            }
        }
        _parsedCommand.validFlag = true;
    }

    const data::parsedCommand& CommandParser::getParsedCommand() const
    {
        return _parsedCommand;
    }

    const std::string CommandParser::toString() const
    {
        std::ostringstream out;

        out << "ParsedCommand data ->\n";
        out << "\tinvoke\t| " << _parsedCommand.invocationPath << "\n";
        out << "\tprogram\t| " << _parsedCommand.programPath << "\n";
        out << "\t" << std::string(12, '-') << "\t\n";
        out << "\tcommand\t| " << _parsedCommand.command << "\n";
        out << "\tvalid\t| " << _parsedCommand.validFlag << "\n";

        int argCount = 1;
        for (const auto& arg : _parsedCommand.arguments) {
            out << "\targ (" << argCount << ")\t| " << arg << "\n";
            argCount++;
        }

        int flagCount = 1;
        for (const auto& [flag, arg] : _parsedCommand.flags) {
            out << "\tflg (" << flagCount << ")\t| " << flag << " -> " << arg << "\n";
            flagCount++;
        }

        return out.str();
    }

    bool CommandParser::hasVerbose() const {
        return _verbose;
    }

    bool CommandParser::hasDebug() const {
        return _debug;
    }
} // cli