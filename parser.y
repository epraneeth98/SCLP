%{
#include <stdio.h>
int yyerror(char *);
int yylex(void);
list<Ast *> l;
Symbol_Table * s = new Symbol_Table();
Symbol_Table * s_local = new Symbol_Table();
int glob = 1;
extern int yylinen;
%}
%union 
{
        int number;
				float fnumber;
        string *string_value;
        Symbol_Table * symbol_table;
        Procedure * procedure;
        list<Ast *> *last;
        Ast * ast;
}
%token <number> NUM
%token PRINT
%token LESS_THAN
%token GREATER_THAN
%token LESS_THAN_EQUAL
%token GREATER_THAN_EQUAL
%token EQUAL
%token NEQ
%token IF
%token ELSE
%token WHILE
%token DO
%token AND
%token NOT
%token OR
%token INTEGER
%token FLOAT
%token VOID
%token ASSIGN
%token <fnumber> FNUM
%token <string_value> NAME
%nterm  <symbol_table> variable_list1
%nterm  <symbol_table> variable_list2
%nterm  <symbol_table> variable_declaration1
%nterm  <symbol_table> declaration1
%nterm  <symbol_table> variable_declaration_list1
%nterm <symbol_table> optional_variable_declaration_list
%type <procedure>  procedure_definition
%type <ast> variable
%type <ast> constant
%type <ast> assignment_statement
%type <ast> statement
%type <ast> condition
%type <ast> stat
%type <last> statement_list
%type <ast> exp
%right '='
%left '?' ':'
%left OR
%left AND
%left EQUAL NEQ
%left LESS_THAN GREATER_THAN LESS_THAN_EQUAL GREATER_THAN_EQUAL
%left '+' '-'
%left '*' '/'
%left UMINUS
%right NOT 
%nonassoc IFX
%nonassoc ELSE
%%

program								:	declaration_list procedure_definition
;

declaration_list					:	/* empty */	             
								{program_object.set_global_table(*(new Symbol_Table()));glob = yylinen;}
									| variable_declaration_list1 {program_object.set_global_table(*$1); s->append_list(*$1,yylinen);glob = yylinen;}			
;

variable_declaration_list1			 :	variable_declaration1 {$$ = new Symbol_Table(); $$->append_list(*$1,yylinen);}
									 |	variable_declaration_list1 
										variable_declaration1 {$$ = new Symbol_Table(); $$->append_list(*$1,yylinen); $$->append_list(*$2,yylinen);}
;

variable_declaration1				 :	declaration1 ';' {$$ = new Symbol_Table(); $$->append_list(*$1,yylinen);}
;

declaration1						 :	INTEGER variable_list1 {$$ = new Symbol_Table(); $$->append_list(*$2,yylinen);}
									 |  FLOAT variable_list2 {$$ = new Symbol_Table(); $$->append_list(*$2,yylinen);}
;

variable_list2                       : NAME {$$ = new Symbol_Table();
											 Symbol_Table_Entry * set1 = new Symbol_Table_Entry(*$1,double_data_type,yylinen); 
											 if (glob == 1) set1->set_symbol_scope(global); else set1->set_symbol_scope(local);
											 $$->push_symbol(set1);
											 }
                                     | variable_list2 ',' NAME {$$ = new Symbol_Table(); 
                                       Symbol_Table_Entry * set2 = new Symbol_Table_Entry(*$3,double_data_type,yylinen); 
											 if (glob == 1) set2->set_symbol_scope(global); else set2->set_symbol_scope(local);
                                      $$->append_list(*$1,yylinen);
                                      $$->push_symbol(set2);}

;
        
variable_list1                       : NAME {$$ = new Symbol_Table();
											 Symbol_Table_Entry * set1 = new Symbol_Table_Entry(*$1,int_data_type,yylinen); 
											 if (glob == 1) set1->set_symbol_scope(global); else set1->set_symbol_scope(local);
											 $$->push_symbol(set1);
											 }
                                     | variable_list1 ',' NAME {$$ = new Symbol_Table(); 
                                       Symbol_Table_Entry * set2 = new Symbol_Table_Entry(*$3,int_data_type,yylinen); 
											 if (glob == 1) set2->set_symbol_scope(global); else set2->set_symbol_scope(local);
                                      $$->append_list(*$1,yylinen);
                                      $$->push_symbol(set2);}
;

procedure_definition				:	VOID NAME '(' ')'
                  	   					'{'
											optional_variable_declaration_list
	                    					statement_list
        	           					'}' {$$ = new Procedure(void_data_type,*$2,yylinen);$$->set_ast_list(*$7); $$->set_local_list(*$6); program_object.set_procedure($$,yylinen);}
;

optional_variable_declaration_list	:	/* empty */ {$$ = new Symbol_Table();}
									|	variable_declaration_list1 {$$ = new Symbol_Table(); $$->append_list(*$1,yylinen);$$->set_table_scope(local);
                                        s_local->append_list(*$1,yylinen);}
;

statement_list	        			:	/* empty */ {$$ = new list<Ast*>;}
									|	statement_list statement {$$->push_back($2);}
;

