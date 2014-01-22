message("-- checking for plantUML")

find_file(
    PLANTUML_PATH
    "plantuml.jar"
    PATHS ${PLANTUML_PATH} "/opt/plantuml/"
    DOC "plantuml.jar file path"
    NO_DEFAULT_PATH
)

if( ${PLANTUML_PATH} STREQUAL "PLANTUML_PATH-NOTFOUND" )
    set( PLANTUML_PATH_FOUND FALSE )
    message("--   plantUML not found")
else()
    set( PLANTUML_PATH_FOUND TRUE )
    message("--   found plantUML")
endif()
