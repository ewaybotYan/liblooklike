find_file(
    DOT_EXECUTABLE_PATH
    "dot"
    PATHS $ENV{PATH}
    DOC "dot executable path"
    NO_DEFAULT_PATH
)

if( ${DOT_EXECUTABLE_PATH} STREQUAL "DOT_EXECUTABLE_PATH-NOTFOUND" )
    set( DOT_EXECUTABLE_PATH_FOUND FALSE )
    message("--   dot not found")
else()
    set( DOT_EXECUTABLE_PATH_FOUND TRUE )
    execute_process(COMMAND "dot -V"
                OUTPUT_VARIABLE DOT_VERSION)
    message("--   found dot (${DOT_VERSION})")
endif()
