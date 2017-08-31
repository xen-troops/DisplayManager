################################################################################
# Check packages
################################################################################

include(FindPkgConfig)

pkg_check_modules (CONFIG REQUIRED libconfig++)
if (!CONFIG_FOUND)
	message(FATAL_ERROR "No libconfig++ found")
endif()

pkg_check_modules (ILM_CTRL REQUIRED ilmControl)
if(!ILM_CTRL_FOUND)
	message(FATAL_ERROR "No ilmControl found")
endif()

pkg_check_modules (ILM_COMMON REQUIRED ilmCommon)
if(!ILM_COMMON_FOUND)
	message(FATAL_ERROR "No ilmCommon found")
endif()

################################################################################
# Includes
################################################################################

include_directories(
	.
	${CONFIG_INCLUDE_PATH}
)

################################################################################
# Sources
################################################################################

set(SOURCES
	main.cpp
	ActionManager.cpp
	Config.cpp
	Display.cpp
	DisplayManager.cpp
	EventHandler.cpp
	IlmObject.cpp
	Layer.cpp
	ObjectManager.cpp
	Surface.cpp
)

################################################################################
# Targets
################################################################################

add_executable(${PROJECT_NAME} ${SOURCES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

################################################################################
# Libraries
################################################################################

target_link_libraries(${PROJECT_NAME}
	pthread
	${CONFIG_LIBRARIES}
	${ILM_CTRL_LIBRARIES}
)
