template class Const_Opd<double>;
template class Const_Opd<int>;

Instruction_Descriptor::Instruction_Descriptor(Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	this->inst_op = op;
	this->mnemonic = mnn;
	this->ic_symbol = ics;       /* symbol for printing in intermediate code */
	this->name = name;
	this->ic_format = icf; /* format for printing in intemediate code */
	this->assem_format = af;
} 

Instruction_Descriptor::Instruction_Descriptor(){}

Tgt_Op Instruction_Descriptor::get_op(){
	return this->inst_op;
}

string Instruction_Descriptor::get_name(){
	return this->name;
}

string Instruction_Descriptor::get_mnemonic(){
	return this->mnemonic;
}
	
string Instruction_Descriptor::get_ic_symbol(){
	return this->ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format(){
	return this->ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format(){
	return this->assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){
	// file_buffer << this->name;
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	this->symbol_entry = &se;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
	if(symbol_entry->get_symbol_scope() == local){
		file_buffer<<symbol_entry->get_start_offset()<<"($fp)";
	}
	else{
		file_buffer<<symbol_entry->get_variable_name();
	}
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs){
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////
	
Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	register_description = rd;
}

Register_Descriptor * Register_Addr_Opd::get_reg(){
	return register_description;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer << register_description->get_name() ;
}
	
void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
	file_buffer << "$" << register_description->get_name() ;
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){
	// this = new Register_Addr_Opd(rhs.get_reg())
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
Const_Opd<T>::Const_Opd(T num){
	this->num = num;
}
	
template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){
	file_buffer<<num;
}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){
	file_buffer<<num;
}

template <class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd<T> & rhs){
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){
	this->op_desc = *machine_desc_object.spim_instruction_table[inst_op];
	this->opd1 = opd1;
	this->result = result;
}

Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
	return opd1;
}

void Move_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
	return this->result;
}

void Move_IC_Stmt::set_result(Ics_Opd * io){
	this->result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer << "	" << op_desc.get_name() << ":    	";
	if(op_desc.get_assembly_format() == a_op_o1_r)
	{
		this->opd1->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		this->result->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		this->result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		this->opd1->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer){
	
	// for load and store
	file_buffer << "	" << op_desc.get_mnemonic() << " ";
	this->result->print_asm_opd(file_buffer);
	file_buffer << ", ";
	this->opd1->print_asm_opd(file_buffer);
	file_buffer << "\n";
}

/////////////////////////////////////////////////////////////////////////////////////////////

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result){
	this->op_desc = *machine_desc_object.spim_instruction_table[inst_op];
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->result = result;
}
	 
Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs){
	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){
	return opd1;
}

void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
	return opd2;
}

void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
	this->opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
	return result;
}

void Compute_IC_Stmt::set_result(Ics_Opd * io){
	this->result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
	if (op_desc.get_op() == uminus or op_desc.get_op() == uminus_d){
		file_buffer << "	" << op_desc.get_name() << ":    	";
		this->result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		this->opd1->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		file_buffer << "	" << op_desc.get_name() << ":    	";
		this->result->print_ics_opd(file_buffer);
		file_buffer << " <- ";
		this->opd1->print_ics_opd(file_buffer);
		file_buffer << ", ";
		this->opd2->print_ics_opd(file_buffer);
		file_buffer << "\n";
	}
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
	if (op_desc.get_op() == uminus or op_desc.get_op() == uminus_d){
		file_buffer <<  "	" << op_desc.get_mnemonic() << " ";
		this->result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		this->opd1->print_asm_opd(file_buffer);
		file_buffer << "\n";
	}
	else{
		file_buffer <<  "	" << op_desc.get_mnemonic() << " ";
		this->result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		this->opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		this->opd2->print_asm_opd(file_buffer);
		file_buffer << "\n";	
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////

Print_IC_Stmt::Print_IC_Stmt(){

}

Print_IC_Stmt::~Print_IC_Stmt(){

}

void Print_IC_Stmt::print_icode(ostream & file_buffer){ // what is this for ??
	file_buffer << "	print\n" ;
}

void Print_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "	syscall\n" ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// for level 3

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, string label){
	this->op_desc = *machine_desc_object.spim_instruction_table[inst_op];
	this->opd1 = opd1;
	this->label = label;
} 

Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){
	return *this;
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){
	return opd1;
}

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

string Control_Flow_IC_Stmt::get_label(){
	return label;
}

void Control_Flow_IC_Stmt::set_label(string label){
	this->label = label;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer << "	" << op_desc.get_name() << ":    	";
		this->opd1->print_ics_opd(file_buffer);
		file_buffer << ", zero : goto ";
		file_buffer << label;
		file_buffer << "\n";
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "	" << op_desc.get_name() << " ";
		this->opd1->print_asm_opd(file_buffer);
		file_buffer << ", $zero, ";
		file_buffer << label;
		file_buffer << "\n";
}

////////////////////////////////////////////////////////////////////////////////////////////

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label){
	this->op_desc = *machine_desc_object.spim_instruction_table[inst_op];
	this->label = label;
} 

Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs){
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

string Label_IC_Stmt::get_label(){
	return label;
}

void Label_IC_Stmt::set_label(string label){
	this->label = label;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer){
	if (op_desc.get_op() != j){
		file_buffer << this->label << ":\n";
	}
	else{
		file_buffer << "	goto " << this->label << "\n";
	}
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer){
	if (op_desc.get_op() != j){
		file_buffer << "\n" << this->label << ":\n";
	}
	else{
		file_buffer << "	j " << this->label << "\n";
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast::Code_For_Ast(){

}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){
	this->ics_list = ic_l;
	this->result_register = reg;
}
	
void Code_For_Ast::append_ics(Icode_Stmt & ics){
	this->ics_list.push_back(&ics);
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list(){
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
	return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg){
	this->result_register = reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
	ics_list = rhs.ics_list ;
	result_register = rhs.result_register;
	// result_register = rhs.get_reg();
	return *this;
}


////////////////////////////// DUMMY /////////////////////////////


Instruction_Descriptor & Icode_Stmt::get_op(){}
Ics_Opd * Icode_Stmt::get_opd1(){}
Ics_Opd * Icode_Stmt::get_opd2(){}
Ics_Opd * Icode_Stmt::get_result(){}

void Icode_Stmt::set_opd1(Ics_Opd * io){}
void Icode_Stmt::set_opd2(Ics_Opd * io){}
void Icode_Stmt::set_result(Ics_Opd * io){}

Register_Descriptor * Ics_Opd::get_reg(){}

























