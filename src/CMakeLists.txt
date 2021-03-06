set(CAPFILES_TARGET_NAME            ${PROJECT_NAME})

###
### SOURCES
###
set(SRC
        archiver.cpp
        archiver.hpp
        cli_wrapper.hpp
        download_manager.cpp
        download_manager.hpp
        command.hpp
        session.hpp)

###
### BUILD
###
add_executable(${CAPFILES_TARGET_NAME}
        main.cpp
        ${SRC})

###
### LIBRARIES
###
find_package(Boost REQUIRED)
find_package(cli CONFIG REQUIRED)

find_library(libpcap libpcap.a)
find_library(pcpp_common libCommon++.a)
find_library(pcpp_packet libPacket++.a)
find_library(pcpp_pcap libPcap++.a)

target_link_libraries(${CAPFILES_TARGET_NAME} PRIVATE
        Boost::boost
        cli::cli

        ${pcpp_packet}
        ${pcpp_pcap}
        ${pcpp_common}
        ${libpcap})