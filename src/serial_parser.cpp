#include "serial_parser/serial_parser.h"

using namespace serial;

SerialParser::SerialParser(Serial& serial, std::string start_delimiter,
                           std::string end_delimiter, int bytes_to_read) {
  serial_ = &serial;
  start_delimiter_length_ = strlen(start_delimiter.c_str());
  end_delimiter_length_ = strlen(end_delimiter.c_str());
  start_delimiter_ = (uint8_t  *) malloc(start_delimiter.size());
  end_delimiter_ = (uint8_t  *) malloc(end_delimiter.size());
  memcpy(start_delimiter_, start_delimiter.data(), start_delimiter.size());
  memcpy(end_delimiter_, end_delimiter.data(), end_delimiter.size());
  bytes_to_read_ = bytes_to_read;
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
  uint8_t start_buffer[start_delimiter_length_], end_buffer[end_delimiter_length_];
  uint8_t c;

  while (serial_->available()) {
    parsed_string = "";
    c = (uint8_t)serial_->read()[0];

    if (start_delimiter_length_ > 0) {
      memmove(start_buffer, start_buffer + 1, start_delimiter_length_ - 1);
      start_buffer[start_delimiter_length_ - 1] = c;
    }

    if (!(*start_function)(start_buffer, start_delimiter_,
                          start_delimiter_length_))
      continue;

    for (int i = 0; i < bytes_to_read_; ++i) {
      parsed_string += serial_->read()[0];
    }

    for (int i = 0; i < end_delimiter_length_; ++i)
      end_buffer[i] = (uint8_t)serial_->read()[0];

    if (!(*end_function)(end_buffer, end_delimiter_, end_delimiter_length_))
      continue;

    return parsed_string;
  }
  return "";
}
