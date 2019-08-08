int Ast::labelCounter;

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){

}
Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	Eval_Result* E = new Eval_Result_Value_Int() ; 
	return *E ; 
}
void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{

}
Eval_Result & Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result* E = new Eval_Result_Value_Int() ; 
	return *E ; 
}



Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	print(file_buffer);
	Eval_Result & res = rhs->evaluate(eval_env,file_buffer);
	lhs->set_value_of_evaluation(eval_env, res);
	lhs->print_value(eval_env,file_buffer);
}



void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
	file_buffer << "\n         " <<variable_symbol_entry->get_variable_name()<<" : ";
	string x = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(x)){
		Eval_Result* y = eval_env.get_variable_value(x);
		if(y->get_result_enum() == int_result){
			file_buffer<<y->get_int_value()<<endl<<endl;
		}else{
			file_buffer<<y->get_double_value()<<endl<<endl;
		}
	}
	else if(interpreter_global_table.does_variable_exist(x)){
		Eval_Result* y = interpreter_global_table.get_variable_value(x);
		if(y->get_result_enum() == int_result){
			file_buffer<<y->get_int_value()<<endl<<endl;
		}else{
			file_buffer<<y->get_double_value()<<endl<<endl;
		}
	}
	else{
		file_buffer<<"Error";
		exit(0);
	}
	
}
Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
	string x = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(x)){
		Eval_Result* y = eval_env.get_variable_value(x);
		return *y;
	}
	else if(interpreter_global_table.does_variable_exist(x)){
		Eval_Result* y = interpreter_global_table.get_variable_value(x);
		return *y;
	}	
	
}
void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
	string x = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(x)){
		eval_env.put_variable_value(result,x);
	}
	else if(interpreter_global_table.does_variable_exist(x)){
		interpreter_global_table.put_variable_value(result,x);
	}
	else{
		exit(0);
	}
}
Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	string x = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(x)){
		Eval_Result* y = eval_env.get_variable_value(x);
		return *y;
	}
	else if(interpreter_global_table.does_variable_exist(x)){
		Eval_Result* y = interpreter_global_table.get_variable_value(x);
		return *y;
	}
}






template<>
Eval_Result & Number_Ast<int>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int * e = new Eval_Result_Value_Int();
	e->set_value(constant);
	e-> set_variable_status(true) ;
	return *e;	
}



template<>
Eval_Result & Number_Ast<double>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Double * e = new Eval_Result_Value_Double();
	e->set_value(constant);
	e-> set_variable_status(true) ;
	return *e;	
}


