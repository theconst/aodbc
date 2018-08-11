{
  "targets": [
    {
      "target_name": "acallin",
      "sources": [ "src/acallin.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    },
    {
         "target_name": "copy_acallin",
         "type":"none",
         "dependencies" : [ "acallin" ],
         "copies":
         [
            {
               'destination': '<(module_root_dir)/node_modules',
               'files': ['<(module_root_dir)/build/Release/acallin.node']
            }
         ]
      }
  ]
}
