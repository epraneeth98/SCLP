template class Number_Ast<double>;
template class Number_Ast<int>;
 

Code_For_Ast & Assignment_Ast::compile(){
	Code_For_Ast lhs_code = rhs->compile();
	Code_For_Ast * cfa = new Code_For_Ast(lhs_code.get_icode_list(), NULL);
	Code_For_Ast rhs_code = lhs->create_store_stmt(lhs_code.get_reg());
	Icode_Stmt * ics = rhs_code.get_icode_list().front();
	cfa->append_ics(*ics);
	return *cfa;
} 

Code_For_Ast & Print_Ast::compile(){
	if (var->get_symbol_entry().get_data_type() == int_data_type){
		list<Icode_Stmt *> dummy;

		Register_Descriptor * r = machine_desc_object.spim_register_table[v0];
		Ics_Opd * result = new Register_Addr_Opd(r);
		Ics_Opd * opd1 = new Const_Opd<int>(1);
		Icode_Stmt * ics = new Move_IC_Stmt(imm_load, opd1, result);

		dummy.push_back(ics);
		Move_IC_Stmt * M;
		
		Register_Descriptor * r1 = machine_desc_object.spim_register_table[a0];
		Register_Addr_Opd * rao = new Register_Addr_Opd(r1);

		Mem_Addr_Opd * mao = new Mem_Addr_Opd((var->get_symbol_entry()));

		M = new Move_IC_Stmt(load,mao,rao);
		dummy.push_back(M);
		Icode_Stmt * pis = new Print_IC_Stmt();
		dummy.push_back(pis);

		Code_For_Ast * cfa = new Code_For_Ast(dummy,NULL);
		return *cfa;
	}else{
		list<Icode_Stmt *> dummy;

		Register_Descriptor * r = machine_desc_object.spim_register_table[v0];
		Ics_Opd * result = new Register_Addr_Opd(r);
		Ics_Opd * opd1 = new Const_Opd<int>(3);
		Icode_Stmt * ics = new Move_IC_Stmt(imm_load, opd1, result);

		dummy.push_back(ics);
		Move_IC_Stmt * M;
		
		Register_Descriptor * r1 = machine_desc_object.spim_register_table[f12];
		Register_Addr_Opd * rao = new Register_Addr_Opd(r1);

		Mem_Addr_Opd * mao = new Mem_Addr_Opd((var->get_symbol_entry()));

		M = new Move_IC_Stmt(load_d,mao,rao);
		dummy.push_back(M);
		Icode_Stmt * pis = new Print_IC_Stmt();
		dummy.push_back(pis);

		Code_For_Ast * cfa = new Code_For_Ast(dummy,NULL);
		return *cfa;
	}
	
}

Code_For_Ast & Name_Ast::compile(){
	Mem_Addr_Opd * mao = new Mem_Addr_Opd(*variable_symbol_entry);
	Register_Descriptor * rd;
	if (variable_symbol_entry->get_data_type() == int_data_type)
		rd = machine_desc_object.get_new_register<int_reg>();
	else
		rd = machine_desc_object.get_new_register<float_reg>();
	Register_Addr_Opd * rao = new Register_Addr_Opd(rd);
	Move_IC_Stmt * M;
	if (variable_symbol_entry->get_data_type() == int_data_type)
		M = new Move_IC_Stmt(load,mao,rao);
	else
		M = new Move_IC_Stmt(load_d,mao,rao);
	list<Icode_Stmt *> ics_list;
	ics_list.push_back(M);
	Code_For_Ast * C = new Code_For_Ast(ics_list,rd);
	return *C;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){
	Ics_Opd * opd1 = new Register_Addr_Opd(store_register);
	Ics_Opd * location = new  Mem_Addr_Opd(*variable_symbol_entry);
	Icode_Stmt * ics;
	if (variable_symbol_entry->get_data_type() == int_data_type)
		ics = new Move_IC_Stmt(store, location, opd1);
	else
		ics = new Move_IC_Stmt(store_d, location, opd1);
	Code_For_Ast * cfa = new Code_For_Ast();
	cfa->set_reg(NULL);
	cfa->append_ics(*ics);
	store_register->reset_register_occupied();
	return *cfa;
}

