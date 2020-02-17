%{

#define YY_NO_UNISTD_H

#include "loader_def.h"
#include "lex_yywrap.h"

%}

DOUBLE [-+]?[0-9]*(\.[0-9]+)([eE][-+]?[0-9]+)?
INT [-+]?[0-9]+
WS [ \t]
NAME [a-zA-Z][a-zA-Z0-9\-_.]*
FILE_NAME [a-zA-Z./][a-zA-Z0-9\-_./]*

%%

\n    { ; }
{WS}+ { ; }
#.*   { ; }

^TYPE              { return TYPE_STATE; }
^VERTEX            { return VERTEX_STATE; }
^NORMAL            { return NORMAL_STATE; }
^TEX_COORD         { return TEX_COORD_STATE; }
^POINT             { return POINT_STATE; }
^NAME              { return NAME_STATE; }
^POLYGON_MESH_TYPE { return POLYGONMESH_TYPE_STATE; }
^MODEL_FILE{WS}+({NAME}|{FILE_NAME})({WS}+({NAME}|{FILE_NAME}))? { return MODEL_FILE1_STATE; }
^MODEL_FILE{WS}+({NAME}|{FILE_NAME})({WS}+({NAME}|{FILE_NAME}))?{WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}) { return MODEL_FILE2_STATE; }
^MODEL_FILE{WS}+({NAME}|{FILE_NAME})({WS}+({NAME}|{FILE_NAME}))?{WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}) { return MODEL_FILE3_STATE; }
^MODEL_FILE{WS}+({NAME}|{FILE_NAME})({WS}+({NAME}|{FILE_NAME}))?{WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}){WS}+({INT}|{DOUBLE}) { return MODEL_FILE4_STATE; }

COMPOSITE   { return COMPOSITE_TYPE_GATHERED; }
MESH        { return MESH_TYPE_GATHERED; }
{DOUBLE}    { return DOUBLE_GATHERED; }
{INT}       { return INT_GATHERED; }
{NAME}      { return NAME_GATHERED; }
{FILE_NAME} { return FILE_NAME_GATHERED; }

. { return LEX_ERROR; }

%%
