template class Number_Ast<double>;
template class Number_Ast<int>;
 
Ast::Ast(){
	
}

Ast::~Ast(){
	
}

Data_Type Ast::get_data_type(){
	return node_data_type;
}

void Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Ast::is_value_zero(){
	return false;
}

bool Ast::check_ast(){
	return true;
}

 Symbol_Table_Entry & Ast::get_symbol_entry(){
 	Symbol_Table_Entry * s = NULL;
 	return *s;
 }

Print_Ast::Print_Ast(Ast *v, int line){
	var = v;
	lineno = line;
	ast_num_child = unary_arity;
}

void Print_Ast::print(ostream & file_buffer){
	file_buffer << "\n         Print:";
	file_buffer << "\n            Var ";
	var->print(file_buffer);
}








Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
	lhs = temp_lhs;
	rhs = temp_rhs;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = temp_lhs->get_data_type();
}

bool Assignment_Ast::check_ast(){
	if(lhs->get_data_type() != rhs->get_data_type()){
		return false;
	}
	if(lhs == NULL or rhs == NULL){
		return false;
	}
}

void Assignment_Ast::print(ostream & file_buffer){
	file_buffer << "\n         Asgn:";
	file_buffer << "\n            LHS ";
	lhs->print(file_buffer);
	file_buffer << "\n            RHS ";
	rhs->print(file_buffer);
	//file_buffer << "]\n" ;
}

bool Arithmetic_Expr_Ast::check_ast(){
	if(lhs->get_data_type() != rhs->get_data_type()){
		return false;
	}
	if(rhs == NULL) return false;
	else return true;
}

Data_Type Arithmetic_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
	node_data_type = var_entry.get_data_type();
	variable_symbol_entry = & var_entry;
	lineno = line;
	ast_num_child = unary_arity;
}

Data_Type Name_Ast::get_data_type(){
	return node_data_type;
}

void Name_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
	return *variable_symbol_entry;
}

void Name_Ast::print(ostream & file_buffer){
	file_buffer << "(Name : ";
	file_buffer << variable_symbol_entry->get_variable_name();
	file_buffer << ")" ;
}

template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
	node_data_type = constant_data_type;
	constant = number;
	lineno = line;
	ast_num_child = unary_arity;
}

template <class T>
Data_Type Number_Ast<T>::get_data_type(){
	return node_data_type;
}

template <class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
	node_data_type = dt;
}

template <class T>
bool Number_Ast<T>::is_value_zero(){
	if (constant == 0) return true;
	else return false;
}

template <class T>
void Number_Ast<T>::print(ostream & file_buffer){
	file_buffer << "(Num : ";
	file_buffer << constant ;
	file_buffer << ")" ;
}

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Plus_Ast::print(ostream & file_buffer){
	file_buffer << "(\n            Arith: PLUS\n";
	file_buffer << "               LHS ";
	lhs->print(file_buffer);
	file_buffer << "\n               RHS ";
	rhs->print(file_buffer);
	file_buffer << ")" ;
}

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Minus_Ast::print(ostream & file_buffer){
	file_buffer << "(\n            Arith: MINUS\n";
	file_buffer << "               LHS ";
	lhs->print(file_buffer);
	file_buffer << "\n               RHS ";
	rhs->print(file_buffer);
	file_buffer << ")" ;
}

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Divide_Ast::print(ostream & file_buffer){
	file_buffer << "(\n            Arith: DIV\n";
	file_buffer << "               LHS ";
	lhs->print(file_buffer);
	file_buffer << "\n               RHS ";
	rhs->print(file_buffer);
	file_buffer << ")" ;
}

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	node_data_type = l->get_data_type();
}

void Mult_Ast::print(ostream & file_buffer){
	file_buffer << "(\n            Arith: MULT\n";
	file_buffer << "               LHS ";
	lhs->print(file_buffer);
	file_buffer << "\n               RHS ";
	rhs->print(file_buffer);
	file_buffer << ")" ;
}

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = unary_arity;
	node_data_type = l->get_data_type();
}

void UMinus_Ast::print(ostream & file_buffer){
	file_buffer << "(\n            Arith: UMINUS\n";
    file_buffer << "               LHS ";
    lhs->print(file_buffer);
	file_buffer << ")" ;
}

Return_Ast::Return_Ast(int line){
	lineno = line;
}

void Return_Ast::print(ostream & file_buffer){
	file_buffer << "Return_Ast\n";
}

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	this -> cond = cond;
	node_data_type =r->get_data_type();
	//printf("Ast cc constructor\n");
}

