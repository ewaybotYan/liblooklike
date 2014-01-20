find_file(
    PLANTUML_PATH
    "plantuml.jar"
    PATHS ${PLANTUML_PATH} "/opt/plantuml/" ${CMAKE_MODULE_PATH}
    DOC "plantuml.jar file path"
)

if( ${PLANTUML_PATH} STREQUAL "PLANTUML_PATH-NOTFOUND" )
    set( PLANTUML_PATH_FOUND FALSE )
    message("-- PlantUML not found")
else()
    set( PLANTUML_PATH_FOUND TRUE )
    message("-- Found plantUML")
endif()
