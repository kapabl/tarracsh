grammar JvmSignature;

classSignature : 'L' packageSpecifier? simpleClassTypeSignature superclassSignature? interfaceSignature* fieldSignature* methodSignature* ';' ;

simpleClassTypeSignature : Identifier typeArguments? ;

superclassSignature : 'L' classTypeSignature ';' ;

interfaceSignature : 'L' classTypeSignature ';' ;

typeArguments : '<' typeArgument* '>' ;

typeArgument : wildcardIndicator? typeSignature ;

wildcardIndicator : '?' wildcardBound? ;

wildcardBound : 'extends' typeSignature | 'super' typeSignature ;

typeSignature : classTypeSignature | arrayTypeSignature | primitiveType ;

classTypeSignature : 'L' packageSpecifier? Identifier typeArguments? ('.' Identifier typeArguments?)* ';' ;

fieldSignature : fieldTypeSignature ;

methodSignature : methodTypeSignature ;

packageSpecifier : Identifier '/' ;

arrayTypeSignature : '[' typeSignature ;

fieldTypeSignature : typeSignature ;

methodTypeSignature : '(' parameterTypeSignature* ')' returnType throwsSignature* ;

parameterTypeSignature : typeSignature ;

returnType : typeSignature | 'V' ;

throwsSignature : '^' classTypeSignature ;

primitiveType : 'B' | 'C' | 'D' | 'F' | 'I' | 'J' | 'S' | 'Z' ;

Identifier : ('a'..'z' | 'A'..'Z' | '_' | '$') ('a'..'z' | 'A'..'Z' | '_' | '$' | '0'..'9')* ;

WS : [ \t\n\r] -> skip ;
