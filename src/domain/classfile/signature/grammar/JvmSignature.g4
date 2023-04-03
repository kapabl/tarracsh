grammar JvmSignature;


all: fieldSignature | methodSignature | classSignature ;

javaTypeSignature : referenceTypeSignature | baseType ;
baseType : 'B' | 'C' | 'D' | 'F' | 'I' | 'J' | 'S' | 'Z' ;

referenceTypeSignature:
      classTypeSignature
    | typeVariableSignature
    | arrayTypeSignature
    ;



classTypeSignature : 'L' packageSpecifier? simpleClassTypeSignature classTypeSignatureSuffix* ';' ;

packageSpecifier : Identifier '/' packageSpecifier ;
simpleClassTypeSignature : Identifier typeArguments? ;
typeArguments : '<' typeArgument typeArgument* '>' ;
typeArgument : wildcardIndicator? referenceTypeSignature '*' ;
wildcardIndicator : '+' | '-' ;
classTypeSignatureSuffix : '.' simpleClassTypeSignature ;
typeVariableSignature : 'T' Identifier ';' ;
arrayTypeSignature: '[' javaTypeSignature ;

classSignature : typeParameters? superclassSignature superInterfaceSignature* EOF;
typeParameters : '<' typeParameter typeParameter* '>' ;
typeParameter : Identifier classBound interfaceBound* ;
classBound : referenceTypeSignature? ;
superclassSignature : classTypeSignature ;
superInterfaceSignature : classTypeSignature ;
interfaceBound : referenceTypeSignature ;

methodSignature: typeParameter? '(' javaTypeSignature* ')' result throwsSignature* EOF;
result : javaTypeSignature | voidDescriptor ;
throwsSignature : 
    '^' classTypeSignature
    '^' typeVariableSignature 
    ;
voidDescriptor : 'V' ;

fieldSignature : referenceTypeSignature EOF;

Identifier : ~[\u0000-\u001f.;[/<>:()]+ ;
