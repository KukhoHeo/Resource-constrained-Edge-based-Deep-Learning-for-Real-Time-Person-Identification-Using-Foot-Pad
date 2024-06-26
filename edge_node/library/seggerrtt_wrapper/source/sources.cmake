if(INCLUDE__SEGGERRTT)

get_filename_component(_tmp_source_dir "${SEGGERRTT__BASE_DIR}/" ABSOLUTE)

include_directories(${_tmp_source_dir}/RTT)

set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/RTT/SEGGER_RTT.c)
set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/RTT/SEGGER_RTT_printf.c)
set(PROJECT_SOURCES ${PROJECT_SOURCES} ${_tmp_source_dir}/RTT/SEGGER_RTT_ASM_ARMv7M.S)

set(PROJECT_SOURCES ${PROJECT_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/ubinos/bsp/dtty.c)

endif(INCLUDE__SEGGERRTT)