template <>
Code_For_Ast & Number_Ast<int>::compile(){
	Register_Descriptor * r = machine_desc_object.get_new_register<int_reg>();
	Ics_Opd * result = new Register_Addr_Opd(r);
	Ics_Opd * opd1 = new Const_Opd<int>(constant);
	Icode_Stmt * ics = new Move_IC_Stmt(imm_load, opd1, result);
	Code_For_Ast * cfa = new Code_For_Ast();
	cfa->set_reg(r);
	cfa->append_ics(*ics);
	return *cfa;
}

template <>
Code_For_Ast & Number_Ast<double>::compile(){
	Register_Descriptor * r = machine_desc_object.get_new_register<float_reg>();
	Ics_Opd * result = new Register_Addr_Opd(r);
	Ics_Opd * opd1 = new Const_Opd<double>(constant);
	Icode_Stmt * ics = new Move_IC_Stmt(imm_load_d, opd1, result);
	Code_For_Ast * cfa = new Code_For_Ast();
	cfa->set_reg(r);
	cfa->append_ics(*ics);
	return *cfa;
}

Code_For_Ast & Plus_Ast::compile(){
	
	Code_For_Ast ll = lhs->compile();
	Code_For_Ast rr = rhs->compile();
	Register_Descriptor * r;
	if (node_data_type == int_data_type)
		r = machine_desc_object.get_new_register<int_reg>();
	else
		r = machine_desc_object.get_new_register<float_reg>();
	Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
	Register_Addr_Opd * rao = new Register_Addr_Opd(rr.get_reg());
	Register_Addr_Opd * fff = new Register_Addr_Opd(r);
	list<Icode_Stmt *> list_l = rr.get_icode_list();
	list<Icode_Stmt *> list_r = ll.get_icode_list();
	list_l.merge(list_r);
	Compute_IC_Stmt * x;
	if (node_data_type == int_data_type)
		x = new Compute_IC_Stmt(add,lao,rao,fff);
	else
		x = new Compute_IC_Stmt(add_d,lao,rao,fff);
	list_l.push_back(x);
	Code_For_Ast * cfa = new Code_For_Ast(list_l,r);


	rr.get_reg()->reset_use_for_expr_result();
	ll.get_reg()->reset_use_for_expr_result();
	return * cfa;
}

Code_For_Ast & Minus_Ast::compile(){
	Code_For_Ast ll = lhs->compile();
	Code_For_Ast rr = rhs->compile();
	Register_Descriptor * r;
	if (node_data_type == int_data_type)
		r = machine_desc_object.get_new_register<int_reg>();
	else
		r = machine_desc_object.get_new_register<float_reg>();
	Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
	Register_Addr_Opd * rao = new Register_Addr_Opd(rr.get_reg());
	Register_Addr_Opd * fff = new Register_Addr_Opd(r);
	list<Icode_Stmt *> list_l = rr.get_icode_list();
	list<Icode_Stmt *> list_r = ll.get_icode_list();
	list_l.merge(list_r);
	Compute_IC_Stmt * x;
	if (node_data_type == int_data_type)
		x = new Compute_IC_Stmt(sub,lao,rao,fff);
	else
		x = new Compute_IC_Stmt(sub_d,lao,rao,fff);
	list_l.push_back(x);
	Code_For_Ast * cfa = new Code_For_Ast(list_l,r);


	rr.get_reg()->reset_use_for_expr_result();
	ll.get_reg()->reset_use_for_expr_result();
	return * cfa;
}


