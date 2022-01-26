#pragma once

#include "command.hpp"
//
#include "EthLayer.h"
#include "HttpLayer.h"
#include "IPv4Layer.h"
#include "Packet.h"
#include "PcapFileDevice.h"
#include "SystemUtils.h"
#include "UdpLayer.h"
#include "pcap.h"

#include <cstdio>
#include <filesystem>
#include <string_view>

template <typename Func>
concept PacketHandler = std::invocable<Func, const pcpp::Packet&, size_t>;

class Archiver {
    static constexpr auto TIME_FORMAT{"%H:%M:%S %d-%m-%Y"};
    static constexpr auto TIME_STRING_SIZE{23};

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
    static void Unpack(std::ostream& os, std::string_view filename) {
        using std::operator""s;

        if (!std::filesystem::exists(filename)) {
            throw std::runtime_error("File does not exist");
        }

        auto pipe = get_pipe(filename);
        char error_buffer[256];
        pcap_t* pcap = pcap_fopen_offline(pipe.GetPipe(), error_buffer);

        for_each(pcap, [&os](const pcpp::Packet& packet, size_t counter) {
            using namespace std;

            // Convert internal time representation to string
            time_t time_tt = packet.getRawPacket()->getPacketTimeStamp().tv_sec;
            char time_string[TIME_STRING_SIZE];
            struct tm* time_tm = localtime(&time_tt);
            strftime(time_string, TIME_STRING_SIZE, TIME_FORMAT, time_tm);

            os << " [Пакет " << counter << "]" << endl;
            os << "Время получения: " << time_string << endl;
            os << "Размер: " << packet.getFirstLayer()->getDataLen() << " байта"
               << endl;
            os << "Порт отправителя: "
               << packet.getLayerOfType<pcpp::UdpLayer>()->getSrcPort() << endl;
            os << "Порт получателя: "
               << packet.getLayerOfType<pcpp::UdpLayer>()->getDstPort() << endl;
            os << endl;
        });
    }

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
    static unique_pipe get_pipe(std::string_view filename) {
        std::string gzip_command{"gzip -d -c "};
        gzip_command.append(filename);

        unique_pipe pipe(gzip_command.c_str());

        if (!pipe.IsValid()) {
            throw std::runtime_error("Could not create pipe");
        }

        return pipe;
    }

    static bool next(pcap_t* pcap, pcpp::LinkLayerType layer_type,
                     pcpp::RawPacket& raw_packet) {
        raw_packet.clear();

        pcap_pkthdr header;
        const uint8_t* data = pcap_next(pcap, &header);

        if (data == nullptr) {
            return false;
        }

        auto my_data = new uint8_t[header.caplen];
        memcpy(my_data, data, header.caplen);
        const struct timeval ts = header.ts;

        return raw_packet.setRawData(my_data, static_cast<int>(header.caplen),
                                     ts, layer_type,
                                     static_cast<int>(header.len));
    }


    static void for_each(pcap_t* pcap, PacketHandler auto function) {
        int layer_int{pcap_datalink(pcap)};
        auto layer_type{static_cast<pcpp::LinkLayerType>(layer_int)};

        pcpp::RawPacket iter;
        for (size_t counter = 1; next(pcap, layer_type, iter); ++counter) {
            pcpp::Packet packet(&iter);
            std::invoke(function, packet, counter);
        }
    }
};
