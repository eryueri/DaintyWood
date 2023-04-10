#pragma once

#define CHECK_NULL(expr) \
    if ((!expr)) \
    { \
        throw std::runtime_error(#expr " is null pointer..."); \
    }

#define MAX_FRAMES_IN_FLIGHT static_cast<uint32_t>(2)
