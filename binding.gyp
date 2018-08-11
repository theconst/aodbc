{
  "targets": [
    {
      "target_name": "anodem",
      "sources": [ "src/anodem.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    },
    {
         "target_name": "copy_v",
         "type":"none",
         "dependencies" : [ "anodem" ],
         "copies":
         [
            {
               'destination': '<(module_root_dir)/node_modules',
               'files': ['<(module_root_dir)/build/Release/anodem.node']
            }
         ]
      }
  ]
}
