#include <iostream>
#include <string>
#include <vector>
#include <iterator>
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
  SerialParser(Serial &serial, std::string start_delimitter, std::string end_delimmiter, int bytes_to_read);
  std::string parse(bool (*start_function)(uint8_t *, const uint8_t *), bool (*end_function)(uint8_t *, const uint8_t *));
  std::string get_parsed_string();
  std::string get_parsed_string(bool (*start_function)(uint8_t *, const uint8_t *), bool (*end_function)(uint8_t *, const uint8_t *));
};
}
