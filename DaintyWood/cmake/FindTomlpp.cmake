if (NOT TARGET tomlplusplus::tomlplusplus)
    if (WIN32)

    elseif (UNIX)
        find_package(tomlplusplus REQUIRED)
    endif ()

endif ()
