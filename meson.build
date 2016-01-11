project('rust-mode', 'cpp', version: '0.5', license: 'MIT')

sources_c = run_command('source-grabber.sh')
sources = sources_c.stdout().strip().split('\n')

add_global_arguments(['-std=c++14', '-fdiagnostics-color=always'], language: 'cpp')

librustmode = shared_library('rust-mode', sources)

inc_dir = include_directories('src')
boost_dep = dependency('boost', modules: ['test'])

readme_error_example = executable('tests/readme_error_example',       'tests/readme_error_example.cpp',    include_directories: inc_dir, link_with: librustmode, cpp_pch: 'pch/error_pch.hpp')
readme_iterator_example = executable('tests/readme_iterator_example', 'tests/readme_iterator_example.cpp', include_directories: inc_dir, link_with: librustmode, cpp_pch: 'pch/iterator_pch.hpp')
readme_utils_example = executable('tests/readme_utils_example',       'tests/readme_utils_example.cpp',    include_directories: inc_dir, link_with: librustmode, cpp_pch: 'pch/utils_pch.hpp')

error_testing = executable('tests/error_testing',       'tests/error_testing.cpp',    include_directories: inc_dir, link_with: librustmode, dependencies: boost_dep, cpp_pch: 'pch/error_pch.hpp')
iterator_testing = executable('tests/iterator_testing', 'tests/iterator_testing.cpp', include_directories: inc_dir, link_with: librustmode, dependencies: boost_dep, cpp_pch: 'pch/iterator_pch.hpp')
utils_testing = executable('tests/utils_testing',       'tests/utils_testing.cpp',    include_directories: inc_dir, link_with: librustmode, dependencies: boost_dep, cpp_pch: 'pch/utils_pch.hpp')

test('Readme Error Example',    readme_error_example)
test('Readme Iterator Example', readme_iterator_example)
test('Readme Utils Example',    readme_utils_example)

test('Error Testing Suite',    error_testing,    args: ['-l', 'test_suite'])
test('Iterator Testing Suite', iterator_testing, args: ['-l', 'test_suite'])
test('Utils Testing Suite',    utils_testing,    args: ['-l', 'test_suite'])

# vim: ft=cmake
