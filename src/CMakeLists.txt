add_library(WrappedVarLib INTERFACE)

target_link_libraries(WrappedVarLib INTERFACE
  project_options
  project_warnings
)

target_include_directories(WrappedVarLib INTERFACE
  ${PROJECT_SOURCE_DIR}/src
)