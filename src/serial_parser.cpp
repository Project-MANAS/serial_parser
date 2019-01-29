#include "serial_parser/serial_parser.h"

using namespace serial;

SerialParser::SerialParser(Serial& serial, std::string start_delimiter,
                           std::string end_delimiter, int bytes_to_read) {
  this->serial = &serial;
  this->start_delimiter_length = strlen(start_delimiter.c_str());
  this->end_delimiter_length = strlen(end_delimiter.c_str());
  this->start_delimiter =
      reinterpret_cast<const uint8_t*>(start_delimiter.data());
  this->end_delimiter =
      reinterpret_cast<const uint8_t*>(end_delimiter.data());
  this->bytes_to_read = bytes_to_read;
}

bool start_reading(uint8_t* start_buffer, const uint8_t* start_delimiter, size_t start_delimiter_length) {
  if (start_delimiter_length > 0)
    return memcmp(start_buffer, start_delimiter, start_delimiter_length) == 0;
  return true;
}

bool end_reading(uint8_t* end_buffer, const uint8_t* end_delimiter, size_t end_delimiter_length) {
  if (end_delimiter_length > 0)
    return memcmp(end_buffer, end_delimiter, end_delimiter_length) == 0;
  return true;
}

std::string SerialParser::get_parsed_string() {
  return this->parse(&start_reading, &end_reading);
}

std::string SerialParser::get_parsed_string(delimiter_function* start_function, delimiter_function* end_function) {
  return this->parse(start_function, end_function);
}

std::string SerialParser::parse(delimiter_function* start_function, delimiter_function* end_function) {
  std::string parsed_string;
  uint8_t start_buffer[start_delimiter_length], end_buffer[end_delimiter_length];
  uint8_t c;

  while (serial->available()) {
    parsed_string = "";
    c = (uint8_t)serial->read()[0];

    if (start_delimiter_length > 0) {
      memmove(start_buffer, start_buffer + 1, start_delimiter_length - 1);
      start_buffer[start_delimiter_length - 1] = c;
    }

    if (!(*start_function)(start_buffer, start_delimiter,
                          start_delimiter_length))
      continue;

    for (int i = 0; i < bytes_to_read; ++i) {
      parsed_string += serial->read()[0];
    }

    for (int i = 0; i < end_delimiter_length; ++i)
      end_buffer[i] = (uint8_t)serial->read()[0];

    if (!(*end_function)(end_buffer, end_delimiter, end_delimiter_length))
      continue;

    return parsed_string;
  }
  return "";
}
