project('engine', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

project_description = 'An example shared library'

project_source_files = [
  'src/engine.cc',
  'src/renderer/vulkan.cc',
]

project_test_files = [
]

build_args = [
  '-std=c++23'
]

glfw = dependency('glfw3')
vulkan = dependency('vulkan')

dependencies = [
  glfw,
  vulkan,
]

exe = executable('engine', project_source_files,
  install : true,
  cpp_args : build_args,
  dependencies : dependencies)
