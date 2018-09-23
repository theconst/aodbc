{
  "targets": [
    {
      "target_name": "nc",
      "cflags!": [ "-fno-exceptions", "-std=gnu++0x" ],
      "cflags": [ "-std=c++14", "-Wno-ignored-qualifiers" ],
      "cflags_cc!": [ "-fno-exceptions", "-std=gnu++0x" ],
      "sources": [ 
        "<!@(node -p \
          \"require('fs').readdirSync('<(module_root_dir)/src') \
              .map(function(f) { \
                return 'src/'+f \
              }) \
              .join(' ')\")"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "nanodbc/nanodbc/",
        "src"
      ],
      "conditions": [
          [
            "OS=='linux'", {
              "ldflags": [
                "-lnanodbc", "-L<(module_root_dir)/nanodbc/build/lib", "-lpthread",
                "-Wl,-rpath,<(module_root_dir)/nanodbc/build/lib"
              ],
            }
          ],
      ],
    },
    {
      "target_name": "copy_nc",
      "type":"none",
      "dependencies" : [ "nc" ],
      "copies": [
        {
          "destination": "<(module_root_dir)/node_modules",
          "files": ["<(module_root_dir)/build/Release/nc.node"]
        }
      ],
    }
  ]
}
