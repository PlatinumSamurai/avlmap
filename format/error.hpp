// Copyright (c) 2024 PlatinumSamurai. All rights reserved.

#ifndef FORMAT_FORMAT_ERROR_HPP_
#define FORMAT_FORMAT_ERROR_HPP_

#include <source_location>
#include <string>
#include <iostream>
#include <exception>


struct Error {
    std::string message;
    std::source_location location;

    explicit Error(const std::string &msg,
        const std::source_location &loc = std::source_location::current()) :
                                                message(msg), location(loc) {}
};


std::ostream& operator<<(std::ostream &out, const Error &error) {
    out << "In file " << error.location.file_name() << "\n" <<
        error.location.function_name() << ", " << error.location.line() <<
        ":\n\t" << error.message << "\n";

    return out;
}


#endif  // FORMAT_FORMAT_ERROR_HPP_

