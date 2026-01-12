#pragma once

#include "esphome.h"

class RC3Climate : public climate::Climate, public uart::UARTDevice {
 public:
  explicit RC3Climate(uart::UARTComponent *parent);

  void setup() override;
  void loop() override;
  climate::ClimateTraits traits() override;

 protected:
  void control(const climate::ClimateCall &call) override;
};
