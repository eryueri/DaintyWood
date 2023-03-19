#pragma once

#define CHECK_NULL(expr) \
    if ((!expr)) \
    { \
        throw std::runtime_error(#expr " is null pointer..."); \
    }

