PROJECT(softvector)

SET(TARGET_PLATFORM_NAME riscv)
SET(TARGET_NAME etiss)
SET(ETISS_PREFIX ${CMAKE_SOURCE_DIR})
SET(INCLUDE_TESTS 0)
#ADD_SUBDIRECTORY(extern)

SET(TARGET_ARCH "RV64IMACV" CACHE STRING "No Target specified. Assuming softvector for etiss ArchImpl RV64IMACV.")
set(PLACE_HOLD_VCPUARCH ${TARGET_ARCH})	
CONFIGURE_FILE(extern/src/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}/softvector.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/extern/src/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}/softvector.cpp @ONLY)

ADD_LIBRARY(${PROJECT_NAME} STATIC
	extern/src/vpu/softvector-element.cpp
	extern/src/vpu/softvector-vector.cpp
	extern/src/vpu/softvector-register.cpp
	extern/src/target/${TARGET_PLATFORM_NAME}/base/base.cpp
	extern/src/target/${TARGET_PLATFORM_NAME}/lsu/lsu.cpp
	extern/src/target/${TARGET_PLATFORM_NAME}/arithmetic/integer.cpp
	${CMAKE_CURRENT_BINARY_DIR}/extern/src/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}/softvector.cpp
)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
   extern/include
   extern/include/target/${TARGET_PLATFORM_NAME}
   extern/include/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}
   ${ETISS_PREFIX}/include
   ${ETISS_PREFIX}/include_c
   ${ETISS_PREFIX}/ArchImpl/${TARGET_ARCH}
)


INSTALL(FILES
    extern/include/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}/softvector.h DESTINATION include/jit/etiss/jit/vpu
)
INSTALL(FILES
    extern/include/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}/softvector.h DESTINATION include/etiss/jit/vpu
)
InstallJitLib(softvector)