Code_For_Ast & Divide_Ast::compile(){
	Code_For_Ast ll = lhs->compile();
	Code_For_Ast rr = rhs->compile();
	Register_Descriptor * r;
	if (node_data_type == int_data_type)
		r = machine_desc_object.get_new_register<int_reg>();
	else
		r = machine_desc_object.get_new_register<float_reg>();
	Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
	Register_Addr_Opd * rao = new Register_Addr_Opd(rr.get_reg());
	Register_Addr_Opd * fff = new Register_Addr_Opd(r);
	list<Icode_Stmt *> list_l = rr.get_icode_list();
	list<Icode_Stmt *> list_r = ll.get_icode_list();
	list_l.merge(list_r);
	Compute_IC_Stmt * x;
	if (node_data_type == int_data_type)
		x = new Compute_IC_Stmt(divd,lao,rao,fff);
	else
		x = new Compute_IC_Stmt(div_d,lao,rao,fff);
	list_l.push_back(x);
	Code_For_Ast * cfa = new Code_For_Ast(list_l,r);


	rr.get_reg()->reset_use_for_expr_result();
	ll.get_reg()->reset_use_for_expr_result();
	return * cfa;
}

Code_For_Ast & Mult_Ast::compile(){
	Code_For_Ast ll = lhs->compile();
	Code_For_Ast rr = rhs->compile();
	Register_Descriptor * r;
	if (node_data_type == int_data_type)
		r = machine_desc_object.get_new_register<int_reg>();
	else
		r = machine_desc_object.get_new_register<float_reg>();
	Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
	Register_Addr_Opd * rao = new Register_Addr_Opd(rr.get_reg());
	Register_Addr_Opd * fff = new Register_Addr_Opd(r);
	list<Icode_Stmt *> list_l = rr.get_icode_list();
	list<Icode_Stmt *> list_r = ll.get_icode_list();
	list_l.merge(list_r);
	Compute_IC_Stmt * x;
	if (node_data_type == int_data_type)
		x = new Compute_IC_Stmt(mult,lao,rao,fff);
	else
		x = new Compute_IC_Stmt(mult_d,lao,rao,fff);
	list_l.push_back(x);
	Code_For_Ast * cfa = new Code_For_Ast(list_l,r);


	rr.get_reg()->reset_use_for_expr_result();
	ll.get_reg()->reset_use_for_expr_result();
	return * cfa;
}


Code_For_Ast & UMinus_Ast::compile(){
	Code_For_Ast ll = lhs->compile();
	// Code_For_Ast rr = rhs->compile();
	Register_Descriptor * r;
	if (node_data_type == int_data_type)
		r = machine_desc_object.get_new_register<int_reg>();
	else
		r = machine_desc_object.get_new_register<float_reg>();
	Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
	Register_Addr_Opd * rao = NULL;
	Register_Addr_Opd * fff = new Register_Addr_Opd(r);
	// list<Icode_Stmt *> list_l = rr.get_icode_list();
	list<Icode_Stmt *> list_r = ll.get_icode_list();
	// list_l.merge(list_r);
	Compute_IC_Stmt * x;
	if (node_data_type == int_data_type)
		x = new Compute_IC_Stmt(uminus,lao,rao,fff);
	else
		x = new Compute_IC_Stmt(uminus_d,lao,rao,fff);
	list_r.push_back(x);
	Code_For_Ast * cfa = new Code_For_Ast(list_r,r);


	// rr.get_reg()->reset_use_for_expr_result();
	ll.get_reg()->reset_use_for_expr_result();
	return * cfa;
}


Code_For_Ast & Return_Ast::compile(){
	Code_For_Ast * cfa = new Code_For_Ast();
	return *cfa;
}

///////////////////////////////// END of LEVEL 2 /////////////////////////////

