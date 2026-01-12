#include "rc3_climate.h"

RC3Climate::RC3Climate(uart::UARTComponent *parent)
    : UARTDevice(parent) {}

void RC3Climate::setup() {
  ESP_LOGI("rc3", "RC3 Climate component started");

  // default state
  this->mode = climate::CLIMATE_MODE_OFF;
  this->target_temperature = 22;
  this->current_temperature = 22;
  // Registreer dit component als Climate entity
  App.register_component(this);
  App.register_climate(this);
}

void RC3Climate::loop() {
  // v1: nog geen RX parsing
  while (available()) {
    char c = read();
    ESP_LOGD("rc3", "RX byte: 0x%02X", (uint8_t)c);
  }
}

climate::ClimateTraits RC3Climate::traits() {
  auto traits = climate::ClimateTraits();

  traits.set_supports_current_temperature(true);
  traits.set_supported_modes({
    climate::CLIMATE_MODE_OFF,
    climate::CLIMATE_MODE_COOL,
    climate::CLIMATE_MODE_HEAT,
    climate::CLIMATE_MODE_DRY,
    climate::CLIMATE_MODE_FAN_ONLY,
    climate::CLIMATE_MODE_AUTO
  });

  traits.set_supported_fan_modes({"1", "2", "3", "4"});
  traits.set_visual_min_temperature(16);
  traits.set_visual_max_temperature(30);
  traits.set_visual_temperature_step(0.5);

  return traits;
}

void RC3Climate::control(const climate::ClimateCall &call) {
  ESP_LOGI("rc3", "Climate control request received");

  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();
    ESP_LOGI("rc3", "Mode changed");
  }

  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
    ESP_LOGI("rc3", "Target temperature: %.1f", this->target_temperature);
  }

  if (call.get_fan_mode().has_value()) {
    ESP_LOGI("rc3", "Fan mode: %s", call.get_fan_mode()->c_str());
  }

  // publish state to Home Assistant
  this->publish_state();
}
