if (NOT TARGET spdlog)
	if(WIN32)
		set(SPDLOG_DIR "E:/Lib/spdlog")
		set(SPDLOG_LIB_DIR "${SPDLOG_DIR}/build/Release")
		set(SPDLOG_INCLUDE_DIR "${SPDLOG_DIR}/include")
		add_library(spdlog STATIC IMPORTED GLOBAL)
		set_target_properties(spdlog
			PROPERTIES
			IMPORTED_LOCATION "${SPDLOG_LIB_DIR}/spdlog.lib"
			INTERFACE_INCLUDE_DIRECTORIES "${SPDLOG_INCLUDE_DIR}"
		)
	elseif(UNIX)
		find_package(spdlog REQUIRED)
	endif()
endif()