Code_For_Ast & Conditional_Expression_Ast::compile(){

	Code_For_Ast condition = cond->compile();
	//printf("done cond\n");
	list<Icode_Stmt *> dummy1 = condition.get_icode_list();
	//printf("done dummy1\n");
	Register_Addr_Opd * opd = new Register_Addr_Opd(condition.get_reg());
	string s = get_new_label();
	Icode_Stmt * ics = new Control_Flow_IC_Stmt(beq,opd,s);

	dummy1.push_back(ics);
	condition.get_reg()->reset_register_occupied();
	Code_For_Ast then = lhs->compile(); // combine code for dummy and then part
	//printf("done then\n");
	list<Icode_Stmt *> x; // = then.get_icode_list().merge(dummy1.get_icode_list());
	x.merge(then.get_icode_list());
	x.merge(dummy1);
	list<Icode_Stmt *> dummy2 = x;

	Register_Descriptor * rr;
	if (node_data_type == int_data_type)
		rr = machine_desc_object.get_new_register<int_reg>();
	else
		rr = machine_desc_object.get_new_register<float_reg>();
	Register_Addr_Opd * lao = new Register_Addr_Opd(rr);

	Register_Descriptor * z = machine_desc_object.spim_register_table[zero];

	Register_Addr_Opd * zz = new Register_Addr_Opd(z);	

	Register_Addr_Opd * oo1 = new Register_Addr_Opd(then.get_reg());

	Icode_Stmt * icss = new Compute_IC_Stmt(or_t,oo1,zz,lao);
	dummy2.push_back(icss);

	then.get_reg()->reset_register_occupied();

	string ss = get_new_label();
	Icode_Stmt * ics2 = new Label_IC_Stmt(j,ss);
	dummy2.push_back(ics2);
		
	Icode_Stmt * ics4 = new Label_IC_Stmt(label,s);
	dummy2.push_back(ics4);
	Code_For_Ast elsepart = rhs->compile(); // combine code for dummy and then part
	
	list<Icode_Stmt *> y; // = then.get_icode_list().merge(dummy1.get_icode_list());
	y.merge(elsepart.get_icode_list());
	y.merge(dummy2);
		
	list<Icode_Stmt *> dummy3 = y;

	Register_Addr_Opd * oo2 = new Register_Addr_Opd(elsepart.get_reg());

	Icode_Stmt * icss2 = new Compute_IC_Stmt(or_t,oo2,zz,lao);
	dummy3.push_back(icss);

	Icode_Stmt * ics3 = new Label_IC_Stmt(label,ss);
	dummy3.push_back(ics3);
	Code_For_Ast * cfa = new Code_For_Ast(dummy3,rr);
	
	elsepart.get_reg()->reset_register_occupied();
	return *cfa;
	
}

Code_For_Ast & Relational_Expr_Ast::compile(){
	//printf("entered Relational_Expr_Ast\n");
	Code_For_Ast ll = lhs_condition->compile();
	Code_For_Ast rr = rhs_condition->compile();
	Register_Descriptor * r = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
	Register_Addr_Opd * rao = new Register_Addr_Opd(rr.get_reg());
	Register_Addr_Opd * fff = new Register_Addr_Opd(r);
	list<Icode_Stmt *> list_l = rr.get_icode_list();
	list<Icode_Stmt *> list_r = ll.get_icode_list();
	list_l.merge(list_r);
	Compute_IC_Stmt * x;
	if(rel_op == less_equalto){
		if(ll.get_reg()->get_use_category() == int_reg )
			x = new Compute_IC_Stmt(sle,lao,rao,fff);
		else
			x = new Compute_IC_Stmt(sle_d,lao,rao,fff);
	}
	else if(rel_op == less_than){
		if(ll.get_reg()->get_use_category() == int_reg )
			x = new Compute_IC_Stmt(slt,lao,rao,fff);
		else
			x = new Compute_IC_Stmt(slt_d,lao,rao,fff);
	}
	else if(rel_op  == greater_than){
		if(ll.get_reg()->get_use_category() == int_reg )
			x = new Compute_IC_Stmt(sgt,lao,rao,fff);
		else
			x = new Compute_IC_Stmt(sgt_d,lao,rao,fff);
	}
	else if(rel_op == greater_equalto){
		if(ll.get_reg()->get_use_category() == int_reg )
			x = new Compute_IC_Stmt(sge,lao,rao,fff);
		else
			x = new Compute_IC_Stmt(sge_d,lao,rao,fff);
	}
	else if(rel_op == equalto){
		if(ll.get_reg()->get_use_category() == int_reg )
			x = new Compute_IC_Stmt(seq,lao,rao,fff);
		else
			x = new Compute_IC_Stmt(seq_d,lao,rao,fff);
	}
	else if(rel_op == not_equalto){
		if(ll.get_reg()->get_use_category() == int_reg )
			x = new Compute_IC_Stmt(sne,lao,rao,fff);
		else
			x = new Compute_IC_Stmt(sne_d,lao,rao,fff);
	}
	list_l.push_back(x);
	Code_For_Ast * cfa = new Code_For_Ast(list_l,r);


	rr.get_reg()->reset_use_for_expr_result();
	ll.get_reg()->reset_use_for_expr_result();
	return * cfa;
}

