PROJECT(softvector)

# TODO: Add softfloat lib as external project
ADD_LIBRARY(${PROJECT_NAME}
    rvv-hl.cpp
    rvv-hl-arithmetic.cpp
    rvv-hl-base.cpp
    rvv-hl-lsu.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ETISS)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC 
    .
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ArchImpl/RV64IMACV
)

TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC
	ETISS_BUILD=1
	ETISSCPU_SPEC
)

INSTALL(FILES
    rvv-hl.h
    DESTINATION include/jit/etiss/jit/vpu
)
InstallJitLib(${PROJECT_NAME})
