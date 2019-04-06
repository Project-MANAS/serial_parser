#include "serial/serial.h"

namespace serial {
using delimiter_function = bool(uint8_t*, const uint8_t*, size_t);
class SerialParser {
 private:
  Serial* serial_;
  uint8_t* start_delimiter_;
  uint8_t* end_delimiter_;
  size_t start_delimiter_length_;
  size_t end_delimiter_length_;
  int bytes_to_read_;

 public:
  SerialParser() = default;
  ~SerialParser() = default;
  SerialParser(Serial& serial, std::string start_delimiter,
               std::string end_delimiter, int bytes_to_read);
  std::string parse(delimiter_function* start_function,
                    delimiter_function* end_function);  // TODO: Use pass by
                                                        // pointer not reference
  std::string get_parsed_string();
  std::string get_parsed_string(delimiter_function* start_function,
                                delimiter_function* end_function);
};
}
