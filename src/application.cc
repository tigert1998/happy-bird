#include "application.hpp"

bool Application::keys_pressed[1024] = {};
Application Application::shared = Application();
