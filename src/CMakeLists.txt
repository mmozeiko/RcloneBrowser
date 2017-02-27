if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /wd4100 /wd4189")
else()
  add_definitions("-pedantic -Wall -Wextra -Werror -std=c++11")
endif()

project(rclone-browser)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(UI
  main_window.ui
  remote_widget.ui
  transfer_dialog.ui
  export_dialog.ui
  progress_dialog.ui
  job_widget.ui
  mount_widget.ui
  stream_widget.ui
  preferences_dialog.ui
)

set(MOC
  main_window.h
  remote_widget.h
  transfer_dialog.h
  export_dialog.h
  progress_dialog.h
  job_widget.h
  mount_widget.h
  stream_widget.h
  preferences_dialog.h
  icon_cache.h
  item_model.h
)

set(OTHER
  pch.h
  utils.h
)

set(SOURCE
  pch.cpp
  main.cpp
  main_window.cpp
  remote_widget.cpp
  transfer_dialog.cpp
  export_dialog.cpp
  progress_dialog.cpp
  job_widget.cpp
  mount_widget.cpp
  stream_widget.cpp
  preferences_dialog.cpp
  icon_cache.cpp
  item_model.cpp
  utils.cpp
)

if(WIN32)
  set(OTHER ${OTHER} resources.rc)
elseif(APPLE)
  set(OTHER ${OTHER} osx_helper.h)
  set(SOURCE ${SOURCE} osx_helper.mm)
endif()

set(QRC resources.qrc)

add_definitions(-DRCLONE_BROWSER_VERSION="${RCLONE_BROWSER_VERSION}")

qt5_wrap_ui(UI_OUT ${UI})
qt5_wrap_cpp(MOC_OUT ${MOC})
qt5_add_resources(QRC_OUT ${QRC} OPTIONS "-no-compress")

source_group("" FILES ${SOURCE} ${MOC} ${UI} ${QRC} ${OTHER})
source_group("Generated" FILES ${MOC_OUT} ${UI_OUT} ${MOC_OUT} ${QRC_OUT})

use_pch(pch.h pch.cpp "${SOURCE}")
use_pch(pch.h pch.cpp "${MOC_OUT}")

if(WIN32)
  add_executable(RcloneBrowser WIN32 ${SOURCE} ${BACKEND} ${OTHER} ${MOC} ${MOC_OUT} ${UI_OUT} ${MOC_OUT} ${QRC_OUT})
  target_link_libraries(RcloneBrowser Qt5::Widgets Qt5::WinExtras)
elseif(APPLE)
  set_source_files_properties(icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  add_executable(rclone-browser MACOSX_BUNDLE ${SOURCE} ${BACKEND} ${OTHER} ${MOC} ${MOC_OUT} ${UI_OUT} ${MOC_OUT} ${QRC_OUT} icon.icns)
  target_link_libraries(rclone-browser Qt5::Widgets Qt5::MacExtras ${COCOA_LIB})
  set_target_properties(rclone-browser PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")

  add_custom_command(TARGET rclone-browser POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_directory "src/icon.icns" "$<TARGET_FILE_DIR:rclone-browser>/../Resources/")
else()
  add_executable(rclone-browser ${SOURCE} ${BACKEND} ${OTHER} ${MOC} ${MOC_OUT} ${UI_OUT} ${MOC_OUT} ${QRC_OUT})
  target_link_libraries(rclone-browser Qt5::Widgets)

  install(TARGETS rclone-browser RUNTIME DESTINATION bin)
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/icon.png" DESTINATION "share/pixmaps" RENAME "rclone-browser.png")
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/rclone-browser.desktop" DESTINATION "share/applications")
endif()