void Conditional_Expression_Ast::print(ostream & file_buffer){
	file_buffer << "(\n         Cond: ";
	file_buffer << "\n            IF_ELSE ";
	cond->print(file_buffer);
	file_buffer << "\n            LHS ";
	lhs->print(file_buffer);
	file_buffer << "\n            RHS ";
	rhs->print(file_buffer);
	file_buffer << ")" ;

}

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
	lhs_condition = lhs;
	rhs_condition = rhs;
	rel_op = rop;
	lineno = line;
	node_data_type = rhs->get_data_type();
}

Data_Type Relational_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast(){

	if (lhs_condition != NULL and rhs_condition != NULL)
		if(lhs_condition->get_data_type() == rhs_condition->get_data_type()){
			return true;
		}
		else{
			cout<<"Error: Incompatible Data_Type";
			return false;

		}
	else //need to be modified for type checking
		return false;
}

void Relational_Expr_Ast::print(ostream & file_buffer){
	file_buffer << "\n            Condition: ";
	if(rel_op == less_equalto){file_buffer << "LE";}
	else if(rel_op == greater_equalto){file_buffer << "GE";}
	else if(rel_op == less_than){file_buffer << "LT";}
	else if(rel_op == greater_than){file_buffer << "GT";}
	else if(rel_op == equalto){file_buffer << "EQ";}
	else if(rel_op == not_equalto){file_buffer << "NE";}
	file_buffer << "\n               LHS ";
	lhs_condition->print(file_buffer);
	file_buffer << "\n               RHS ";
	rhs_condition->print(file_buffer);
	// file_buffer << ")" ;
}

Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
	lhs_op = lhs;
	rhs_op = rhs;
	bool_op = bop;
	lineno = line;
	node_data_type = void_data_type;
}

Data_Type Logical_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast(){
	if (rhs_op != NULL)
		return true;
	else //need to be modified for type checking
		return false;
}

void Logical_Expr_Ast::print(ostream & file_buffer){
	file_buffer << "\n            Condition: ";
	if(bool_op == _logical_not){file_buffer << "NOT";}
	else if(bool_op == _logical_or){file_buffer << "OR";}
	else if(bool_op == _logical_and){file_buffer << "AND";}
	if(lhs_op != NULL){
		file_buffer << "\n               LHS (";
		lhs_op->print(file_buffer);
		file_buffer << ")" ;
	}
	file_buffer << "\n               RHS (";
	rhs_op->print(file_buffer);
	file_buffer << ")" ;
}

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line){
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	lineno = line;
	node_data_type = void_data_type;
}

Data_Type Selection_Statement_Ast::get_data_type(){
	return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast(){
	if (then_part != NULL)
		return true;
	else
		return false;
}

void Selection_Statement_Ast::print(ostream & file_buffer){
	file_buffer << "\n         IF : ";
	file_buffer << "\n         CONDITION (";
	cond->print(file_buffer) ;
	if(else_part==NULL)
		file_buffer << ")\n         THEN (";
	else
		file_buffer << ")\n         THEN (\n";
	then_part->print(file_buffer);
	file_buffer << ")" ;
	if (else_part != NULL){
		file_buffer << "\n         ELSE (\n";
		else_part->print(file_buffer);
		file_buffer << ")" ;
	}
}

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form){
	this->cond = cond;
	this->body = body;
	is_do_form = do_form;
	lineno = line;
	node_data_type = void_data_type;
}

Data_Type Iteration_Statement_Ast::get_data_type(){
	return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast(){
	if (body != NULL)
		return true;
	else
		return false;
}

void Iteration_Statement_Ast::print(ostream & file_buffer){
	if(is_do_form==false){
		file_buffer << "\n         WHILE: " ;
		file_buffer << "\n         CONDITION (";
		cond->print(file_buffer) ;
		file_buffer << ")";
		file_buffer << "\n         BODY (";
		body->print(file_buffer);
		file_buffer << ")";
	}else{
		file_buffer << "\n         DO (" ;
		body->print(file_buffer);
		file_buffer << ")";
		file_buffer << "\n         WHILE CONDITION (";
		cond->print(file_buffer) ;
		file_buffer << ")";
		
	}

}

Sequence_Ast::Sequence_Ast(int line){
	lineno = line;
}

void Sequence_Ast::ast_push_back(Ast * ast){
	statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer){
	for (std::list<Ast *>::iterator it=statement_list.begin(); it != statement_list.end(); ++it){
		Ast * p = *it;
		file_buffer << "\n";
		p->print(file_buffer);
		
	}
}