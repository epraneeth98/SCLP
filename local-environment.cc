void Symbol_Table::create(Local_Environment & local_global_variables_table){
	
	for (std::list<Symbol_Table_Entry *>::iterator it=variable_table.begin(); it != variable_table.end(); ++it){
		Symbol_Table_Entry * p = *it;
		if (p->get_data_type() == int_data_type){
			Eval_Result_Value_Int * eval = new Eval_Result_Value_Int();
			eval->set_result_enum(int_result);
			eval->set_value(0);
			if(p->get_symbol_scope()==local)
			eval->set_variable_status(false);
			local_global_variables_table.put_variable_value(*eval, p->get_variable_name());	
		}
		else{
			Eval_Result_Value_Double * eval = new Eval_Result_Value_Double();
			eval->set_result_enum(double_result);
			eval->set_value(0);
			if(p->get_symbol_scope()==local)
			eval->set_variable_status(false); 
			local_global_variables_table.put_variable_value(*eval, p->get_variable_name());	
		}
		
	}
}
///////////////////////////////////////////////////////////////////////
// Eval_Result class
void  Eval_Result::set_value(int number) 
{
	// do nothing
}
void  Eval_Result::set_value(double number) 
{
	// do nothing
}
int Eval_Result::get_int_value()
{
	 return 0 ;
}
double Eval_Result::get_double_value()
{
	return 0 ; 
}

bool Eval_Result::is_variable_defined()
{
	return false ;
}
void Eval_Result::set_variable_status(bool def)
{
	// do nothing
}

void Eval_Result::set_result_enum(Result_Enum res) 
{
	result_type = res ; 
}
Result_Enum Eval_Result::get_result_enum() 
{
	return result_type ; 
}

// Eval_Result_Value
void  Eval_Result_Value::set_value(int number) 
{
	// 
}
void  Eval_Result_Value::set_value(double number) 
{

}
int Eval_Result_Value::get_int_value()
{
	 return 0 ;
}
double Eval_Result_Value::get_double_value()
{
	return 0 ; 
}

bool Eval_Result_Value::is_variable_defined()
{
	return false ;
}
void Eval_Result_Value::set_variable_status(bool def)
{

}

void Eval_Result_Value::set_result_enum(Result_Enum res) 
{

}
Result_Enum Eval_Result_Value::get_result_enum() 
{
	return result_type ; 
}
///////////////////////////////////////////////////////////////////////
Local_Environment::Local_Environment(){};
Local_Environment::~Local_Environment(){};


Eval_Result_Value_Int::Eval_Result_Value_Int(){
	defined = false;
	result_type = int_result;
}

Eval_Result_Value_Int::~Eval_Result_Value_Int(){
}

Eval_Result_Value_Double::Eval_Result_Value_Double(){
	defined = false;
	result_type = double_result;
}

Eval_Result_Value_Double::~Eval_Result_Value_Double(){
}

void Eval_Result_Value_Int::set_value(int number){
	value = number;
	defined = true;
}

void Eval_Result_Value_Int::set_value(double number){
	value = number;
	defined = true;
}

int Eval_Result_Value_Int::get_int_value(){
	if(defined == true)return value;
}

void Eval_Result_Value_Int::set_variable_status(bool def){
	defined = def;
}

bool Eval_Result_Value_Int::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res){
	result_type = res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum(){
	return result_type;
}

void Eval_Result_Value_Double::set_value(int number){
	value = number;
	defined=true;
}

void Eval_Result_Value_Double::set_value(double number){
	value = number;
	defined=true;
}

double Eval_Result_Value_Double::get_double_value(){
	if(defined==true)return value;
}

void Eval_Result_Value_Double::set_variable_status(bool def){
	defined = def;
}

bool Eval_Result_Value_Double::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res){
	result_type = res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum(){
	return result_type;
}

void Local_Environment::print(ostream & file_buffer){
	for (std::map<string, Eval_Result *>::iterator it=variable_table.begin(); it!=variable_table.end(); ++it)
    {
    	if ((it->second)->get_result_enum() == double_result){
    		if ((it->second)->is_variable_defined())
    			file_buffer << VAR_SPACE << it->first << " : " << (it->second)->get_double_value() << '\n';
    		else
    			file_buffer << VAR_SPACE << it->first << " : " << "undefined" << '\n';
    	}
    	else{
    		if ((it->second)->is_variable_defined())
    			file_buffer << VAR_SPACE << it->first << " : " << (it->second)->get_int_value() << '\n';
    		else
    			file_buffer << VAR_SPACE << it->first << " : " << "undefined" << '\n';
    	}
    }
}

bool Local_Environment::is_variable_defined(string name){
	map<string, Eval_Result *>::iterator it = variable_table.find(name);
	return it->second->is_variable_defined();
}

Eval_Result * Local_Environment::get_variable_value(string name){
	map<string, Eval_Result *>::iterator it = variable_table.find(name);
	return it->second;
}

void Local_Environment::put_variable_value(Eval_Result & value, string name){
	map<string, Eval_Result *>::iterator it = variable_table.find(name);
	if (it!=variable_table.end()){
		if (value.get_result_enum() == int_result)
			it->second->set_value(value.get_int_value());
			
		else
			it->second->set_value(value.get_double_value());
		it->second->set_variable_status(true);
	}
	else
		variable_table.insert(std::pair<string, Eval_Result *>(name,&value));
}

bool Local_Environment::does_variable_exist(string name){
	map<string, Eval_Result *>::iterator it = variable_table.find(name);
	if (it!=variable_table.end())
		return true;
	else 
		return false;
}