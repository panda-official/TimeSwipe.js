{
    "targets": [{
        "target_name": "timeswipe",
        "cflags!": [ "-fno-exceptions", "-Wall" ],
        "cflags_cc!": [ "-fno-exceptions", "-Wall" ],
        "sources": [
            "napi.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "<!@(node -p \"require('napi-thread-safe-callback').include\")"
        ],
        'libraries': [
            "/usr/lib/libtimeswipe.a",
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ ]
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
    }]
}
