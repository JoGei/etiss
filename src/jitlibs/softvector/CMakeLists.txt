PROJECT(softvector)

SET(TARGET_PLATFORM_NAME riscv)
SET(TARGET_NAME etiss)
SET(ETISS_PREFIX ${CMAKE_SOURCE_DIR})
SET(INCLUDE_TESTS 0)
#ADD_SUBDIRECTORY(extern)

SET(TARGET_ARCH "RV64GCV" CACHE STRING "No Target specified. Assuming softvector for etiss ArchImpl RV64GCV.")

include(FetchContent)
FetchContent_Declare(softvector_srcs
	GIT_REPOSITORY https://github.com/tum-ei-eda/softvector.git
	GIT_TAG master
)

if(NOT softvector_srcs_POPULATED)
	FetchContent_Populate(softvector_srcs)
	FetchContent_GetProperties(softvector_srcs)
	set(EXTERNAL_INSTALL_LOCATION ${softvector_srcs_SOURCE_DIR})
	INSTALL(FILES ${softvector_srcs_SOURCE_DIR}/include/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}/softvector.h DESTINATION include/jit/etiss/jit/vpu )
	INSTALL(FILES
    ${softvector_srcs_SOURCE_DIR}/include/target/${TARGET_PLATFORM_NAME}/${TARGET_NAME}/softvector.h DESTINATION include/etiss/jit/vpu )
	SET(SOFTVECTOR_BUILD_DOC OFF)
	ADD_SUBDIRECTORY(${softvector_srcs_SOURCE_DIR} ${softvector_srcs_BINARY_DIR})
	InstallJitLib(softvector)
endif()
