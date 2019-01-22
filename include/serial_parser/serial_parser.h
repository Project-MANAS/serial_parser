#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "serial/serial.h"

namespace serial {
class SerialParser {
 private:
  Serial *serial;
  const uint8_t *start_delimitter;
  const uint8_t *end_delimmiter;
  int start_delimitter_length;
  int end_delimmiter_length;
  int bytes_to_read;

 public:
  SerialParser(){};
  ~SerialParser(){};
  SerialParser(Serial &serial, std::string start_delimitter,
               std::string end_delimmiter, int bytes_to_read);
  std::string parse(
      std::function<bool(uint8_t *, const uint8_t *)> start_function,
      std::function<bool(uint8_t *, const uint8_t *)> end_function);
  std::string get_parsed_string();
  std::string get_parsed_string(
      std::function<bool(uint8_t *, const uint8_t *)> start_function,
      std::function<bool(uint8_t *, const uint8_t *)> end_function);
};
}  // namespace serial
