grammar KapaQuery;		
query: statement;
statement: list | schema;
schema: 'schema';
list: 'list' tablename (where|);
where: 'where' expr;

expr: filter 
    | '(' expr 'and' expr ')'
    | '(' expr 'or'  expr ')'
    ;

filter: column OP EscapedString;
OP: ( '=' | 'like');
tablename: Identifier;
column: Identifier;

Identifier: [_a-zA-Z][_a-zA-Z0-9]*;
WS: [ \t\r\n]+ -> skip;

EscapedString
    : '"'      (Escape | '""'   | ~["])* '"'
    | '\''     (Escape | '\'\'' | ~['])* '\''
    | '\u201C' (Escape | .)*? ('\u201D' | '\u2033')   // smart quotes
    ;

fragment Escape
    : '\u0060\''    // backtick single-quote
    | '\u0060"'     // backtick double-quote
    ;

