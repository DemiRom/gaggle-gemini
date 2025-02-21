project('gaggle-gemeni', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++26'])

executable('gaggle-gemeni',
           'src/GeminiException.cpp',
           'src/GeminiException.h',
           'src/GeminiRequest.cpp',
           'src/GeminiRequest.h',
           'src/ResponseObject.h',
           'src/ResponseParser.cpp',
           'src/ResponseParser.h',
           'src/StringUtils.cpp',
           'src/StringUtils.h',
           'src/main.cpp',
           dependencies : [
              dependency('OpenSSL'),
              dependency('SQLite3')],
           install : true)
