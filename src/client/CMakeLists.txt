set(SOURSE_FIlES "main.cpp"
	"Client.cpp"
	"Client.h"
	"../TransmitSettings.h")
add_executable (client ${SOURSE_FIlES} )
target_link_libraries(client Boost::date_time Boost::regex Boost::thread)
target_include_directories(client PRIVATE ${BOOST_INCLUDE_DIRS})