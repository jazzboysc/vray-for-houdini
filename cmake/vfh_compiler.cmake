#
# Copyright (c) 2015, Chaos Software Ltd
#
# V-Ray For Houdini
#
# ACCESSIBLE SOURCE CODE WITHOUT DISTRIBUTION OF MODIFICATION LICENSE
#
# Full license text: https://github.com/ChaosGroup/vray-for-houdini/blob/master/LICENSE
#

if(WIN32)
	# Houdini specific
	set(CMAKE_CXX_FLAGS "/wd4355 /w14996 /wd4800 /wd4244 /wd4305 /wd4251 /wd4275 /wd4396 /wd4018 /wd4267 /wd4146 /EHsc /GT /bigobj")

else()
	set(CMAKE_CXX_FLAGS       "-std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "-g -DNDEBUG")

	if (APPLE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
		add_definitions(-DBOOST_NO_CXX11_RVALUE_REFERENCES)
	endif()

	add_definitions(-D__OPTIMIZE__)

	# Houdini specific
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-parentheses -Wno-sign-compare -Wno-reorder -Wno-uninitialized -Wno-unused-parameter -Wno-deprecated -fno-strict-aliasing")

	# Houdini SDK / V-Ray SDK
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch -Wno-narrowing -Wno-int-to-pointer-cast")

	if (NOT APPLE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-literal-suffix -Wno-unused-local-typedefs")

		set(CMAKE_CXX_CREATE_SHARED_LIBRARY "<CMAKE_CXX_COMPILER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> -Wl,--start-group <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group")
	endif()
endif()

message(STATUS "Using flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "  Release: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "  Debug:   ${CMAKE_CXX_FLAGS_DEBUG}")