Code_For_Ast & Logical_Expr_Ast::compile(){
	if(bool_op != _logical_not){
	////printf("here 1\n");		
		Code_For_Ast ll = lhs_op->compile();
		Code_For_Ast rr = rhs_op->compile();
		Register_Descriptor * r;
		r = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
		Register_Addr_Opd * rao = new Register_Addr_Opd(rr.get_reg());
		Register_Addr_Opd * fff = new Register_Addr_Opd(r);

	//	//printf("here 2\n");

		list<Icode_Stmt *> list_l = rr.get_icode_list();
		list<Icode_Stmt *> list_r = ll.get_icode_list();
		
	//	//printf("here 3\n");

		list_l.merge(list_r);
		Compute_IC_Stmt * x;
		if (bool_op == _logical_and){

	//		//printf("and logical\n");

			x = new Compute_IC_Stmt(and_t,lao,rao,fff);
		}else if(bool_op == _logical_or){
	//		//printf("or logical\n");
			x = new Compute_IC_Stmt(or_t,lao,rao,fff);
		}
	//	//printf("here 4\n");
		list_l.push_back(x);
		Code_For_Ast * cfa = new Code_For_Ast(list_l,r);
	//	//printf("here 5\n");
		rr.get_reg()->reset_use_for_expr_result();
		ll.get_reg()->reset_use_for_expr_result();
	//	//printf("here 6\n");
		return * cfa;
	}else{
		//Code_For_Ast ll = lhs_op->compile();
		////printf("here 1\n");
		Code_For_Ast rr = rhs_op->compile();
		Register_Descriptor * r;
		////printf("here 2\n");
		r = machine_desc_object.get_new_register<int_reg>();
		//Register_Addr_Opd * lao = new Register_Addr_Opd(ll.get_reg());
		Register_Addr_Opd * rao = new Register_Addr_Opd(rr.get_reg());
		////printf("here 3\n");
		Register_Addr_Opd * fff = new Register_Addr_Opd(r);
		list<Icode_Stmt *> list_l = rr.get_icode_list();
		////printf("here 4\n");
		//list<Icode_Stmt *> list_r = ll.get_icode_list();
		//list_l.merge(list_r);
		Const_Opd <int> * k = new Const_Opd<int>(1);
		Icode_Stmt * ics = new Move_IC_Stmt(imm_load, k, fff);

		Register_Descriptor * r1;
		r1 = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd * result = new Register_Addr_Opd(r1);


		list_l.push_back(ics);

		Compute_IC_Stmt * x;
		x = new Compute_IC_Stmt(not_t,rao,fff,result);
		////printf("here 5\n");
		list_l.push_back(x);
		Code_For_Ast * cfa = new Code_For_Ast(list_l,r1);
		rr.get_reg()->reset_use_for_expr_result();
		r->reset_use_for_expr_result();

		////printf("here 6\n");
		//ll.get_reg()->reset_use_for_expr_result();
		return * cfa;
	}
}

