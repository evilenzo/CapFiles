#pragma once

#include "EthLayer.h"
#include "HttpLayer.h"
#include "IPv4Layer.h"
#include "Packet.h"
#include "PcapFileDevice.h"
#include "SystemUtils.h"
#include "UdpLayer.h"
#include "command.hpp"
#include "pcap.h"

#include <cstdio>
#include <filesystem>
#include <string_view>

/// @brief Concept for checking for_each lambdas
template <typename Func>
concept PacketHandler = std::invocable<Func, const pcpp::Packet&, size_t>;

/// @brief Class for unpacking file in memory and parsing its contents
class Archiver {
    static constexpr auto TIME_FORMAT{"%H:%M:%S %d-%m-%Y"};
    static constexpr auto TIME_STRING_SIZE{23};

    /// @brief RAII pipe
    class unique_pipe {
      public:
        unique_pipe(std::string_view command) {
            m_pipe = popen(command.data(), "r");
        }

        unique_pipe(unique_pipe&& pipe) = default;
        unique_pipe& operator=(unique_pipe&& pipe) = default;

        unique_pipe(const unique_pipe& pipe) = delete;
        unique_pipe& operator=(const unique_pipe& pipe) = delete;

        bool IsValid() { return m_pipe; }

        FILE* GetPipe() { return m_pipe; }

        ~unique_pipe() { pclose(m_pipe); }

      private:
        FILE* m_pipe;
    };

  public:
    /// @brief Unpack file to memory and parse packets
    static void Unpack(std::ostream& os, std::string_view filename);

    /// @brief Commands list for CLI wrapper
    static constexpr auto METHODS = std::tuple(  //
        CommandObject{"unpack", "Unpack file in memory",
                      [](std::ostream& os, const std::string& filename) {
                          os << "Unpacking..." << std::endl;
                          try {
                              Archiver::Unpack(os, filename);
                          } catch (const std::exception& ex) {
                              os << ex.what() << std::endl;
                          }
                      }});

  private:
    /// @brief Unpack file in memory and get pipe
    static unique_pipe get_pipe(std::string_view filename);

    /// @brief Get next packet
    static bool next(pcap_t* pcap, pcpp::LinkLayerType layer_type,
                     pcpp::RawPacket& raw_packet);

    /// @brief Iterate through all pcap packets and invoke some handler
    static void for_each(pcap_t* pcap, PacketHandler auto function);
};
