#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "serial/serial.h"

namespace serial {
using delimmiter_function = bool(uint8_t*, const uint8_t*, size_t);
class SerialParser {
 private:
  Serial* serial;
  const uint8_t* start_delimitter;
  const uint8_t* end_delimmiter;
  size_t start_delimitter_length;
  size_t end_delimmiter_length;
  int bytes_to_read;

 public:
  SerialParser() = default;
  ~SerialParser() = default;
  SerialParser(Serial& serial, std::string start_delimitter,
               std::string end_delimmiter, int bytes_to_read);
  std::string parse(delimmiter_function* start_function,
                    delimmiter_function* end_function);
  std::string get_parsed_string();
  std::string get_parsed_string(delimmiter_function* start_function,
                                delimmiter_function* end_function);

};
}  // namespace serial
