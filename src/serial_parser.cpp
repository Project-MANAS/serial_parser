#include "serial_parser/serial_parser.h"

using namespace serial;

SerialParser::SerialParser(Serial &serial, std::string start_delimitter, std::string end_delimmiter, int bytes_to_read) {
  this->serial = &serial;
  this->start_delimitter_length = strlen(start_delimitter.c_str());
  this->end_delimmiter_length = strlen(end_delimmiter.c_str());
  this->start_delimitter = reinterpret_cast<const uint8_t*>(start_delimitter.data());
  this->end_delimmiter = reinterpret_cast<const uint8_t*>(end_delimmiter.data());
  this->bytes_to_read = bytes_to_read;
}

bool start_reading(uint8_t *start_buffer, const uint8_t *start_delimitter) {
    if (memcmp(start_buffer, start_delimitter, 2) == 0) {
      return true;
    }
    return false;
}

bool end_reading(uint8_t *end_buffer, const uint8_t *end_delimmiter) {
    if (memcmp(end_buffer, end_delimmiter, 2) == 0) {
      return true;
    }
    return false;
}

std::string SerialParser::get_parsed_string() {
  return this->parse(&start_reading, &end_reading);
}

std::string SerialParser::get_parsed_string(bool (*start_function)(uint8_t *, const uint8_t *), bool (*end_function)(uint8_t *, const uint8_t *)) {
  return this->parse(start_function, end_function);
}

std::string SerialParser::parse(bool (*start_function)(uint8_t *, const uint8_t *), bool (*end_function)(uint8_t *, const uint8_t *)) {
  uint8_t start_buffer[start_delimitter_length];
  char c;
  while (serial->available()) {
    uint8_t c = serial->read()[0];
    memmove(start_buffer, start_buffer + 1, start_delimitter_length - 1);
    start_buffer[start_delimitter_length - 1] = c;

    if ((*start_function)(start_buffer, start_delimitter)) {
      std::string parsed_string;
      for (int i = 0; i < bytes_to_read; ++i) {
        parsed_string += serial->read()[0];
      }

      uint8_t end_buffer[end_delimmiter_length];
      c = serial->read()[0];
      for (int i = 1; i < 2; ++i) end_buffer[i] = c;
      if((*end_function)(end_buffer, end_delimmiter));
        return parsed_string;
    }
  }
  return "";
}
