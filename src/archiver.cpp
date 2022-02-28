#include "archiver.hpp"

void Archiver::Unpack(std::ostream& os, std::string_view filename) {
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

        os << " [Packet " << counter << "]" << endl;
        os << "Time of arrival: " << time_string << endl;
        os << "Size: " << packet.getFirstLayer()->getDataLen() << " bytes"
           << endl;
        os << "Sender port: "
           << packet.getLayerOfType<pcpp::UdpLayer>()->getSrcPort() << endl;
        os << "Receiver port: "
           << packet.getLayerOfType<pcpp::UdpLayer>()->getDstPort() << endl;
        os << endl;
    });
}

Archiver::unique_pipe Archiver::get_pipe(std::string_view filename) {
    std::string gzip_command{"gzip -d -c "};
    gzip_command.append(filename);

    unique_pipe pipe(gzip_command.c_str());

    if (!pipe.IsValid()) {
        throw std::runtime_error("Could not create pipe");
    }

    return pipe;
}

bool Archiver::next(pcap_t* pcap, pcpp::LinkLayerType layer_type,
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

    return raw_packet.setRawData(my_data, static_cast<int>(header.caplen), ts,
                                 layer_type, static_cast<int>(header.len));
}

void Archiver::for_each(pcap_t* pcap, PacketHandler auto function) {
    int layer_int{pcap_datalink(pcap)};
    auto layer_type{static_cast<pcpp::LinkLayerType>(layer_int)};

    pcpp::RawPacket iter;
    for (size_t counter = 1; next(pcap, layer_type, iter); ++counter) {
        pcpp::Packet packet(&iter);
        std::invoke(function, packet, counter);
    }
}
