#pragma once

#include <any>
#include <unordered_map>
#include <string>

class UObject {
  public:
 template <typename T>
 void set_property(std::string_view name, T&& t) {
   _properties[std::string(name)] = std::forward<T>(t);
 }

 std::any& get_property(const std::string& name) {
   return _properties.at(name);
 }
private:
  std::unordered_map<std::string, std::any> _properties;
};


