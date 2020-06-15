{
  "targets": [
    {
      "target_name": "gist",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "./src/binding.cpp",
        "./src/Gist/src/Gist.cpp",
        "./src/Gist/src/core/CoreFrequencyDomainFeatures.cpp",
        "./src/Gist/src/core/CoreTimeDomainFeatures.cpp",
        "./src/Gist/src/fft/WindowFunctions.cpp",
        "./src/Gist/src/mfcc/MFCC.cpp",
        "./src/Gist/src/onset-detection-functions/OnsetDetectionFunction.cpp",
        "./src/Gist/src/pitch/Yin.cpp",
        "./src/Gist/libs/kiss_fft130/kiss_fft.c"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "./src/Gist/src",
        "./src/Gist/libs/kiss_fft130"
      ],
      "ldflags": [
      ],
      "cflags_cc": [
        "-std=c++11",
        "-Wall",
        "-Wno-sign-compare",
        "-Wno-unused-local-typedefs",
        "-Wno-deprecated-declarations",
        "-Winit-self",
        "-g",
        "-fPIC",
      ],
      "library_dirs": [
      ],
      "libraries": [
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS', 'USE_KISS_FFT' ],
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
