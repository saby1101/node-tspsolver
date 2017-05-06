{
  'targets': [
    {
      'target_name': 'node-tspsolver',
      'include_dirs': [
        '<!(node -e "require(\'nan\')")'
      ],
      'cflags' : [ '-std=c++11' ],
      'sources': [ 
          'src/addon.cpp',
          'src/tsp_v8.cpp',
          'src/tsp.cpp'
      ],
      'conditions': [
          [ 'OS!="win"', {
            'cflags+': [ '-std=c++11' ],
            'cflags_c+': [ '-std=c++11' ],
            'cflags_cc+': [ '-std=c++11' ],
          }],
          [ 'OS=="mac"', {
            'xcode_settings': {
              'OTHER_CPLUSPLUSFLAGS' : [ '-std=c++11', '-stdlib=libc++' ],
              'OTHER_LDFLAGS': [ '-stdlib=libc++' ],
              'MACOSX_DEPLOYMENT_TARGET': '10.7'
            }
          }]
        ]
    }
  ]
}
