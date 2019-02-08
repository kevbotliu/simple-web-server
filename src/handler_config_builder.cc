#include "handler_config_builder.h"

HandlerConfig* HandlerConfigBuilder::build() {
	return new HandlerConfig(conf_filename_, handlers_);
}

HandlerConfigBuilder& HandlerConfigBuilder::set_config(std::string filename) {
	conf_filename_ = filename;
  return *this;
}

HandlerConfigBuilder& HandlerConfigBuilder::register_handler(std::string handler_name) {
	for (auto handler : handlers_)
		if (handler.name == handler_name) return *this;

	Handler h;
	h.name = handler_name;
  handlers_.push_back(h);
  return *this;
}
