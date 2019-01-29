#include "serial_parser/serial_parser.h"

using namespace serial;

SerialParser::SerialParser(Serial& serial, std::string start_delimitter,
                           std::string end_delimmiter, int bytes_to_read) {
  this->serial = &serial;
  this->start_delimitter_length = strlen(start_delimitter.c_str());
  this->end_delimmiter_length = strlen(end_delimmiter.c_str());
  this->start_delimitter =
      reinterpret_cast<const uint8_t*>(start_delimitter.data());
  this->end_delimmiter =
      reinterpret_cast<const uint8_t*>(end_delimmiter.data());
  this->bytes_to_read = bytes_to_read;
}

bool start_reading(uint8_t* start_buffer, const uint8_t* start_delimitter,
                   size_t start_delimitter_length) {
  return memcmp(start_buffer, start_delimitter, start_delimitter_length) == 0;
}

bool end_reading(uint8_t* end_buffer, const uint8_t* end_delimmiter,
                 size_t end_delimitter_length) {
  return memcmp(end_buffer, end_delimmiter, end_delimitter_length) == 0;
}

std::string SerialParser::get_parsed_string() {
  return this->parse(&start_reading, &end_reading);
}

std::string SerialParser::get_parsed_string(delimmiter_function* start_function,
                                            delimmiter_function* end_function) {
  return this->parse(start_function, end_function);
}

std::string SerialParser::parse(delimmiter_function* start_function,
                                delimmiter_function* end_function) {
  uint8_t start_buffer[start_delimitter_length];
  while (serial->available()) {
    uint8_t c = (uint8_t)serial->read()[0];
    memmove(start_buffer, start_buffer + 1, start_delimitter_length - 1);
    start_buffer[start_delimitter_length - 1] = c;

    if ((*start_function)(start_buffer, start_delimitter,
                          start_delimitter_length)) {
      std::string parsed_string;
      for (int i = 0; i < bytes_to_read; ++i) {
        parsed_string += serial->read()[0];
      }

      uint8_t end_buffer[end_delimmiter_length];
      for (int i = 0; i < end_delimmiter_length; ++i)
        end_buffer[i] = (uint8_t)serial->read()[0];
      if ((*end_function)(end_buffer, end_delimmiter, end_delimmiter_length))
        return parsed_string;
    }
  }
  return "";
}
