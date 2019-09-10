#ifndef QMAX_PACKET_HPP
#define QMAX_PACKET_HPP

#include <string>
#include <sstream>
#include <utility>
#include <functional>

namespace QMax {

class Packet {
 public:
  int id;
  int length;
 public:
  Packet() = default;
  Packet(int id, int length) : id(id), length(length) {}
  virtual ~Packet() = default;
  std::size_t getReprSizet() const {
    return std::hash<std::string>{}(getReprString());
  }
 private:
  virtual std::string getReprString() const = 0;
};

class IPPacket : public Packet {
 protected:
  std::string src_ip;
  std::string dst_ip;
 public:
  IPPacket() = default;

  ~IPPacket() override = default;

  IPPacket(std::string src_ip, std::string dst_ip, int id, int length) : src_ip(std::move(std::move(src_ip))),
                                                                         dst_ip(std::move(std::move(dst_ip))),
                                                                         Packet(id, length) {}

  inline friend std::ostream &operator<<(std::ostream &ostream, IPPacket const &v) {
    ostream << std::string("IP_SRC:") << v.src_ip << ",IP_DST:" << v.dst_ip << ",ID:" << v.id;
    return ostream;
  }
 private:
  std::string getReprString() const override {
    std::stringstream sstm;
    sstm << src_ip << "." << dst_ip;
    return sstm.str();
  }
};

class TransportPacket : public IPPacket {
 protected:
  std::string src_port;
  std::string dst_port;
  std::string protocol;
 public:
  TransportPacket() = default;

  ~TransportPacket() override = default;

  TransportPacket(std::string src_ip, std::string dst_ip, std::string src_port, std::string dst_port,
                  std::string protocol, int id, int length) :
      IPPacket(std::move(src_ip), std::move(dst_ip), id, length),
      src_port(std::move(std::move(src_port))),
      dst_port(std::move(std::move(dst_port))),
      protocol(std::move(std::move(protocol))) {}
      
  inline friend std::ostream &operator<<(std::ostream &ostream, TransportPacket const &v) {
    ostream << v.protocol << ",";
    ostream << std::string("IP_SRC:") << v.src_ip << ",IP_DST:" << v.dst_ip << ",ID:" << v.id;
    ostream << std::string("SRC_PORT:") << v.src_port << ",PORT_DST:" << v.dst_port;
    return ostream;
  }

 private:
  std::string getReprString() const override {
    std::stringstream sstm;
    sstm << src_ip << "." << dst_ip << "." << src_port << "." << dst_port << "." << protocol;
    return sstm.str();
  }
};
}

#endif //QMAX_PACKET_HPP
