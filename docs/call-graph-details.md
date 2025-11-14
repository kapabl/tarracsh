## Call graph features
    - outout representable call-graph 
    - replace a version of lib in jar and create a separate call-graph and check if call-graph breaks:
        -missing function call that existed before

### Export formats
`tarracsh call-graph` skips writing graph artifacts by default. Pass `--dot` and/or `--gml` to emit `callgraph/callgraph.dot` or `callgraph/callgraph.gml` under the selected `--output-dir`.

## Pending tasks
    - populate `FieldRefRow.classname` in `src/domain/graph/ClassfileProcessor.cpp` so field references can be linked back to concrete fields and produce edges
    - implement the `runAsClient` and `runAsServer` flows in `src/app/commands/callgraph/CallGraph.cpp` (currently just stubs that print “not implemented yet”)
    - hook up `CallGraphAnalyzer::doClassFile` / `processStandaloneClassFile` so single-class inputs work instead of exiting early
    - revisit persistence: `CallGraphAnalyzer::endAnalysis()` stops the DB but `write()` is still commented out, so call-graph data never reaches disk outside long-running servers
## Third parties (maven)
    - given maven lib, pull all versions from
maven and save it in store
    - pull third party lib from jar and plug new version and check 
    if call graph breaks
    - allow query that output missing method/class/signature
