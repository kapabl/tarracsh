## Call graph features
    - outout representable call-graph 
    - replace a version of lib in jar and create a separate call-graph and check if call-graph breaks:
        -missing function call that existed before

## Third parties (maven)
    - given maven lib, pull all versions from
maven and save it in store
    - pull third party lib from jar and plug new version and check 
    if call graph breaks
    - allow query that output missing method/class/signature