Eval_Result & Arithmetic_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int* x = new Eval_Result_Value_Int();
	return *x;
}

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int* x = new Eval_Result_Value_Int() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_int_value()
						+ (rhs -> evaluate(eval_env , file_buffer)).get_int_value());
		return *x;
	}else if (node_data_type==double_data_type){
		Eval_Result_Value_Double* x = new Eval_Result_Value_Double() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_double_value()
						+ (rhs -> evaluate(eval_env , file_buffer)).get_double_value());
		return *x;
	}else{
		file_buffer<<"Error";
		exit(0);
	}
}

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int* x = new Eval_Result_Value_Int() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_int_value()
						- (rhs -> evaluate(eval_env , file_buffer)).get_int_value());
		return *x;
	}else if (node_data_type==double_data_type){
		Eval_Result_Value_Double* x = new Eval_Result_Value_Double() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_double_value()
						- (rhs -> evaluate(eval_env , file_buffer)).get_double_value());
		return *x;
	}else{
		file_buffer<<"Error";
		exit(0);
	}
}

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int* x = new Eval_Result_Value_Int() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_int_value()
						/ (rhs -> evaluate(eval_env , file_buffer)).get_int_value());
		return *x;
	}else if (node_data_type==double_data_type){
		Eval_Result_Value_Double* x = new Eval_Result_Value_Double() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_double_value()
						/ (rhs -> evaluate(eval_env , file_buffer)).get_double_value());
		return *x;
	}else{
		file_buffer<<"Error";
		exit(0);
	}
}


Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int* x = new Eval_Result_Value_Int() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_int_value()
						* (rhs -> evaluate(eval_env , file_buffer)).get_int_value());
		return *x;
	}else if (node_data_type==double_data_type){
		Eval_Result_Value_Double* x = new Eval_Result_Value_Double() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_double_value()
						* (rhs -> evaluate(eval_env , file_buffer)).get_double_value());
		return *x;
	}else{
		file_buffer<<"Error";
		exit(0);
	}
}


Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int* x = new Eval_Result_Value_Int() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_int_value() * (-1));
		return *x;
	}else if (node_data_type==double_data_type){
		Eval_Result_Value_Double* x = new Eval_Result_Value_Double() ; 
		x -> set_value((lhs -> evaluate(eval_env , file_buffer)).get_double_value() * (-1));
		return *x;
	}else{
		file_buffer<<"Error";
		exit(0);
	}
}


Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	//cond -> print(file_buffer) ; 
	// printf("hey\n");
	if((cond->evaluate(eval_env,file_buffer)).get_int_value() == 1){
		// printf(" left %f\n", lhs->evaluate(eval_env,file_buffer).get_double_value() );
		return lhs->evaluate(eval_env,file_buffer);
	}else{
		// printf(" right %f\n", rhs->evaluate(eval_env,file_buffer).get_double_value() );
		return rhs->evaluate(eval_env,file_buffer);
	}
};

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		int l_val = lhs_condition->evaluate(eval_env,file_buffer).get_int_value();
		int r_val = rhs_condition->evaluate(eval_env,file_buffer).get_int_value();
		Eval_Result_Value_Int *x = new Eval_Result_Value_Int();
		x -> set_variable_status(true) ; 
		if(rel_op == less_than){
			if(l_val < r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == greater_than){
			if(l_val > r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == less_equalto){
			if(l_val <= r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == greater_equalto){
			if(l_val >= r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == equalto){
			if(l_val == r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == not_equalto){
			if(l_val != r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}
		return *x;
	}else if(node_data_type==double_data_type){
		double l_val = lhs_condition->evaluate(eval_env,file_buffer).get_double_value();
		double r_val = rhs_condition->evaluate(eval_env,file_buffer).get_double_value();
		Eval_Result_Value_Int *x = new Eval_Result_Value_Int();
		if(rel_op == less_than){
			if(l_val < r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == greater_than){
			if(l_val > r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == less_equalto){
			if(l_val <= r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == greater_equalto){
			if(l_val >= r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == equalto){
			// printf("Heyyy\n");
			// printf("%f , %f\n", l_val , r_val );
			if(l_val == r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}else if(rel_op == not_equalto){
			if(l_val != r_val){
				x->set_value(1);
			}else{
				x->set_value(0);
			}
		}
		return *x;
	}
}

Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	int r_val = rhs_op->evaluate(eval_env,file_buffer).get_int_value();
	Eval_Result *x = new Eval_Result_Value_Int();
	if(bool_op == _logical_or){
			int l_val = lhs_op->evaluate(eval_env,file_buffer).get_int_value();

		if(l_val==1 || r_val==1){
			x->set_value(1);
		}else{
			x->set_value(0);
		}
	}else if(bool_op == _logical_and){
			int l_val = lhs_op->evaluate(eval_env,file_buffer).get_int_value();

		if(l_val==1 && r_val==1){
			x->set_value(1);
		}else{
			x->set_value(0);
		}
	}else if(bool_op == _logical_not){
		if(r_val==0){
			x->set_value(1);
		}else{
			x->set_value(0);
		}
	}
	return *x;
}

Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if((cond->evaluate(eval_env,file_buffer)).get_int_value() == 1){
		return then_part->evaluate(eval_env,file_buffer);
	}else{
		if(else_part != NULL) 
		return else_part->evaluate(eval_env,file_buffer);
	}
}

Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(is_do_form == false){
		while(cond->evaluate(eval_env,file_buffer).get_int_value()==1){
			body->evaluate(eval_env,file_buffer);
		} 
	}else{
		do{
			body->evaluate(eval_env,file_buffer);
		}while(cond->evaluate(eval_env,file_buffer).get_int_value()==1);	
		// return body->evaluate(eval_env,file_buffer); 
	}
	Eval_Result_Value_Int* x = new Eval_Result_Value_Int();
	return *x;
}

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	for(list<Ast *>::iterator it = statement_list.begin();it != statement_list.end();it++){
		(*it)->evaluate(eval_env,file_buffer);
	}
	Eval_Result_Value_Int* x = new Eval_Result_Value_Int();
	return *x;
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int* x = new Eval_Result_Value_Int();
	return *x;
}

