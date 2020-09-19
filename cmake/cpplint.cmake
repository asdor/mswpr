function(find_directories OUT SRC)
    file(GLOB children RELATIVE ${SRC} ${SRC}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${SRC}/${child} AND NOT ${child} MATCHES "build|cmake|git")
            find_directories(dirlist ${SRC}/${child})
            list(APPEND dirlist "${SRC}/${child}")
        endif()
    endforeach()
    set(${OUT} "${dirlist}" PARENT_SCOPE)
endfunction()

function(find_includes OUT SRC)
    file(STRINGS ${SRC} _included_files REGEX "#[ ]*include")
    foreach(_include ${_included_files})
        string(REGEX MATCH "(\")([^\"]+)(\")" DUMMY ${_include})
        set(_inc ${CMAKE_MATCH_2})
        foreach(_path ${ARGN})
            set(_full_path "${_path}/${_inc}")
            if(EXISTS ${_full_path} AND NOT IS_DIRECTORY ${_full_path})
                find_includes(_lst ${_full_path} ${ARGN})
                break()
            endif()
        endforeach()
    endforeach()
    list(APPEND _lst "${SRC}")
    set(${OUT} "${_lst}" PARENT_SCOPE)
endfunction()

math(EXPR LAST "${CMAKE_ARGC} - 1")
set(SRC ${CMAKE_ARGV${LAST}})
find_directories(DIRS ${SEARCH_PATHS})

if(INPLACE)
    set(FLAG -i)
else()
    set(FLAG -output-replacements-xml)
endif()

find_includes(FILES_TO_CHECK ${SRC} ${DIRS})

foreach(_file ${SRC} ${FILES_TO_CHECK})
    execute_process(COMMAND ${EXE} ${FLAG} ${_file} -style=file WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE _output RESULT_VARIABLE _result)
    if(_result OR _output MATCHES "<replacement ")
        message(FATAL_ERROR "Failed for ${_file} [${_result}]\n${_output}")
    endif()
endforeach()




# function(find_includes OUT SRC)
#     file(STRINGS ${SRC} _included_files REGEX "#[ ]*include")
#     foreach(_include ${_included_files})
#         string(REGEX MATCH "[<\"]([^>\"]+)[>\"]" DUMMY ${_include})
#         set(_inc ${CMAKE_MATCH_1})
#         foreach(_path ${ARGN})
#             set(_full_path "${_path}/${_inc}")
#             if(EXISTS ${_full_path})
#                 find_includes(_lst ${_full_path} ${ARGN})
#                 break()
#             endif()
#         endforeach()
#     endforeach()
#     list(APPEND _lst "${SRC}")
#     set(${OUT} "${_lst}" PARENT_SCOPE)
# endfunction()

# math(EXPR CPPLINT_ARGC "${CMAKE_ARGC} - 5")
# math(EXPR _last_argc "${CMAKE_ARGC} - 1")
# foreach(_idx RANGE 5 ${_last_argc})
#     list(APPEND CPPLINT_ARGV "${CMAKE_ARGV${_idx}}")
# endforeach()

# # Вызов компилятора.
# execute_process(COMMAND ${CPPLINT_ARGV} RESULT_VARIABLE _ret ENCODING AUTO)
# if(NOT _ret EQUAL 0)
#     message(FATAL_ERROR "Compiler failed with retcode ${_ret}")
# endif()

# # Получение имя компилируемого исходного файла.
# cmake_parse_arguments(EA "" "-c" "" ${CPPLINT_ARGV})
# set(SRC "${EA_-c}")
# get_filename_component(INCS "${SRC}" DIRECTORY)

# # Получение директорий с заголовочными файлами.
# math(EXPR _last_index "${CPPLINT_ARGC} - 1")
# foreach(_idx RANGE ${_last_index})
#     list(GET CPPLINT_ARGV ${_idx} _inc_flag)
#     if(_inc_flag STREQUAL "-I") # -I /path/to/dir
#         math(EXPR _next "${_idx} + 1")
#         list(GET CPPLINT_ARGV ${_next} _inc)
#     elseif(_inc_flag MATCHES "^-I") # -I/path/to/dir
#         string(SUBSTRING "${_inc_flag}" 2 -1 _inc)
#     else()
#         continue()
#     endif()

#     list(APPEND INCS "${_inc}")
# endforeach()

# find_includes(FILES_TO_CHECK ${SRC} ${INCS})

# if(DEFINED ENV{VERBOSE})
#     message(STATUS "CPPLINT: ${FILES_TO_CHECK}")
# endif()

# if(INPLACE)
#     set(FLAG -i)
# else()
#     set(FLAG -output-replacements-xml)
# endif()

# foreach(_file ${FILES_TO_CHECK})
#     execute_process(COMMAND ${EXE} ${FLAG} ${_file} -style=file WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE _output RESULT_VARIABLE _result)
#     if(NOT _result EQUAL 0 OR _output MATCHES "<replacement ")
#         message(FATAL_ERROR "CPPLINT failed for ${_file}\n${_output}")
#     endif()
# endforeach()
