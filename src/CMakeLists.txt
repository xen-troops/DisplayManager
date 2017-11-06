################################################################################
# Check packages
################################################################################

include(FindPkgConfig)

pkg_check_modules(CONFIG REQUIRED libconfig++)
if (!CONFIG_FOUND)
	message(FATAL_ERROR "No libconfig++ found")
endif()

pkg_check_modules(ILM_CTRL REQUIRED ilmControl)
if(!ILM_CTRL_FOUND)
	message(FATAL_ERROR "No ilmControl found")
endif()

pkg_check_modules(ILM_COMMON REQUIRED ilmCommon)
if(!ILM_COMMON_FOUND)
	message(FATAL_ERROR "No ilmCommon found")
endif()

pkg_check_modules(DBUS1 REQUIRED dbus-1)
if(!DBUS1_FOUND)
	message(FATAL_ERROR "No dbus-1 found")
endif()

pkg_check_modules(DBUS_CPP REQUIRED dbus-cpp)
if(!DBUS_CPP_FOUND)
	message(FATAL_ERROR "No dbus-cpp found")
endif()

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
