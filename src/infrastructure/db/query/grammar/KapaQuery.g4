grammar KapaQuery;		
query: statement;
statement: list | schema;
schema: 'schema';
list: ('list'|'select') tablename (where|);
where: 'where' expr;

expr: filter 
    | '(' expr logical_oper expr ')'
    ;

filter: column oper EscapedString;

tablename: Identifier;
column: Identifier;

oper: REGEX | EQUAL | NOT_EQUAL | STARS_WITH | END_WITH;
logical_oper: AND | OR;

NOT_EQUAL: '!=';
EQUAL: '=';
REGEX: '*^*';
STARS_WITH: '^*';
END_WITH: '*^';

AND: 'and';
OR: 'or';

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
