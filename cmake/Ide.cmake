#---------------------------------------------------------------------------------------
# IDE support for headers
#---------------------------------------------------------------------------------------

set(SMARTCITY_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/../include")

#---------------------------------------------------------------------------------------
# SmartCity.Domain headers
#---------------------------------------------------------------------------------------

file(GLOB SMARTCITY_CORE_HEADERS "${SMARTCITY_HEADERS_DIR}/SmartCity.Core/" "${SMARTCITY_HEADERS_DIR}/SmartCity.Core/Renderer")
set(SMARTCITY_CORE_ALL_HEADERS ${SMARTCITY_CORE_HEADERS})


#---------------------------------------------------------------------------------------
# SmartCity.Client headers
#---------------------------------------------------------------------------------------

file(GLOB SMARTCITY_CLIENT_HEADERS "${SMARTCITY_HEADERS_DIR}/SmartCity.Client/")
set(SMARTCITY_CLIENT_ALL_HEADERS ${SMARTCITY_CLIENT_HEADERS})

set(SMARTCITY_ALL_HEADERS   ${SMARTCITY_DOMAIN_CORE_HEADERS}
                            ${SMARTCITY_CLIENT_HEADERS})