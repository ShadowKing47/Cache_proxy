cmake_minimum_required (VERSION 3.10)


project ("CachingProxy" VERSION 1.0)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Путь к папке с Boost
set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../boost_1_86_0")

# Путь к папке с заголовочными файлами Boost
set(Boost_INCLUDE_DIR "${BOOST_ROOT}")


# Указываем CMake, где искать модули конфигурации.
if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
    # Путь к папке с файлами .cmake, который содержит конфигурационную информацию о boost 
    set(CMAKE_PREFIX_PATH "${BOOST_ROOT}/bin/x64/lib/cmake/")
    set(Boost_DIR "${BOOST_ROOT}/bin/x64/lib/cmake/")
    message(STATUS "Building for 64-bit architecture")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32-bit
    # Путь к папке с файлами .cmake, который содержит конфигурационную информацию о boost 
    set(CMAKE_PREFIX_PATH "${BOOST_ROOT}/bin/x86/lib/cmake/")
    set(Boost_DIR "${BOOST_ROOT}/bin/x86/lib/cmake/")
    message(STATUS "Building for 32-bit architecture")
else()
    message(FATAL_ERROR "Unsupported architecture")
endif()

set(Boost_INCLUDE_DIRS "${BOOST_ROOT}")

# Указываем, что нужно искать boost через заданные нами параметры, а не через стандартный поиск cmake
find_package(Boost COMPONENTS 
             program_options  
             system
             REQUIRED
            )


# Добавляем подкаталоги
add_subdirectory(CLI)
add_subdirectory(Network)

# Создаём исполняемый файл
add_executable(CachingProxy CachingProxy.cpp) 


# Указываем где лежат includes
target_include_directories(CachingProxy PUBLIC 
                           "${CMAKE_SOURCE_DIR}"
                           Boost::asio 
                           Boost::beast
                          )


# Связываем с нашими библиотеками и Boost
target_link_libraries(CachingProxy PUBLIC 
                      CLI 
                      Network
                      Boost::program_options  
                      Boost::system
                     )