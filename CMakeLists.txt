cmake_minimum_required(VERSION 3.30)

project(postmachine VERSION 0.3.6 LANGUAGES CXX)

# C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QT
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)
qt_standard_project_setup()

# Files
file(GLOB PROJECT_SOURCES
     src/*.hpp
     src/*.cpp
     src/*.ui
)
include_directories(src)

# Resources
if (WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/data/info.rc")
endif ()

# App
qt_add_executable(${CMAKE_PROJECT_NAME}
                ${PROJECT_SOURCES}
                ${APP_ICON_RESOURCE_WINDOWS}
                data/recources.qrc)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)

# macOS Bundle
if(APPLE)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Post Machine"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.adidvar.postmachine"
        MACOSX_BUNDLE_BUNDLE_IDENTIFIER "com.adidvar.postmachine"
    )
endif()

# Translation
file(GLOB TRANSLATIONS_FILES
     locales/*.ts
)

qt_add_translations(${CMAKE_PROJECT_NAME} TS_FILES ${TRANSLATIONS_FILES} QM_FILES_OUTPUT_VARIABLE qm_files)

add_dependencies(${CMAKE_PROJECT_NAME} postmachine_lupdate)
add_dependencies(${CMAKE_PROJECT_NAME} postmachine_lrelease)

# Deploy
include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(LINUX)
    install(FILES ${qm_files} DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/locales)
    install(FILES ${CMAKE_SOURCE_DIR}/data/postmachine.png DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps)
    install(FILES ${CMAKE_SOURCE_DIR}/data/PostMachine.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
endif()

if(WIN32)
    install(FILES ${qm_files} DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/locales)
endif()

if (APPLE OR WIN32)
    qt_generate_deploy_app_script(
    TARGET ${CMAKE_PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
    install(SCRIPT ${deploy_script})
endif ()

# CPack
set(CPACK_PACKAGE_VENDOR "adidvar")
set(CPACK_PACKAGE_NAME "post-machine")
set(CPACK_PACKAGE_CONTACT "adidvar@gmail.com")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/package")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Post machine - post machine emulator")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}-v${CPACK_PACKAGE_VERSION}")

# --- macOS DMG ---
if(APPLE)
    set(CPACK_GENERATOR "Bundle")
    set(CPACK_PACKAGE_NAME "PostMachine")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_BUNDLE_NAME "PostMachine")
    set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/data/postmachine.icns")
    set(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/data/Info.plist")

endif()

# --- Debian (DEB) ---
if(UNIX AND NOT APPLE)
    list(APPEND CPACK_GENERATOR "DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "adidvar")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt-base (>= 6.0.0)")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

    set(CPACK_RPM_PACKAGE_LICENSE "GPL-3")
    set(CPACK_RPM_PACKAGE_SUMMARY "Post Machine")
    set(CPACK_RPM_PACKAGE_DESCRIPTION "Post machine emulator")
    set(CPACK_RPM_PACKAGE_REQUIRES "qt >= 6.0.0")

endif()

# --- Windows Installer (NSIS) ---
if(WIN32)
    list(APPEND CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "PostMachine")
    set(CPACK_NSIS_PACKAGE_NAME "PostMachine Installer")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/data/postmachine.ico")
    set(CPACK_NSIS_CONTACT "adidvar@gmail.com")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set (CPACK_CREATE_DESKTOP_LINKS "${CMAKE_PROJECT_NAME}")
    set (CPACK_PACKAGE_EXECUTABLES "${CMAKE_PROJECT_NAME}" "Post Machine" )
    set (CPACK_RESOURCE_FILE_LICENSE
        "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

endif()

include(CPack)
