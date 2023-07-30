add_library(
  tsl 
  tsl.c
  variable.c
  instructions.c
  function.c
)
target_include_directories(tsl PUBLIC ${tml_SOURCE_DIR}/include/tsl) 
