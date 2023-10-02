## Install antlr4 and run this in the directory to generate c++ files
Antlr4 >= 4.9.2 is required

```
pip install antlr4-tools
```

```
antlr4 -Dlanguage=Cpp  -package kapa::infrastructure::db::query KapaQuery.g4
```

Installing antlr4 with vcpkg is required too