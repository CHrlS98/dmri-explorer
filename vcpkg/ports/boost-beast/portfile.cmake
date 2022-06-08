# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/beast
    REF boost-1.79.0
    SHA512 d3f17b37fd503d9e65f0490832302d14318898a6b598864143fbd5310f69ade026499efe6947c66fd7309770ec63bb0dad1688cf3f750910426a058d53127e10
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})
