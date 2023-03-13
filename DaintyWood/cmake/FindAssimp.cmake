if (NOT TARGET assimp)
    if (WIN32)

    elseif (UNIX)
        find_package(assimp REQUIRED)
    endif ()

endif ()

