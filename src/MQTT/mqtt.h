#pragma once

#include <stdio.h>
#include <string>

class mqtt {
  private:
    std::string rootTopic;

  public:
    mqtt(/* args */);
    ~mqtt();
    void sendMessage(std::string message);
    void recieveMessage(std::string message);
};

mqtt::mqtt(/* args */) {}

mqtt::~mqtt() {}