Code_For_Ast & Selection_Statement_Ast::compile(){
	Code_For_Ast condition = cond->compile();
	//printf("done cond\n");
	list<Icode_Stmt *> dummy1 = condition.get_icode_list();
	//printf("done dummy1\n");
	Register_Addr_Opd * opd = new Register_Addr_Opd(condition.get_reg());
	string s = get_new_label();
	Icode_Stmt * ics = new Control_Flow_IC_Stmt(beq,opd,s);
	dummy1.push_back(ics);
	condition.get_reg()->reset_register_occupied();
	Code_For_Ast then = then_part->compile(); // combine code for dummy and then part
	//printf("done then\n");
	list<Icode_Stmt *> x; // = then.get_icode_list().merge(dummy1.get_icode_list());
	x.merge(then.get_icode_list());
	x.merge(dummy1);
	list<Icode_Stmt *> dummy2 = x;
	// string ss = get_new_label();
	// Icode_Stmt * ics2 = new Label_IC_Stmt(j,ss);
	// dummy2.push_back(ics2);
	if(else_part != NULL){
		Icode_Stmt * ics4 = new Label_IC_Stmt(label,s);
		dummy2.push_back(ics4);
		Code_For_Ast elsepart = else_part->compile(); // combine code for dummy and then part
		
		list<Icode_Stmt *> y; // = then.get_icode_list().merge(dummy1.get_icode_list());
		y.merge(elsepart.get_icode_list());
		y.merge(dummy2);
		
		list<Icode_Stmt *> dummy3 = y;
		Icode_Stmt * ics3 = new Label_IC_Stmt(label,s);
		//dummy3.push_back(ics3);
		Code_For_Ast * cfa = new Code_For_Ast(dummy3,NULL);
		return *cfa;
	}
	Icode_Stmt * ics3 = new Label_IC_Stmt(label,s);
	dummy2.push_back(ics3);
	Code_For_Ast * cfa = new Code_For_Ast(dummy2,NULL);
	return *cfa;

}

Code_For_Ast & Iteration_Statement_Ast::compile(){
	string label2 = get_new_label();
	string label1 = get_new_label();
	Code_For_Ast condition = cond->compile();
	//printf("done cond\n");
	list<Icode_Stmt *> dummy1 = condition.get_icode_list();
	//printf("done dummy1\n");
	Icode_Stmt * lab1 = new Label_IC_Stmt(label,label1);
	dummy1.push_front(lab1); // cond part
	Register_Addr_Opd * opd = new Register_Addr_Opd(condition.get_reg());
	Icode_Stmt * ics = new Control_Flow_IC_Stmt(bne,opd,label2);
	dummy1.push_back(ics);
	condition.get_reg()->reset_register_occupied();

	Code_For_Ast then = body->compile(); // combine code for dummy and then part
	//printf("body done\n");
	
	//then.get_icode_list();

	list<Icode_Stmt *> dummy2 = then.get_icode_list();

	Icode_Stmt * lab2 = new Label_IC_Stmt(label,label2);
	dummy2.push_front(lab2); // cond part

	if (!is_do_form){
		Icode_Stmt * ics22 = new Label_IC_Stmt(j,label1);
		dummy2.push_front(ics22);
	}

	list<Icode_Stmt *> dummy3 = dummy1;
	list<Icode_Stmt *> dummy4 = dummy2;	
	
	dummy4.insert(dummy4.end(),dummy3.begin(),dummy3.end());
	
	Code_For_Ast * cfa = new Code_For_Ast(dummy4,NULL);
	return *cfa;

}

Code_For_Ast & Sequence_Ast::compile(){
	list<Icode_Stmt *> x;
	for (std::list<Ast *>::iterator it=statement_list.begin(); it != statement_list.end(); ++it)
	{
		Code_For_Ast rr = (*it)->compile();
		x.insert(x.end(),rr.get_icode_list().begin(),rr.get_icode_list().end());
		//x.merge(rr.get_icode_list());
	}
	Code_For_Ast * z = new Code_For_Ast(x,NULL);
	return *z;
}

///////////////////////////////// END OF LEVEL 3 ////////////////////////////////

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

template <class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}


Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){}
