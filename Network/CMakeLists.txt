

# —оздаЄм статическую библиотеку 'network'
add_library(Network 
			Listener.cpp 
			Session.cpp 
			Logger.cpp
			Cache.cpp)


# ”казываем, где лежат заголовки (публичные)
target_include_directories(Network PUBLIC 
						   ${CMAKE_CURRENT_SOURCE_DIR}
						   ${Boost_INCLUDE_DIRS}
						   )