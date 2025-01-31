cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(VitaTetherK)
include("${VITASDK}/share/vita.cmake" REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.c
)

target_link_libraries(${PROJECT_NAME}
  SceDebugForDriver_stub
  SceCtrlForDriver_stub
  SceCpuForDriver_stub
  SceKernelSuspendForDriver_stub
  SceModulemgrForDriver_stub
  SceSysclibForDriver_stub
  SceSysmemForDriver_stub
  SceSysconForDriver_stub
  SceThreadmgrForDriver_stub
  SceUdcdForDriver_stub
  SceBtForDriver_stub
  SceIofilemgrForDriver_stub 
  SceOledForDriver_stub_weak
  SceLcdForDriver_stub_weak
  SceSblAIMgrForDriver_stub
  taihenForKernel_stub
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME}
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(${PROJECT_NAME}.skprx ${PROJECT_NAME}
  CONFIG vitatetherk.yml
  UNSAFE
)

vita_create_stubs(stubs VitaTetherK ${CMAKE_CURRENT_SOURCE_DIR}/vitatetherk.yml KERNEL)

add_custom_target(VitaTetherK_all
  ALL
  DEPENDS VitaTetherK.skprx-self
  DEPENDS libVitaTetherK_stub.a
  DEPENDS libVitaTetherK_stub_weak.a
)