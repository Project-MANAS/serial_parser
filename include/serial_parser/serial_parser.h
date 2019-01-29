#include "serial/serial.h"

namespace serial {
using delimiter_function = bool(uint8_t*, const uint8_t*, size_t);
class SerialParser {
 private:
  Serial* serial;
  const uint8_t* start_delimiter;
  const uint8_t* end_delimiter;
  size_t start_delimiter_length;
  size_t end_delimiter_length;
  int bytes_to_read;

 public:
  SerialParser() = default;
  ~SerialParser() = default;
  SerialParser(Serial& serial, std::string start_delimiter,
               std::string end_delimiter, int bytes_to_read);
  std::string parse(delimiter_function* start_function,
                    delimiter_function* end_function);
  std::string get_parsed_string();
  std::string get_parsed_string(delimiter_function* start_function,
                                delimiter_function* end_function);
};
}
