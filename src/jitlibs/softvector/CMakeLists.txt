PROJECT(softvector)

# TODO: Add softfloat lib as external project
ADD_LIBRARY(${PROJECT_NAME}
    rvv-hl.cpp
    rvv-hl-integer.cpp
    rvv-hl-base.cpp
    rvv-hl-lsu.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ETISS)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC 
    .
    ./include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../ArchImpl/RV64IMACV
)

INSTALL(FILES
    include/rvv-hl.h
    DESTINATION include/jit/etiss/jit/vpu
)
InstallJitLib(${PROJECT_NAME})
