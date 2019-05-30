################################################################################
# Check packages
################################################################################

include(FindPkgConfig)

pkg_check_modules(CONFIG REQUIRED libconfig++)

pkg_check_modules(ILM_CTRL ilmControl)
if(NOT ILM_CTRL_FOUND)
	set(ILM_CTRL_LIBRARIES ilmControl)
endif()

pkg_check_modules(ILM_COMMON ilmCommon)
if(NOT ILM_COMMON_FOUND)
	set(ILM_COMMON_LIBRARIES ilmCommon)
endif()

pkg_check_modules(DBUS_CXX REQUIRED dbus-cxx-1.0)

################################################################################
# CFLAGS
################################################################################

################################################################################
# Includes
################################################################################

include_directories(
	.
	${CONFIG_INCLUDE_DIRS}
	${DBUS_CXX_INCLUDE_DIRS}
	${ILM_CTRL_INCLUDE_DIRS}
	${ILM_COMMON_INCLUDE_DIRS}
)

message(STATUS "DBUS_CXX_INCLUDE_DIRS              = ${DBUS_CXX_INCLUDE_DIRS}")

################################################################################
# Sources
################################################################################

set(SOURCES
	main.cpp
	ActionManager.cpp
	Config.cpp
	DBusControlAdapter.cpp
	DBusServer.cpp
	Display.cpp
	DisplayManager.cpp
	EventHandler.cpp
	IlmObject.cpp
	Layer.cpp
	ObjectManager.cpp
	Surface.cpp
	Utils.cpp
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
	${DBUS_CXX_LIBRARIES}
	${ILM_CTRL_LIBRARIES}
	${ILM_COMMON_LIBRARIES}
)
