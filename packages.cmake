include(FetchContent)

FetchContent_Declare(
	ClipsInfoModelRulemachine1
	GIT_REPOSITORY https://github.com/WhiteGobo/ClipsInfoModelRulemachine1.git
	#GIT_TAG 6ae08cf54ec74aa5f8a9d0ca8547155af53be03a
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(ClipsInfoModelRulemachine1)

find_package(clips REQUIRED)


FetchContent_Declare(
	hashtable
	GIT_REPOSITORY https://github.com/WhiteGobo/hashtable
        CMAKE_CACHE_ARGS -DBUILD_SHARED_LIBS=OFF
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(hashtable)

find_package(cwalk)

FetchContent_Declare(
	CInterfaceClipsSerializer
	GIT_REPOSITORY https://github.com/WhiteGobo/CInterfaceClipsSerializer.git
	#GIT_TAG 6ae08cf54ec74aa5f8a9d0ca8547155af53be03a
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(CInterfaceClipsSerializer)

find_package(Mime2Rdf4C)
#FetchContent_Declare(
#	Mime2Rdf4C
#	GIT_REPOSITORY https://github.com/WhiteGobo/Mime2Rdf4C.git
#	#GIT_TAG 8b244ca1826c024a641d98f0bb52e3c649e79972
#	FIND_PACKAGE_ARGS
#)
#FetchContent_MakeAvailable(Mime2Rdf4C)

FetchContent_Declare(
	RIFRDFQuery
	GIT_REPOSITORY https://github.com/WhiteGobo/RIFRDFQuery.git
        OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(RIFRDFQuery)
