file(GLOB COMMON_SOURCES
    "*.cpp"
    "datamgr/*.cpp"
    "datastore/*.cpp"
    "mempool/*.cpp"
    "objectalloc/*.cpp"
    "processor/*.cpp"
    "processor/win/*.cpp"
    "processor/mac/*.cpp"
    "processor/linux/*.cpp"
    "ref/*.cpp"
    "string/*.cpp"
    "thread/*.cpp"
    "time/*.cpp"
    "time/win/*.cpp"
    "time/mac/*.cpp"
    "time/linux/*.cpp"
    "xml/*.cpp"
)

add_library(common STATIC
    ${COMMON_SOURCES}
)

target_include_directories(common
    PUBLIC
        ${PROJECT_SOURCE_DIR}
)

target_link_libraries(common
    PUBLIC
        expat-2.0
        bc
        storm
        tempest
)
