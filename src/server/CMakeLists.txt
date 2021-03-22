set(SOURSE_FIlES "main.cpp"
	"Server.cpp"
	"Server.h"
	"CSVProcessor.h" 
	"CSVProcessor.cpp"
	"CSVFile.cpp"
	"../TransmitSettings.h")
add_executable (server ${SOURSE_FIlES} )
target_link_libraries(server Boost::date_time Boost::regex)
target_include_directories(server PRIVATE ${BOOST_INCLUDE_DIRS})