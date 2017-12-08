################################################################################
# Check packages
################################################################################

include(FindPkgConfig)

pkg_check_modules(CONFIG REQUIRED libconfig++)

pkg_check_modules(ILM_CTRL ilmControl)
if(NOT ILM_CTRL_FOUND)
	set(ILM_CTRL_LIBRARIES "ilmControl;ilmCommon")
endif()

pkg_check_modules(ILM_COMMON ilmCommon)
if(NOT ILM_COMMON_FOUND)
	set(ILM_COMMON_LIBRARIES ilmCommon)
endif()

pkg_check_modules(DBUS1 REQUIRED dbus-1)

pkg_check_modules(DBUS_CPP REQUIRED dbus-cpp)

pkg_check_modules(PROPERTIES_CPP REQUIRED properties-cpp)

################################################################################
# CFLAGS
################################################################################

string(REPLACE ";" " " DBUS1_CFLAGS_STR "${DBUS1_CFLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DBUS1_CFLAGS_STR}")

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
	${DBUS_CPP_LIBRARIES}
	${ILM_CTRL_LIBRARIES}
)
