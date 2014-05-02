string(REPLACE ":" " " PATH $ENV{PATH})

find_file(
    DOT_EXECUTABLE_PATH
    "dot"
    PATHS ${PATH}
    DOC "dot executable path"
)

if( ${DOT_EXECUTABLE_PATH} STREQUAL "DOT_EXECUTABLE_PATH-NOTFOUND" )
    set( DOT_EXECUTABLE_PATH_FOUND FALSE )
    message("-- Dot not found")
else()
    set( DOT_EXECUTABLE_PATH_FOUND TRUE )
    execute_process(COMMAND "dot -V"
                OUTPUT_VARIABLE DOT_VERSION)
    message(STATUS "-- Found dot (${DOT_VERSION})")
endif()
