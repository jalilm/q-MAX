#include "PacketsReader.hpp"

using namespace std;

namespace QMax {
void PacketsReader::getNextUCLAPacket(const CSVIterator &it) {
  int i = 0;
  while (i < (*it).size()) {
    switch (i) {
      case 0:id++;
        break;
      case 1:IP_SRC = (*it)[1];
        break;
      case 2:IP_DST = (*it)[2];
        break;
      case 3:PORT_SRC = (*it)[3];
        break;
      case 4:PORT_DST = (*it)[4];
        break;
      case 5:
        if ((*it)[5] == "U") {
          length = stoi((*it)[6]) + 64; // add 64 bytes of headers.
          proto = "UDP (17)";
        } else {
          length = stoi((*it)[5]) + 64; // add 64 bytes of headers.
          proto = "TCP (6)";
        }
        break;
      default:;
    }
    ++i;
  }
}

void PacketsReader::getNextUNIVPacket(const CSVIterator &it) {
  id = stoi((*it)[0]);
  proto = (*it)[1];
  length = stoi((*it)[2]);
  IP_SRC = (*it)[3];
  IP_DST = (*it)[4];
  PORT_SRC = (*it)[5];
  PORT_DST = (*it)[6];
}

void PacketsReader::getNextCAIDAPacket(const CSVIterator &it) {
  for (int i = 0; i < 7; i++) {
    string field = (*it)[i];
    auto pos = field.find(':');
    auto fs = field.substr(pos + 1, field.length());
    switch (i) {
      case 0:id = stoi(fs);
        break;
      case 1:proto = fs;
        break;
      case 2:length = stoi(fs);
        break;
      case 3:IP_SRC = fs;
        break;
      case 4:IP_DST = fs;
        break;
      case 5:PORT_SRC = fs;
        break;
      case 6:PORT_DST = fs;
        break;
      default:break;
    }
  }
}

void PacketsReader::getNextPacket(const CSVIterator &it) {
  switch (dataset) {
    case DATASET::CAIDA:
    case DATASET::CAIDA18:
      return getNextCAIDAPacket(it);
    case DATASET::UCLA:
    case DATASET::UCLA_FULL:return getNextUCLAPacket(it);
    case DATASET::UNIV1:
    case DATASET::UNIV2:return getNextUNIVPacket(it);
    default:;
  }
}

IPPacket *PacketsReader::getNextIPPacket() {
  if (it == CSVIterator() || read_pkts==stop_after_pkts) {
    return nullptr;
  }
  try {
    getNextPacket(it++);
  } catch (...) {
    return getNextIPPacket();
  }
  ++read_pkts;
  return new IPPacket(IP_SRC, IP_DST, id, length);
}

TransportPacket *PacketsReader::getNextTransportPacket() {
  if (it == CSVIterator() || read_pkts == stop_after_pkts) {
    return nullptr;
  }
  try {
    getNextPacket(it++);
  } catch (...) {
    return getNextTransportPacket();
  }
  ++read_pkts;
  return new TransportPacket(IP_SRC, IP_DST, PORT_SRC, PORT_DST, proto, id, length);
}
}