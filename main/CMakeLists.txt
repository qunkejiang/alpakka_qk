

set(SOURCES "main.cc"
    "application.cc"
)
set(INCLUDE_DIRS "."  )

# 添加板级公共文件
file(GLOB BOARD_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/*.cc)
list(APPEND SOURCES ${BOARD_COMMON_SOURCES})
file(GLOB BOARD_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/hid/*.cc)
list(APPEND SOURCES ${BOARD_COMMON_SOURCES})
file(GLOB BOARD_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/profiles/*.c)
list(APPEND SOURCES ${BOARD_COMMON_SOURCES})
file(GLOB BOARD_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
list(APPEND SOURCES ${BOARD_COMMON_SOURCES})
file(GLOB UI_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.c)
list(APPEND SOURCES ${UI_COMMON_SOURCES})
file(GLOB UI_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp)
list(APPEND SOURCES ${UI_COMMON_SOURCES})
file(GLOB IMAGE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/images/*.c)
list(APPEND SOURCES ${IMAGE_SOURCES})
file(GLOB FONTS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/fonts/*.c)
list(APPEND SOURCES ${FONTS_SOURCES})

list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/hid)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)



idf_component_register(SRCS ${SOURCES}
                    PRIV_REQUIRES spi_flash nvs_flash  fatfs esp_adc esp_driver_gpio driver usb
                    INCLUDE_DIRS ${INCLUDE_DIRS}
                    )


idf_component_get_property(tusb_lib espressif__tinyusb COMPONENT_LIB)
target_include_directories(${tusb_lib} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/hid)      

spiffs_create_partition_image(storage ../spiffs_img FLASH_IN_PROJECT)
                    
