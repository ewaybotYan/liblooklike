# part of this file is taken from http://mementocodex.wordpress.com/2013/01/19/how-to-generate-code-documentation-with-doxygen-and-cmake-a-slightly-improved-approach/

if(BUILD_DOCUMENTATION)

    # create output directory because Doxygen won't do it
    file(MAKE_DIRECTORY ${DOC_OUTPUT_PATH})

    FIND_PACKAGE(Doxygen)
    IF(NOT DOXYGEN_FOUND)
        MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
    ENDIF()

    find_package(Dot)
    if(DOT_EXECUTABLE_PATH_FOUND)
        set( HAVE_DOT "YES" ) # for doxyfile
    else()
        set( HAVE_DOT "NO" ) # for doxyfile
    endif()

    if(USE_PLANTUML)
        find_package(Java COMPONENTS Runtime)
        if( NOT Java_Runtime_FOUND )
            MESSAGE(FATAL_ERROR "Java is needed to run PLANTUML")
        else()
            find_package(PlantUml)
        endif()
    endif()

    # doxygen doc target
    SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in     )
    SET( doxyfile             ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile        )
    SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html )
    SET( doxy_output_root     ${DOC_OUTPUT_PATH}                          )
    SET( doxy_input           "${CMAKE_CURRENT_SOURCE_DIR}/../include"    )
    set( doxy_input           "${doxy_input} ${CMAKE_CURRENT_SOURCE_DIR}/../examples" )
    set( doxy_input           "${doxy_input} ${CMAKE_CURRENT_SOURCE_DIR}" )
    set( examples             "${CMAKE_CURRENT_SOURCE_DIR}/../examples" )
    set( doxygen_log          "${CMAKE_CURRENT_BINARY_DIR}/doxygen.log"   )

    ADD_CUSTOM_COMMAND( OUTPUT ${doxy_html_index_file}
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
    COMMENT "Generating project code documentation")

    ADD_CUSTOM_TARGET( doc
                       ALL
                       DEPENDS ${doxy_html_index_file}
                               ${plantuml_images} )

    #optional plantuml target
    if( USE_PLANTUML AND PLANTUML_PATH_FOUND )
        set( image_dir "${CMAKE_CURRENT_BINARY_DIR}/uml_images")
        file(MAKE_DIRECTORY ${image_dir})
        ADD_CUSTOM_TARGET( plantuml
        COMMAND ${Java_JAVA_EXECUTABLE} -jar ${PLANTUML_PATH} -o "${image_dir}" "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h"
        COMMENT "Generating UML diagrams for documentation")
        add_dependencies( doc plantuml)
    endif()

    # set doxygen paths in Doxyfile
    CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )

endif(BUILD_DOCUMENTATION)
