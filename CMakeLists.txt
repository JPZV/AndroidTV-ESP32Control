idf_component_register(SRCS 
                    "src/TVConnectionStatus.cpp"
                    "src/TVControl.cpp"
                    "src/TVControlConfiguration.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_common esp_system
                    esp-nimble-cpp)
