cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(VitaTether)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "VitaTether")
set(VITA_TITLEID  "VTETHER01")
set(VITA_VERSION  "01.00")

add_subdirectory(module)

include_directories(
  module/include
)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/module)

add_executable(${PROJECT_NAME}
  client/src/main.c
)

add_dependencies(${PROJECT_NAME} "VitaTetherK_all")

target_link_libraries(${PROJECT_NAME}
  vita2d
  SceLibKernel_stub
  SceSysmodule_stub
  SceCtrl_stub
  SceTouch_stub
	m
  SceNet_stub
  SceShellSvc_stub
  SceVshBridge_stub
  SceSysmem_stub
  ScePgf_stub
	jpeg
  freetype
  c
  ScePower_stub
  SceCommonDialog_stub
  SceProcessmgr_stub
  png16
	SceGxm_stub
  SceDisplay_stub
  SceAppMgr_stub
  taihen_stub
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME} UNSAFE)

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys sce_sys
  FILE ${CMAKE_CURRENT_BINARY_DIR}/module/VitaTetherK.skprx module/VitaTetherK.skprx
)