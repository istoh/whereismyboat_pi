# ~~~
# Summary:      Plugin-specific build setup
# License:      GPLv3+
# ~~~

# This file is the plugin part of the OpenCPN plugin template
# (github.com/Rasbats/shipdriver_pi); the rest of the build files are the
# template's and should be updated from there.

set(OCPN_TEST_REPO
    "istoh/whereismyboat-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "istoh/whereismyboat-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "istoh/whereismyboat-prod"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

set(PKG_NAME WhereIsMyBoat_pi)
set(PKG_VERSION 0.1.0) # Major.Minor.Patch
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME WhereIsMyBoat)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME WhereIsMyBoat) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Points to your boat when it is off the chart")
set(PKG_DESCRIPTION [=[
When your own ship is outside the visible chart, an arrow at the edge of
the chart points towards it and shows its distance and bearing. Click the
arrow to move the chart to the boat.
]=])

set(PKG_AUTHOR "Isto Hunttila")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/istoh/whereismyboat_pi)
set(PKG_INFO_URL https://github.com/istoh/whereismyboat_pi)

set(SRC
    src/whereismyboat_pi.h
    src/whereismyboat_pi.cpp
    src/pointer.h
    src/pointer.cpp
)

set(PKG_API_LIB api-18)  #  A dir in opencpn-libs/ e. g., api-17 or api-16

macro(late_init)
endmacro ()

macro(add_plugin_libraries)
  add_subdirectory("${CMAKE_SOURCE_DIR}/opencpn-libs/plugin_dc")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugin-dc)
endmacro ()