statement 							:	assignment_statement {$$ = $1;}
									|	IF '(' condition ')' stat %prec IFX {$$ = new Selection_Statement_Ast($3,$5,NULL, yylinen);}
       								|   IF '(' condition ')' stat ELSE stat {$$ = new Selection_Statement_Ast($3,$5,$7, yylinen);}
       								| 	WHILE '(' condition ')' stat {$$ = new Iteration_Statement_Ast($3,$5,yylinen,false);}
       								| 	DO stat WHILE '(' condition ')' ';' {$$ = new Iteration_Statement_Ast($5,$2,yylinen,true);}
       								|	stat {$$ = $1;}
       								| 	PRINT variable ';'{$$ = new Print_Ast($2,yylinen);}
;

stat                                : statement {$$ = $1;}
									| '{' stat '}' {$$ = $2;}
									| '{' statement '}' {$$ = $2;}
									| '{' statement statement_list '}' {Sequence_Ast * p = new Sequence_Ast(yylinen);
															 p->ast_push_back($2); 
															 for (std::list<Ast *>::iterator it=(*$3).begin(); it != (*$3).end(); ++it){
																Ast * x = *it;
																p->ast_push_back(x);
															 }
															 $$ = p;
															 }

condition : exp GREATER_THAN exp {$$ = new Relational_Expr_Ast($1,greater_than,$3, yylinen);
								if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Relop statement data type not compatible");}
		  | exp LESS_THAN exp {$$ = new Relational_Expr_Ast($1,less_than,$3, yylinen);
		  if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Relop statement data type not compatible");}
		  | exp GREATER_THAN_EQUAL exp {$$ = new Relational_Expr_Ast($1,greater_equalto,$3, yylinen);
		  if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Relop statement data type not compatible");}
		  | exp LESS_THAN_EQUAL exp {$$ = new Relational_Expr_Ast($1,less_equalto,$3, yylinen);
		  if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Relop statement data type not compatible");}
		  | exp EQUAL exp {$$ = new Relational_Expr_Ast($1,equalto,$3, yylinen);
		  if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Relop statement data type not compatible");}
		  | exp NEQ exp {$$ = new Relational_Expr_Ast($1,not_equalto,$3, yylinen);
		  if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Relop statement data type not compatible");}
		  | '(' condition ')' {$$ = $2;}
		  | condition AND condition {$$ = new Logical_Expr_Ast($1,_logical_and,$3, yylinen);}
		  | condition OR condition {$$ = new Logical_Expr_Ast($1,_logical_or,$3, yylinen);}
		  | NOT condition {$$ = new Logical_Expr_Ast(NULL,_logical_not,$2, yylinen);}
;

assignment_statement				:	variable ASSIGN exp ';' 
										{if ($1->get_data_type() != $3->get_data_type())
											yyerror("Error: Assignment statement data type not compatible"); 
											$$ = new Assignment_Ast($1, $3, yylinen);}
;

exp 						: 	variable { $$ = $1; }
							|  	constant { $$ = $1; }
							|  	'-' exp %prec UMINUS { $$ = new UMinus_Ast($2,NULL,yylinen); $$->set_data_type($2->get_data_type()); }
							|	exp '+' exp {$$ = new Plus_Ast($1,$3,yylinen); 
								if(($1->get_data_type() == double_data_type) || ($3->get_data_type() == double_data_type))
									$$->set_data_type(double_data_type);
								else
									$$->set_data_type(int_data_type);
								if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Arith statement data type not compatible"); 
								}
							|	exp '-' exp {$$ = new Minus_Ast($1,$3,yylinen);
								if(($1->get_data_type() == double_data_type) || ($3->get_data_type() == double_data_type))
									$$->set_data_type(double_data_type);
								else
									$$->set_data_type(int_data_type);
								if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Arith statement data type not compatible");
								}
							| 	exp '*' exp {$$ = new Mult_Ast($1,$3,yylinen);
								if(($1->get_data_type() == double_data_type) || ($3->get_data_type() == double_data_type))
									$$->set_data_type(double_data_type);
								else
									$$->set_data_type(int_data_type);
								if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Arith statement data type not compatible");
								}
							| 	exp '/' exp {$$ = new Divide_Ast($1,$3,yylinen);
								if(($1->get_data_type() == double_data_type) || ($3->get_data_type() == double_data_type))
									$$->set_data_type(double_data_type);
								else
									$$->set_data_type(int_data_type);
								if($1->get_data_type() != $3->get_data_type()) yyerror("Error: Arith statement data type not compatible");
								}
							|	'(' exp ')' {$$ = $2;}
							| 	condition '?' exp ':' exp 
							{$$ = new Conditional_Expression_Ast($1,$3,$5, yylinen);
							if($3->get_data_type() == double_data_type)
								$$->set_data_type(double_data_type);
							else
								$$->set_data_type(int_data_type);
							}
;
 
variable							:	NAME {if(s_local->variable_in_symbol_list_check(*$1))
												{$$ = new Name_Ast(*$1,s_local->get_symbol_table_entry(*$1),yylinen);}
											else
												{$$ = new Name_Ast(*$1,s->get_symbol_table_entry(*$1),yylinen);}}
;

constant							:	NUM {$$ = new Number_Ast<int>($1,int_data_type,yylinen);} 
                                    | FNUM {$$ = new Number_Ast<double>($1,double_data_type,yylinen);}
;

%%
int yyerror (char * mesg){
fprintf(stderr, "cs316: Error %s . line : %d\n", mesg, yylinen);
}
