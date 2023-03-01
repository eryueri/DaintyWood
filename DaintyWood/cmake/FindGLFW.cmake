if (NOT TARGET glfw)
	if (WIN32)
		set(GLFW_DIR "E:/Lib/glfw-3.3.8.bin.Win64")
		set(GLFW_LIB_DIR "${GLFW_DIR}/lib-vc2022")
		set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
		add_library(glfw STATIC IMPORTED GLOBAL)
		set_target_properties(glfw
			PROPERTIES
			IMPORTED_LOCATION "${GLFW_LIB_DIR}/glfw3.lib"
			INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
		)
	elseif (UNIX)
		find_package(glfw3 REQUIRED)
	endif()
endif()
