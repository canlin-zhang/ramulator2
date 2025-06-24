set(TRANSLATION_FILES
  translation.h
  register_translation.h

  impl/no_translation.h impl/no_translation.cpp
  impl/random_translation.h impl/random_translation.cpp
)

add_library(ramulator-translation STATIC ${TRANSLATION_FILES})

target_link_libraries(ramulator-translation PUBLIC
  ramulator-base
  ramulator-frontend
)

target_include_directories(ramulator-translation
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
