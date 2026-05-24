set(CMAKE_OBJECT_PATH_MAX 450)
idf_component_register(
  SRCS "app_main.c" "server_main.c"
  INCLUDE_DIRS "."
  REQUIRES soc esp_system esp_hw_support esp_pm freertos lib
)
