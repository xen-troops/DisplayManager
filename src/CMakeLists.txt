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

pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-unix-2.0)

find_program(GDBUS_CODEGEN NAMES gdbus-codegen)
if (NOT GDBUS_CODEGEN)
    message(SEND_ERROR "Can't find gdbus-codegen")
endif()

################################################################################
# CFLAGS
################################################################################

################################################################################
# Includes
################################################################################

include_directories(
	.
	${CMAKE_CURRENT_BINARY_DIR}
	${CONFIG_INCLUDE_DIRS}
	${GLIB_INCLUDE_DIRS}
	${GIO_INCLUDE_DIRS}
	${ILM_CTRL_INCLUDE_DIRS}
	${ILM_COMMON_INCLUDE_DIRS}
)

message(STATUS "GIO_INCLUDE_DIRS              = ${GIO_INCLUDE_DIRS}")

################################################################################
# DBus stubs
################################################################################

add_custom_command(OUTPUT DBusControl.c
    COMMAND ${GDBUS_CODEGEN} --interface-prefix com.epam.DisplayManager. --generate-c-code=DBusControl ${PROJECT_SOURCE_DIR}/src/com.epam.displaymanager.xml
    DEPENDS ${PROJECT_SOURCE_DIR}/src/com.epam.displaymanager.xml
)

################################################################################
# Sources
################################################################################

set(SOURCES
	main.cpp
	ActionManager.cpp
	Config.cpp
	DBusControl.c
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
	${GLIB_LIBRARIES}
	${GIO_LIBRARIES}
	${ILM_CTRL_LIBRARIES}
	${ILM_COMMON_LIBRARIES}
)
