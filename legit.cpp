#include <iostream>
#include <fstream>
#include <regex>
#include <string>

/********************************************************************
 MINI PYTHON INTERPRETER USING C++
 Developed By: LUJIA WU and Evan WU
 Course Name: COSC 4315 Programming Languages and Paradigms

 Requirements to Run:
   GNU C++ (g++)
 Command to Run the Code
   g++ minipython.cpp -o ˜/bin/mini_python
 Description:

*********************************************************************/

std::ofstream error_log("error.log");
std::string NO_SUCH_VARIABLE = "Error: Line contain unknown variable: ";
std::string INPUT_ARG_ERROR = "Error: INPUT argument is not based on template string as below:\n-o test-cases/tc01";
std::string INPUT_FILE_ERROR = "Error: Could not open the file - Line: ";
std::string SYNTAX_ERROR = "Error: Syntax error in line: ";
std::string OUTRANGE = "Error: Index out of range in line: ";
std::string DATA_TYPE_ERROR = "Error: Data type do not match in line: ";
std::string FUNC_ERROR = "Error: Contain undefined function in line: ";

bool ifFlag = false;
bool elseFlag = false;
bool defFlag = false;
bool boolresult = false;
bool funcexistFlag;

std::string words;
std::string actualCodes = "";
std::string target = "";
std::vector<std::pair<std::string, int>> variables;
std::vector<std::pair<std::string, std::vector<int>>> values;
std::vector<std::string> SIGNS = {">","<","==",">=","<=","!="};
std::vector<std::string> operations = {"+", "-", "*", "/"};

std::string buffer;
std::vector<int> bufferVecInt;
std::vector<std::string> bufferVecStr;
std::vector<std::string> var;
std::vector<std::string> varR;
std::vector<std::string> python;

int getlineNo(std::string line){
    for (int lineNo = 1; lineNo <= python.size(); lineNo++)
    {
        if(python[lineNo-1].find(line)!=-1){
            return lineNo;
        }
    }
    return 0;
}

std::string removespace(std::string target) {
    target.erase(target.find_last_not_of(' ') + 1);
    target.erase(0, target.find_first_not_of(' '));
    return target;
}

bool funcexist(std::string line) {
    std::string newstring;
    if(line.find("=")!=-1) {
        newstring = line.substr(line.find("=")+1, line.find("(")-line.find("=")-1);
        newstring = removespace(newstring);
        for(int i =0; i<python.size();i++){
            if(python[i].find(newstring)!=-1) { //function exist
                return true;
            }
        }
    }
    else {
        newstring = line.substr(0, line.find("("));
        newstring = removespace(newstring);
        for(int i =0; i<python.size();i++) {
            if(python[i].find(newstring)!=-1){ //function exist
                return true;
            }
        }
    }
    return false;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

/*
std::vector<std::string> split(const std::string &s, const std::string &seperator) {
    std::vector<std::string> result;
    typedef std::string::size_type string_size;
    int i = 0;
    
    while (i != s.size()) {
        int flag = 0;
        while (i != s.size() && flag == 0) {
            flag = 1;
            for (string_size x = 0; x < seperator.size(); ++x) {
                if (s[i] == seperator[x]) {
                    i++;
                    flag = 0;
                    break;
                }
            }
        }
        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0) {
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[j] == seperator[x]) {
                    flag = 1;
                    break;
                }
            if (flag == 0) {
                j++;
            }
        }
        if (i != j)
        {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}
*/

bool is_number(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) {
        it++;
    }
    return !s.empty() && it == s.end();
}

std::vector<int> getList(std::string input)
{
    std::vector<std::string> lis;
    std::vector<int> result = {};
    //std::vector<int> result;
    int start_pos = input.find('[');
    int end_pos = input.find(']');
    std::string arr = input.substr(start_pos + 1, (end_pos - start_pos) - 1);
    lis = split(arr, ",");
    for (int i = 0; i < lis.size(); i++)
    {
        result.push_back(stoi(lis[i]));
    }
    return result;
}

int findTarget(std::string target)
{
    for (int i = 0; i < variables.size(); i++)
    {
        if (variables[i].first == target)
        {
            return i;
        }
    }
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].first == target)
        {
            return i;
        }
    }
    return -1;
}

int getIndex(std::string input)
{
    int index;
    int start_pos = input.find('[');
    int end_pos = input.find(']');
    if(input.find(":")!=-1){
        if(input.find("[:]")!=-1){
            return -1;
        }
        else{ // [1:]
            end_pos = input.find(':');
        }
    }
    
    if (is_number(input.substr(start_pos + 1, (end_pos - start_pos) - 1)) != 1)
    {
        //std::cout <<"checkpoint149 n "<<input.substr(start_pos + 1, (end_pos - start_pos) - 1)<<" n\n";
        index = variables[findTarget(input.substr(start_pos + 1, (end_pos - start_pos) - 1))].second;
        //std::cout <<"checkpoint151"<<" n\n";
    }
    else
    {
        index = stoi(input.substr(start_pos + 1, (end_pos - start_pos) - 1));
    }
    
    // std::cout<< "list name: " <<listName <<"\n";
    // std::cout<< "index: " << index << "\n";

    return index;
}

std::string findVector(std::string target)
{
    for (int i = 0; i < variables.size(); i++)
    {
        if (variables[i].first == target)
        {
            return "variables";
        }
    }
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i].first == target)
        {
            return "values";
        }
    }
    return " ";
}

bool ifstatement(int left, int mid, int right)
{
    switch (mid)
    {
    case 0:
        if (left >= right)
            return true;
        else
            return false;
        break;
    case 1:
        if (left <= right)
            return true;
        else
            return false;
        break;
    case 2:
        if (left > right)
            return true;
        else
            return false;
        break;
    case 3:
        if (left < right)
            return true;
        else
            return false;
        break;
    case 4:
        if (left != right)
            return true;
        else
            return false;
        break;
    case 5:
        if (left == right)
            return true;
        else
            return false;
        break;
    default:
        return NULL;
    }
}

int handleError(std::string MSG, int lineno)
{
    /*
    1. (missing parenthesis
    2. extra statements (e.g. recursivefunctions)
    3. unhandled data types (e.g. an object)
    */
    //std::cout << "check 236" << "\n";
    //std::cout << "check 237" << "\n";
    if (MSG == "")
        return 0;
    if (MSG.starts_with("#") || MSG.starts_with("print")||MSG.starts_with("return"))
        return 0;
    if(defFlag && actualCodes.starts_with(" ")){
        //std::cout << "checkpoint489" << "\n";
        return 0;
    }
    else if(defFlag && !actualCodes.starts_with(" ")){
        defFlag = false;
        return 0;
    }
    if(MSG.find("(")!=-1 && MSG.find("print")==-1){
        if(funcexist){
            return 0;
        }
        else{
            error_log << FUNC_ERROR << getlineNo(MSG) << "\n";
            std::cout << FUNC_ERROR << getlineNo(MSG) << "\n";
            exit(0);
        }
    }
    if(MSG.find("=")!=-1 && MSG.find("+")!=-1 && MSG.find("[")==-1){// yes =, yes +, no []
        std::vector<std::string> str_R;
        bool haveNo = false;
        str_R=split(MSG,"=");
        str_R=split(str_R[1],"+");

        for(int i = 0; i <str_R.size();i++){
            str_R[i]=removespace(str_R[i]);
        }
        for(int i = 0; i <str_R.size();i++){
            if(is_number(str_R[i])){
                haveNo = true;
                break;
            }
        }
        if(haveNo){
            for(int i = 0; i <str_R.size();i++){
                if(i!=str_R.size()){
                    if(findVector(str_R[i])!="variables"&& !is_number(str_R[i])){
                        error_log << DATA_TYPE_ERROR << getlineNo(MSG) << "\n";
                        std::cout << DATA_TYPE_ERROR << getlineNo(MSG) << "\n";
                        exit(0);
                    }
                    
                }
            }
        }
        else{
            for(int i = 0; i <str_R.size();i++){
                if(i!=str_R.size()-1){
                    if(findVector(str_R[i])!=findVector(str_R[i+1])){
                        error_log << DATA_TYPE_ERROR << getlineNo(MSG) << "\n";
                        std::cout << DATA_TYPE_ERROR << getlineNo(MSG) << "\n";
                        exit(0);
                    }
                    
                }
            }
        }
        return 0;
    }
    //std::cout << "check 242" << "\n";
    std::vector<std::string> varR;
    /*
     if ((MSG.find('(') != -1&&MSG.find(')')==-1)||(MSG.find('(')==-1 && MSG.find(')')!=-1)
     if(count(MSG.begin(),MSG.end(),'(') !=count(MSG.begin(),MSG.end(),')'))
    */
    // missing ()
    // if ( (MSG.find('(') != -1 && MSG.find(')') == -1) || (MSG.find('(') == -1 && MSG.find(')') != -1) )
    if (count(MSG.begin(), MSG.end(), '(') != count(MSG.begin(), MSG.end(), ')'))
    {
        error_log << SYNTAX_ERROR << getlineNo(MSG) << "\n";
        std::cout << SYNTAX_ERROR << getlineNo(MSG) << "\n";
        exit(0);
    }
    // if(count(MSG.begin(),MSG.end(),'"')%2 != 0 || count(MSG.begin(),MSG.end(),"\'")%2 != 0){
    if (count(MSG.begin(), MSG.end(), '"') % 2 != 0)
    {
        error_log << SYNTAX_ERROR << getlineNo(MSG) << "\n";
        std::cout << SYNTAX_ERROR << getlineNo(MSG) << "\n";
        exit(0);
    }
    if (count(MSG.begin(), MSG.end(), '[') != count(MSG.begin(), MSG.end(), ']'))
    {
        error_log << SYNTAX_ERROR << getlineNo(MSG) << "\n";
        std::cout << SYNTAX_ERROR << getlineNo(MSG) << "\n";
        exit(0);
    }
    std::string for_if_only;
    for_if_only=removespace(MSG);
    if (for_if_only.starts_with("if"))
    {
        if (MSG.find(":") == -1)
        { //||count(MSG.begin(),MSG.end(),'=')==1
            error_log << SYNTAX_ERROR << getlineNo(MSG) << "\n";
            std::cout << SYNTAX_ERROR << getlineNo(MSG) << "\n";
            exit(0);
        }
        return 0;
    }
    //std::cout << "check 281" << "\n";
    if (MSG.find('=') != -1)
    {
        //std::cout << "check 284" << "\n";
        varR = split(MSG, "=");
        varR[1] = removespace(varR[1]);
        if (varR[1].find("+") == -1)
        { // no +
            if (varR[1].starts_with("["))
            {
                return 0;
            }

            if (varR[1].find('[') != -1)
            { // variable with []
            
                if (varR[1].find("[:]")==-1) {
                    int index = getIndex(varR[1]);
                    varR[1] = varR[1].substr(0, varR[1].find("["));
                    if (findVector(varR[1]) == " ")
                    { // no this list exist
                        error_log << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        std::cout << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        //std::cout <<"checkpoint282";
                        exit(0);
                    }
                    else
                    { // this list is exist
                        
                        if (index > values[findTarget(varR[1])].second.size() - 1)
                        { // index out of range
                            error_log << OUTRANGE << getlineNo(MSG) << "\n";
                            std::cout << OUTRANGE << getlineNo(MSG) << "\n";
                            //std::cout<<index<<"  checkpoint304 \n";
                            exit(0);
                        }
                    }
                }
                //std::cout << "check 294" << "\n";
            }
            if (is_number(varR[1]) != 1)
            { // not a digit
                if(varR[1].find("[")==-1){//no []on the right
                    if (findVector(varR[1]) == " ")
                    { // no this variable exist
                        error_log << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        std::cout << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        //std::cout <<"checkpoint301";
                        exit(0);
                    }
                }
                else{// yes [] on the right
                    std::string listName = varR[1].substr(0, (varR[1].find("[")));
                    if (findVector(listName) == " ")
                    { // no this variable exist
                        error_log << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        std::cout << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        //std::cout <<"checkpoint328" << "\n";
                        exit(0);
                    }
                }    
            }
            //std::cout << "check 305" << "\n";
        }

        else
        { // yes +
            varR = split(varR[1], "+");
            //std::cout << "check 348" << "\n";
            for (int i = 0; i < varR.size(); i++)
            {
                if(MSG.find("[")==-1){// no []
                    //std::cout << "check 351" << "\n";
                    /*
                    if(findVector(varR[i])!=findVector(varR[i+1])){
                        std::cout << "check 352" << "\n";
                        error_log << DATA_TYPE_ERROR << lineno << " \n";
                        std::cout << DATA_TYPE_ERROR << lineno << " \n";
                        exit(0);
                    }*/
                }
                varR[i] = removespace(varR[i]);
                if (varR[i].starts_with("["))
                {
                    return 0;
                }
                if (varR[i].find('[') != -1)
                { // variable with []
                    int index = getIndex(varR[i]);
                    varR[i] = varR[i].substr(0, varR[i].find("["));
                    if (findVector(varR[i]) == " ")
                    { // no this list exist
                        error_log << NO_SUCH_VARIABLE << getlineNo(MSG) << " \n";
                        std::cout << NO_SUCH_VARIABLE << getlineNo(MSG) << " \n";
                        //std::cout <<"checkpoint324";
                        exit(0);
                    }
                    else
                    { // this list is exist
                        if (index > values[findTarget(varR[i])].second.size() - 1)
                        { // index out of range
                            error_log << OUTRANGE << getlineNo(MSG) << "\n";
                            std::cout << OUTRANGE << getlineNo(MSG) << "\n";
                            //std::cout<<"checkpoint362 \n";
                            exit(0);
                        }
                    }
                }
                if (is_number(varR[i]) != 1)
                { // not a digit
                    if (findVector(varR[i]) == " ")
                    { // no this variable exist
                        error_log << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        std::cout << NO_SUCH_VARIABLE << getlineNo(MSG) << "\n";
                        //std::cout <<i<<" checkpoint343 \n";
                        exit(0);
                    }
                }
            }
        }
    }
    return 0;
}

void main_function(std::vector<std::string> python);

void run_function_N(std::string MSG){
    std::string function_name = MSG.substr(0,MSG.find("("));
    std::string parameter_name = MSG.substr(MSG.find("(")+1,MSG.find(")")-MSG.find("(")-1);
    std::vector<std::string> parameter_name_list;
    if(parameter_name.find(",")!=-1){//more than 1 parameter
        parameter_name_list=split(parameter_name,",");
    }
    else{// only one parameter
        parameter_name_list.push_back(parameter_name);
    }
    std::vector<std::string> function_content;
    //std::cout<<"checkpoint 413 n\n";
    for(int i =0; i <python.size();i++){
        if(python[i].find(function_name)!=-1){
            std::string parameter_name_with_def = python[i].substr(python[i].find("(")+1,python[i].find(")")-python[i].find("(")-1);
            std::vector<std::string> parameter_name_list_with_def;
            if(parameter_name.find(",")!=-1){//more than 1 parameter
                parameter_name_list_with_def=split(parameter_name_with_def,",");
            }
            else{// only one parameter
                parameter_name_list_with_def.push_back(parameter_name_with_def);
            }
            for(int q=0;q<parameter_name_list_with_def.size();q++){
                //std::cout<<"checkpoint 425 n\n";
                if(parameter_name_list_with_def[q]!=parameter_name_list[q]){
                    //std::cout<<parameter_name_list_with_def[q]<<" checkpoint 427 n\n";
                    //std::cout<<parameter_name_list[q]<<" n\n";
                    if(findVector(parameter_name_list_with_def[q])!=" "){
                        //std::cout<<"checkpoint 431 n\n";
                        variables[findTarget(parameter_name_list_with_def[q])].second=variables[findTarget(parameter_name_list[q])].second;
                        continue;
                    }
                    else if(findVector(parameter_name_list[q])=="variables"){//parameter is a variable
                        //std::cout<<"checkpoint 429 n\n";
                        variables.push_back(std::make_pair(parameter_name_list_with_def[q], variables[findTarget(parameter_name_list[q])].second));
                        //std::cout<<"checkpoint 431 n\n";
                    }
                    else if(is_number(parameter_name_list[q])==1){//parameter is a number
                        variables.push_back(std::make_pair(parameter_name_list_with_def[q], stoi(parameter_name_list[q])));     
                    }
                    else{//parameter is a list
                        values.push_back(std::make_pair(parameter_name_list_with_def[q], values[findTarget(parameter_name_list[q])].second));
                    }
                    //std::cout<<"checkpoint 432 n\n";
                }
            }
            for(i=i+1;i <python.size();i++){
                if(!python[i].starts_with(" ")||all_of(python[i].begin(),python[i].end(),isspace))
                    break;
                function_content.push_back(python[i]);
            }
            break;
        }
    }
    //std::cout<<"checkpoint 455 n\n";
    main_function(function_content);
}

std::vector<int> run_function_L(std::string MSG){
    std::string function_name = MSG.substr(0,MSG.find("("));
    std::string parameter_name = MSG.substr(MSG.find("(")+1,MSG.find(")")-MSG.find("(")-1);
    std::vector<std::string> parameter_name_list;
    if(parameter_name.find(",")!=-1){//more than 1 parameter
        parameter_name_list=split(parameter_name,",");
    }
    else{// only one parameter
        parameter_name_list.push_back(parameter_name);
    }
    std::vector<std::string> function_content;
    std::string last_line_of_function;
    std::vector<std::string> last_line_of_function_list;
    for(int i =0; i <python.size();i++){

        if(python[i].find(function_name)!=-1){
            std::string parameter_name_with_def = python[i].substr(python[i].find("(")+1,python[i].find(")")-python[i].find("(")-1);
            std::vector<std::string> parameter_name_list_with_def;
            if(parameter_name.find(",")!=-1){//more than 1 parameter
                parameter_name_list_with_def=split(parameter_name_with_def,",");
            }
            else{// only one parameter
                parameter_name_list_with_def.push_back(parameter_name_with_def);
            }
            for(int q=0;q<parameter_name_list_with_def.size();q++){
                if(parameter_name_list_with_def[q]!=parameter_name_list[q]){
                    if(findVector(parameter_name_list_with_def[q])!=" "){
                        variables[findTarget(parameter_name_list_with_def[q])].second=variables[findTarget(parameter_name_list[q])].second;
                        continue;
                    }
                    else if(findVector(parameter_name_list[q])=="variables"){//parameter is a variable
                        variables.push_back(std::make_pair(parameter_name_list_with_def[q], variables[findTarget(parameter_name_list[q])].second));
                    }
                    else if(is_number(parameter_name_list[q])==1){//parameter is a number
                        variables.push_back(std::make_pair(parameter_name_list_with_def[q], stoi(parameter_name_list[q])));     
                    }
                    else{//parameter is a list
                        values.push_back(std::make_pair(parameter_name_list_with_def[q], values[findTarget(parameter_name_list[q])].second));
                    }
                }
            }
            for(i=i+1;i <python.size();i++){
                if(!python[i].starts_with(" ")||all_of(python[i].begin(),python[i].end(),isspace))
                    break;
                function_content.push_back(python[i]);
            }
            break;
        }
    }
    main_function(function_content);
    last_line_of_function=function_content[function_content.size()-1];
    last_line_of_function = removespace(last_line_of_function);
    last_line_of_function_list = split(last_line_of_function," ");

    return values[findTarget(last_line_of_function_list[1])].second;
}

int run_function(std::string MSG){
    std::string function_name = MSG.substr(0,MSG.find("("));
    std::string parameter_name = MSG.substr(MSG.find("(")+1,MSG.find(")")-MSG.find("(")-1);
    std::vector<std::string> parameter_name_list;
    //std::cout<<parameter_name<<" n\n";
    if(parameter_name.find(",")!=-1){//more than 1 parameter
        parameter_name_list=split(parameter_name,",");
    }
    else{// only one parameter
        parameter_name_list.push_back(parameter_name);
    }
    std::vector<std::string> function_content;
    std::string last_line_of_function;
    std::vector<std::string> last_line_of_function_list;
    for(int i =0; i <python.size();i++){

        if(python[i].find(function_name)!=-1){
            std::string parameter_name_with_def = python[i].substr(python[i].find("(")+1,python[i].find(")")-python[i].find("(")-1);
            std::vector<std::string> parameter_name_list_with_def;
            if(parameter_name.find(",")!=-1){//more than 1 parameter
                parameter_name_list_with_def=split(parameter_name_with_def,",");
            }
            else{// only one parameter
                parameter_name_list_with_def.push_back(parameter_name_with_def);
            }
            for(int q=0;q<parameter_name_list_with_def.size();q++){
                //std::cout<<"checkpoint 425 n\n";
                if(parameter_name_list_with_def[q]!=parameter_name_list[q]){
                    //std::cout<<"checkpoint 427 n\n";
                    //std::cout<<parameter_name_list[q]<<" n\n";
                    if(findVector(parameter_name_list_with_def[q])!=" "){
                        
                        if(findVector(parameter_name_list[q])=="variables")//parameter is a variable
                        {
                            variables[findTarget(parameter_name_list_with_def[q])].second=variables[findTarget(parameter_name_list[q])].second;
                        }
                        else if(is_number(parameter_name_list[q])==1){//parameter is a number
                            variables[findTarget(parameter_name_list_with_def[q])].second=stoi(parameter_name_list[q]);    
                        }
                        else{//parameter is a list
                            values[findTarget(parameter_name_list_with_def[q])].second = values[findTarget(parameter_name_list[q])].second;
                        }
                    }
                    else{
                        if(findVector(parameter_name_list[q])=="variables"){//parameter is a variable
                            //std::cout<<"checkpoint 429 n\n";
                            variables.push_back(std::make_pair(parameter_name_list_with_def[q], variables[findTarget(parameter_name_list[q])].second));
                            //std::cout<<"checkpoint 431 n\n";
                        }
                        else if(is_number(parameter_name_list[q])==1){//parameter is a number
                            variables.push_back(std::make_pair(parameter_name_list_with_def[q], stoi(parameter_name_list[q])));     
                        }
                        else{//parameter is a list
                            values.push_back(std::make_pair(parameter_name_list_with_def[q], values[findTarget(parameter_name_list[q])].second));
                        }
                    }
                    
                    //std::cout<<"checkpoint 432 n\n";
                }
            }
            for(i=i+1;i <python.size();i++){
                if(!python[i].starts_with(" ")||all_of(python[i].begin(),python[i].end(),isspace))
                    break;
                function_content.push_back(python[i]);
            }
            break;
        }
    }
    //std::cout<<"checkpoint 622 "<< variables[findTarget("index")].second  <<" n\n";
    main_function(function_content);
    //std::cout << "checkpoint443" << "\n";
    last_line_of_function=function_content[function_content.size()-1];
    //std::cout << "checkpoint445" << "\n";
    last_line_of_function = removespace(last_line_of_function);
    //std::cout << "checkpoint447" << "\n";
    last_line_of_function_list = split(last_line_of_function," ");
    //std::cout << "checkpoint449" << "\n";
    //std::cout <<last_line_of_function_list[1]<< " 450\n";
    //std::cout <<variables[findTarget(last_line_of_function_list[1])].second<<" 451n\n";
    for(int c=0;c<variables.size();c++){
        //std::cout<<" showroom : "<<variables[c].first<<" : "<<variables[c].second<<" \n";
    }
    if(last_line_of_function.find("+")!=-1){
        int idkwmid=0;
        last_line_of_function=last_line_of_function.substr(last_line_of_function.find(" ")+1);
        last_line_of_function_list=split(last_line_of_function,"+");
        for(int z=0;z<last_line_of_function_list.size();z++){
            last_line_of_function_list[z]=removespace(last_line_of_function_list[z]);
            //std::cout <<last_line_of_function_list[z]<<" "<<variables[findTarget(last_line_of_function_list[z])].second<<" dddddn\n";
        }
        for(int x=0;x<last_line_of_function_list.size();x++){
            idkwmid+=variables[findTarget(last_line_of_function_list[x])].second;
        }
        //std::cout << "checkpoint517" << "\n";
        return idkwmid;
    }
    else{
        //std::cout<<variables[findTarget(last_line_of_function_list[1])].first<<" to check what t h u r :"<<variables[findTarget(last_line_of_function_list[1])].second<<" \n";
        return variables[findTarget(last_line_of_function_list[1])].second;
    }
}

std::string to_check_the_return_type(std::string MSG){
    //std::cout <<MSG<< "checkpoint456"<<" n\n";
    std::string function_name = MSG.substr(0,MSG.find("("));
    //std::cout <<function_name<< " checkpoint459"<<" n\n";
    std::string parameter_name = MSG.substr(MSG.find("(")+1,MSG.find(")")-MSG.find("(")-1);
    std::vector<std::string> parameter_name_list;
    std::vector<std::string> function_content;
    std::string last_line_of_function;
    std::vector<std::string> last_line_of_function_list;
    if(parameter_name.find(",")!=-1){//more than 1 parameter
        parameter_name_list=split(parameter_name,",");
    }
    else{// only one parameter
        parameter_name_list.push_back(parameter_name);
    }

    for(int i =0; i <python.size();i++){
        if(python[i].find(function_name)!=-1){
            std::string parameter_name_with_def = python[i].substr(python[i].find("(")+1,python[i].find(")")-python[i].find("(")-1);
            std::vector<std::string> parameter_name_list_with_def;
            if(parameter_name.find(",")!=-1){//more than 1 parameter
                parameter_name_list_with_def=split(parameter_name_with_def,",");
            }
            else{// only one parameter
                parameter_name_list_with_def.push_back(parameter_name_with_def);
            }
            for(int q=0;q<parameter_name_list_with_def.size();q++){
                //std::cout<<"checkpoint 425 n\n";
                if(parameter_name_list_with_def[q]!=parameter_name_list[q]){
                    //std::cout<<"checkpoint 427 n\n";
                    //std::cout<<parameter_name_list[q]<<" n\n";
                    if(findVector(parameter_name_list[q])=="variables"){//parameter is a variable
                        //std::cout<<"checkpoint 429 n\n";
                        variables.push_back(std::make_pair(parameter_name_list_with_def[q], variables[findTarget(parameter_name_list[q])].second));
                        //std::cout<<"checkpoint 431 n\n";
                    }
                    else if(is_number(parameter_name_list[q])==1){//parameter is a number
                        
                        //std::cout<<"checkpoint 692 "<< parameter_name_list_with_def[q]<<" : "<<parameter_name_list[q]<<" n\n";
                        variables.push_back(std::make_pair(parameter_name_list_with_def[q], stoi(parameter_name_list[q])));  
                        //std::cout<<"checkpoint 694 "<< variables[findTarget("index")].second  <<" n\n";   
                    }
                    else{//parameter is a list
                        values.push_back(std::make_pair(parameter_name_list_with_def[q], values[findTarget(parameter_name_list[q])].second));
                    }
                    //std::cout<<"checkpoint 432 n\n";
                }
            }
            for(i=i+1;i <python.size();i++){
                if(!python[i].starts_with(" ")||all_of(python[i].begin(),python[i].end(),isspace))
                    break;
                function_content.push_back(removespace(python[i]));
            }
            break;
        }
    }
    main_function(function_content);
    //std::cout <<function_content[1]<< " checkpoint472"<<" n\n";
    last_line_of_function=function_content[function_content.size()-1];
    //std::cout <<last_line_of_function<<"checkpoint474"<<" n\n";
    last_line_of_function = removespace(last_line_of_function);
    //std::cout << "checkpoint476"<<" n\n";
    last_line_of_function_list = split(last_line_of_function," ");
    //std::cout <<"checkpoint478"<<" n\n";
    //std::cout <<last_line_of_function_list[1]<<" n\n";
    std::string returnvalue=findVector(last_line_of_function_list[1]);
    //std::cout <<"checkpoint480"<<" n\n";
    //std::cout << returnvalue << "  n\n";
    return returnvalue;
}

void main_function(std::vector<std::string> python)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
    for (int lineNo = 1; lineNo <= python.size(); lineNo++)
    {
        //std::cout << lineNo << " n\n";        ////////////////////////////////////////////// lineno
        //std::cout<< python[lineNo-1]<< "\n";
         //std::cout<< boolresult<<" i\n";
        // std::cout << findVector("listZ") << " n\n";
        int tmp = 0;
        bufferVecInt = {};
        actualCodes = python[lineNo - 1];
        //std::cout << "checkpoint363" << "\n";
        handleError(actualCodes, lineNo);
        //std::cout << "checkpoint478" << "\n";
        if(defFlag && actualCodes.starts_with(" ")){
            //std::cout << "checkpoint489" << "\n";
            continue;
        }
        else if(defFlag && !actualCodes.starts_with(" ")){
            defFlag = false;
        }
        //std::cout << "checkpoint1" << "\n";
        actualCodes = removespace(actualCodes);
        // var = {A:1, B:2} cout << var[1].first = B
        //std::cout << "checkpoint2" << "\n";
        if(actualCodes.starts_with("defFlag")){
            defFlag=true;
            continue;
        }
        //std::cout<< actualCodes<< "499\n";
        //std::cout << "checkpoint3" << "\n";
        if(actualCodes.starts_with("return")){
            //std::cout << "checkpoint501" << "\n";
            return ;
        }    
        //std::cout << "checkpoint4" << "\n";
        if (actualCodes.find("else") != -1)
        {
            elseFlag = true;
            continue;
        }
        //std::cout <<"checkpoint651"<<" n\n";
        if (ifFlag)
        {
            //std::cout <<"checkpoint654 "<<python[lineNo]<<" n\n";
            if (!boolresult && !elseFlag)
            {
                //std::cout <<"checkpoint656"<<" n\n";
                continue;
            }
            else if (boolresult && elseFlag)
            {
                //std::cout <<"checkpoint661"<<" n\n";
                if (!python[lineNo].starts_with(" "))
                {
                    ifFlag = false;
                    boolresult = false;
                    elseFlag = false;
                }
                continue;
            }
            else if (!python[lineNo].starts_with(" ") && !python[lineNo].starts_with("else"))
            {
                //std::cout <<"checkpoint672"<<" n\n";
                ifFlag = false;
                boolresult = false;
                elseFlag = false;
            }   
            else if(python[lineNo-1].find("if")!=-1){
                //std::cout<<"checkpoint678 n\n";
                elseFlag = false;
            }
            //std::cout <<"checkpoint682"<<" n\n";
        }
        //std::cout <<"checkpoint684"<<" n\n";
        if (actualCodes.starts_with("#"))
        {
            continue;
        }
        else if (actualCodes.starts_with("print"))
        {                                    // printtttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
            if (actualCodes.find(",") != -1) // print more than one parameters
            {
                actualCodes = actualCodes.substr(actualCodes.find("(") + 1, actualCodes.find(")") - actualCodes.find("(") - 1);
                bufferVecStr = split(actualCodes, ",");
                words = bufferVecStr[0];
                words = words.substr(words.find_first_of('"') + 1, words.find_last_of('"') - 1);
                std::cout << words<<" ";
                target = bufferVecStr[1];
                target=removespace(target);
                //std::cout << "Checking parameter on the right side of colon ('listX:', ?): " << target << "\n"; 
                //std::cout << "Checking parameter on the left side of colon (?, listX): " << bufferVecStr[0] << "\n"; 
                // 修理: print("listX:", listX)
                //for (int i=0; i<values.size(); i++) {std::cout << "values: " << values[i].first << " " << "\n";}
                if (findVector(target) == "values"){
                    //std::cout << "Check 692" << "\n";
                    std::cout << "[";
                    for (int i = 0; i < values[findTarget(target)].second.size(); i++)
                    {
                        std::cout << values[findTarget(target)].second[i];
                        if (i != values[findTarget(target)].second.size() - 1)
                            std::cout << ", ";
                    }
                    std::cout << "]" << "\n";
                }
                else{
                    if (findVector(target) == "variables")
                    {
                        // std::cout << target << "\n";
                        std::cout << variables[findTarget(target)].second << "\n";
                    }
                    else if (findVector(target) == "values")
                    {
                        //std::cout << "INSIDE PRINT values CONDITION" << "\n";
                        std::cout << "[";
                        for (int i = 0; i < values[findTarget(target)].second.size(); i++)
                        {
                            std::cout << values[findTarget(target)].second[i];
                            if (i != values[findTarget(target)].second.size() - 1)
                                std::cout << ", ";
                        }
                        std::cout << "]"
                                << "\n";
                    }
                }
            }
            else // only one parameter in print()
            {
                //std::cout << "Checkpoint: else" << "\n";
                target = actualCodes.substr(actualCodes.find("(") + 1, actualCodes.find(")") - actualCodes.find("(") - 1);
                // std::cout <<target<<" n\n";
                // std::cout << findTarget("a") << "\n";
                if (findVector(target) == "variables")
                {
                    // std::cout << target << "\n";
                    std::cout << variables[findTarget(target)].second << "\n";
                }
                else if (findVector(target) == "values")
                {
                    //std::cout << "INSIDE PRINT values CONDITION" << "\n";
                    std::cout << "[";
                    for (int i = 0; i < values[findTarget(target)].second.size(); i++)
                    {
                        std::cout << values[findTarget(target)].second[i];
                        if (i != values[findTarget(target)].second.size() - 1)
                            std::cout << ", ";
                    }
                    std::cout << "]" << "\n";
                }
            }
            // std::cout<<target<<"\n";
            /**
            if (findVector(target) == "variables")
            {
                // std::cout << target << "\n";
                std::cout << variables[findTarget(target)].second << "\n";
            }
            else if (findVector(target) == "values")
            {
                //std::cout << "INSIDE PRINT values CONDITION" << "\n";
                std::cout << "[";
                for (int i = 0; i < values[findTarget(target)].second.size(); i++)
                {
                    std::cout << values[findTarget(target)].second[i];
                    if (i != values[findTarget(target)].second.size() - 1)
                        std::cout << ", ";
                }
                std::cout << "]"
                          << "\n";
            }
*/
            /*for (int i=0; i<variables.size(); i++){
                if (variables[i].first == target){
                    std::cout<<variables[i].second<<"\n";
                }
            }*/
        }
        else if (count(actualCodes.begin(), actualCodes.end(), '=') == 1 && !actualCodes.starts_with("if"))
        { // assign values (only 1 "=")
            //std::cout <<"checkpoint453"<<" n\n";
            var = split(actualCodes, "=");
            if (actualCodes.find('+') == -1)
            { // no 加+减-乘*除/
                //std::cout <<"checkpoint457"<<" n\n";
                for (int j = 0; j < var.size(); j++)
                {
                    //std::cout << "check kick before" << "\n";
                    var[j] = removespace(var[j]);
                    //std::cout << "checkpoint489" << "\n";
                }
                if (var[0].find('[') == -1)
                { // no [] on left side
                    //std::cout <<"checkpoint464"<<" n\n";
                    //  std::cout << "var[1]: " << var[1] << "\n";
                    if (var[1].find('[') != -1)
                    { // at least one [] on the right side

                        //std::cout << "check statement" << "\n";
                        if (var[1].starts_with("["))
                        {
                            //std::cout << "checkpoint664" << " n\n";
                            if(var[1].find(",")==-1){
                                bufferVecInt=getList(var[1]);
                                //values.push_back(std::make_pair(var[0], getList(var[1])));
                                //std::cout<<values[findTarget(var[0])].second.size()<<" 805 n\n";    
                            }    
                            else{
                                var[1]=var[1].substr(1,var[1].find("]")-1);
                                std::vector<std::string> content=split(var[1],",");
                                for(int q=0;q<content.size();q++){
                                    if(is_number(content[q])==1){
                                        bufferVecInt.push_back(stoi(content[q]));
                                    }
                                    else{
                                        bufferVecInt.push_back(variables[findTarget(content[q])].second);
                                    }
                                }
                            }
                            //std::cout <<"checkpoint819"<<" n\n";
                            if(findVector(var[0])=="values"){
                                values[findTarget(var[0])].second=bufferVecInt;
                            }
                            else{
                                values.push_back(std::make_pair(var[0],bufferVecInt));
                            }
                            //std::cout << "checkpoint826" << " n\n";
                            //std::cout<<values[findTarget(var[0])].second[0]<<" 827\n";
                            //std::cout<<values[findTarget(var[0])].second.size()<<" 828\n";
                        }
                        else if (var[1].find("[:]")!=-1 ){
                            var[1]=var[1].substr(0,(var[1].find("[")));
                            //std::cout << "var[1]: " << var[1] <<" \n";
                            //std::cout << "var[0]: " << var[0] << "\n";
                            values.push_back(std::make_pair(var[0], values[findTarget(var[1])].second));
                        }
                        else
                        { // element = list[i]
                            //std::cout <<"checkpoint636"<<" n\n";
                            int index = getIndex(var[1]);
                            //std::cout <<"checkpoint638"<<" n\n";
                            std::string listName = var[1].substr(0, (var[1].find("[")));
                            //std::cout <<"checkpoint640"<<" n\n";
                            int pos = findTarget(listName);
                            //std::cout <<"checkpoint642"<<" n\n";
                            int value = values[pos].second[index];
                            //std::cout <<"checkpoint644"<<" n\n";
                            variables.push_back(std::make_pair(var[0], value));
                            //std::cout << "checkpoint646"<< " n\n";
                        }
                    }
                    else // no +-*/, no [] on right
                    {
                        if(var[1].find("len(")!=-1){
                            std::string listName= var[1].substr(var[1].find("(")+1,var[1].find(")")-var[1].find("(")-1);
                            if(findVector(var[0])=="variables"){
                                variables[findTarget(var[0])].second=values[findTarget(listName)].second.size();
                            }
                            else{
                                variables.push_back(std::make_pair(var[0],values[findTarget(listName)].second.size()));
                            }
                        }
                        else if(var[1].find("(")!=-1){//yes defFlag on the right              
                            std::string functionname = var[1];
                            std::string variable_on_the_L = var[0];
                            //std::cout <<"checkpoint643"<<" n\n";
                            std::string qwer=to_check_the_return_type(var[1]);
                            //std::cout <<qwer<< "checkpoint645"<< " n\n";
                            if(qwer=="variables"){//function return variables
                                //std::cout <<functionname<< " 64444444444444444444n\n";
                                if(findVector(variable_on_the_L)=="variables")
                                    variables[findTarget(variable_on_the_L)].second=run_function(functionname);
                                else
                                    variables.push_back(std::make_pair(variable_on_the_L,run_function(functionname)));
                                defFlag = false;
                                //std::cout << variable_on_the_R<< "   643\n";
                                //std::cout << variables[findTarget(variable_on_the_R)].second<< "   644\n";
                            }
                            else{//function return list
                                if(findVector(variable_on_the_L)=="values")
                                    values[findTarget(variable_on_the_L)].second=run_function_L(functionname);
                                else
                                    values.push_back(std::make_pair(variable_on_the_L,run_function_L(functionname)));
                                defFlag = false;
                            }
                        }
                        else{//↓
                            //std::cout << "Check708"<< "\n";
                            if (variables.size() >= 1)
                            {
                                if (findTarget(var[0]) == -1)
                                {
                                    //std::cout <<var[0]<< "Check713"<< "\n";
                                    //std::cout <<var[1]<< "Check713"<< "\n";
                                    //variables.push_back(std::make_pair(var[0], stoi(var[1])));
                                    variables.push_back(std::make_pair(var[0], stoi(var[1])));
                                    //for (int i=0; i < variables.size(); i++){
                                    //    std::cout << "variables: " << variables[i].first << " " << variables[i].second << "\n";
                                    //}
                                    //std::cout << "Check490"<< "\n";
                                }
                                else
                                {
                                    variables[findTarget(var[0])].second = stoi(var[1]);
                                    //std::cout << "Check495"<< "\n";
                                }
                            }
                            else if (variables.size() == 0)
                            {
                                variables.push_back(std::make_pair(var[0], stoi(var[1])));
                                //std::cout << "Check501"<< "\n";
                            }
                        }    
                    }
                    // std::cout << "var[1]: " << var[1] << "\n";
                    // if (var[1].find('[') != -1) { // contains at least one list on the right side
                    // std::string content = var[1].substr(var[1].find("["), 3);

                    // std::cout << "Content of the list: " << "\n";
                    //}
                }
                else
                { // [] on left and no +-*/ on right
                    //std::cout <<"checkpoint1"<<" n\n";
                    int listIndex = getIndex(var[0]);
                    //std::cout <<"checkpoint2"<<" n\n";
                    //std::cout << "listIndex: " << listIndex << "\n";
                    std::string listName = var[0].substr(0, (var[0].find("[")));// listname on the left of =
                    //std::cout <<"checkpoint3"<<" n\n";
                    int value;
                    //std::cout <<listName<<" this is listname\n";
                    //for(int s=0;s<values[findTarget(listName)].second.size();s++)
                    //    std::cout<<values[findTarget(listName)].second[0]<<" \n";
                    // std::cout << "var[1]: n" << var[1] << "n\n";
                    //std::cout <<"checkpoint"<<" n\n";
                    if(var[0].find(":")!=-1){// there is ":" inside the [] on the left of hte =
                        int RlistIndex = getIndex(var[1]);
                        //std::cout << "RlistIndex: " << RlistIndex << "\n";
                        std::string RlistName = var[1].substr(0, (var[1].find("[")));
                        //std::cout <<"RlistName: " << RlistName << "n\n";
                        //std::cout<<values[findTarget(RlistName)].second.size()<<" <<this is size \n";
                        for(listIndex;listIndex<values[findTarget(RlistName)].second.size();listIndex++){
                            values[findTarget(listName)].second[listIndex]=values[findTarget(RlistName)].second[RlistIndex];
                            RlistIndex++;
                        }
                        //std::cout <<"checkpoint561"<<" n\n";
                    }
                    //std::cout <<"checkpoint"<<" n\n";
                    if (is_number(var[1])==1)
                    { // contains [] on left, non-integer on right
                        value = stoi(var[1]);
                    }
                    else
                    {
                        if(var[1].find("[")==-1){//no [] on the right
                            value = variables[findTarget(var[1])].second;
                        }
                        else{// yes [] on the right
                            
                            int listIndex = getIndex(var[1]);
                            std::string listName = var[1].substr(0, (var[1].find("[")));

                            value = values[findTarget(listName)].second[listIndex];
                        }
                        // std::cout << "index: " << findTarget(var[1]) << "\n";
                        // std::cout<<variables[findTarget(var[1])].first<<" v\n";
                    }
                    // std::cout<<"checkpoint"<<" n\n";
                    values[findTarget(listName)].second[listIndex]=value;
                    /*for (int i = 0; i < values.size(); i++)
                    {
                        if (values[i].first == listName)
                        {
                            std::vector<int> tmpV = values[i].second;
                            tmpV[listIndex] = value;
                            values[i].second = tmpV;
                            // std::cout << values[0].first << " " << values[0].second[1] << "\n";
                        }
                    }
                    */
                }
            }
            else if (actualCodes.find('+') != -1 && var[1].find('[') != -1)
            { // a = temp + y + l3[0] //yes+-*/ && yes[] on the right
                varR = split(var[1], "+");
                var[0] = removespace(var[0]);
                //std::cout <<"checkpoint797"<<" n\n";
                if (var[0].find('[') != -1){ //listZ[0] = listZ[0] + x
                    //std::cout << "checkpoint555"<<" n\n";
                    for (int m = 0; m < varR.size(); m++)
                    {
                        varR[m] = removespace(varR[m]);
                        if (is_number(varR[m]) == 1)
                        {
                            tmp = tmp + stoi(varR[m]);
                        }
                        else if(varR[m].find('[')==-1){
                            tmp = tmp + variables[findTarget(varR[m])].second;
                        }
                        else{
                            int listIndex = getIndex(varR[m]);
                            std::string listName = varR[m].substr(0, (varR[m].find("[")));
                            tmp=tmp + values[findTarget(listName)].second[listIndex];
                        }
                    }
                    int LlistIndex = getIndex(var[0]);
                    std::string LlistName = var[0].substr(0, (var[0].find("[")));
                    values[findTarget(LlistName)].second[LlistIndex]= tmp;                    
                }
                else{
                    //std::cout <<"checkpoint821"<<" n\n";
                    for (int m = 0; m < varR.size(); m++)
                    {
                        varR[m] = removespace(varR[m]);
                        if (is_number(varR[m]) == 1)
                        {
                            tmp += stoi(varR[m]);
                        }
                        else
                        {
                            if (varR[m].find("[") == -1)
                            {
                                if(findVector(varR[m])=="values"){//whole list
                                    for (int j = 0; j < values[findTarget(varR[m])].second.size(); j++)
                                    {
                                        bufferVecInt.push_back(values[findTarget(varR[m])].second[j]);
                                    }
                                }
                                else
                                    tmp = tmp + variables[findTarget(varR[m])].second;
        
                            }
                            else if(varR[m].starts_with("[")){
                                bufferVecInt.push_back(getIndex(varR[m]));
                            }
                            else
                            {
                                int listIndex = getIndex(varR[m]);
                                std::string listName = varR[m].substr(0, (varR[m].find("[")));
                                tmp = tmp + values[findTarget(listName)].second[listIndex];
                            }
                        }
                    }
                    // std::cout<<tmp<<" tmp result\n";
                    if(findVector(var[0])=="variables"){
                        variables[findTarget(var[0])].second=tmp;
                    }
                    else if(findVector(var[0])=="values"){
                        values[findTarget(var[0])].second = bufferVecInt;
                    }
                    else{
                        variables.push_back(std::make_pair(var[0], tmp));
                    }
                }    
            }
            else if (actualCodes.find('+') != -1 && var[1].find('(') != -1){//yes+-*/ and function calling on right
                varR = split(var[1], "+");
                var[0] = removespace(var[0]);
                std::string value_L = var[0];
                std::string return_type ="";
                int buffvalue=0;
                return_type=findVector(varR[0]);
                /*for(int q=0; q<varR.size();q++){
                    if(findVector(varR[q])==" "){
                        std::cout <<"checkpoint1071 "<<varR[q]<<" n\n";
                        std::string return_type = to_check_the_return_type(varR[q]);
                        std::cout <<q<<" checkpoint1073 "<<return_type<<" n\n";
                        break;
                    }
                }*/
                //std::cout <<"checkpoint1075 "<<return_type<<" n\n";
                if(return_type=="variables"){
                    //std::cout <<"checkpoint1077"<<" n\n";
                    for(int q=0; q<varR.size();q++){
                        if(findVector(varR[q])==" "){
                            buffvalue+=run_function(varR[q]);
                            //std::cout <<"checkpoint1079 "<<buffvalue<<" n\n";
                        }
                        else{
                            buffvalue+=variables[findTarget(varR[q])].second;
                        }
                    }
                    if(findVector(var[0])==" "){
                        variables.push_back(std::make_pair(value_L,buffvalue));
                        //std::cout <<"checkpoint1092"<<" n\n";
                    }
                    else{
                        variables[findTarget(value_L)].second=buffvalue;
                        //std::cout <<var[0]<<" checkpoint1096"<<" n\n";
                    }
                }
            }
            else
            { // yes 加减乘除+-*/ in MSG
                 //std::cout <<var[0]<<"\n";
                for (int j = 0; j < var.size(); j++)
                {
                    var[j] = removespace(var[j]);
                }
                //std::cout <<"checkpoint821"<<" n\n";
                buffer = var[1];
                varR = split(buffer, "+");
                for (int i = 0; i < varR.size(); i++)
                {
                    varR[i] = removespace(varR[i]);
                    //std::cout << varR[i] << "   620\n";
                    if (findVector(varR[i]) == "values")
                    {
                        if (findVector(var[0]) == " ")
                        { // variable not in any vectors
                            //std::cout <<"checkpoint831"<<" n\n";
                            for (int j = 0; j < values[findTarget(varR[i])].second.size(); j++)
                            {
                                //std::cout <<"checkpoint1012"<<" n\n";
                                bufferVecInt.push_back(values[findTarget(varR[i])].second[j]);
                                // sort(bufferVecInt.begin(),bufferVecInt.end());
                            }
                            if (i == varR.size() - 1)
                            {
                                values.push_back(std::make_pair(var[0], bufferVecInt));
                            }
                            // std::cout<<values.size()<<"\n";
                        }
                        else if(findVector(var[0]) == "values"){
                            if(i!=varR.size()-1){
                                for (int j = 0; j < values[findTarget(varR[i])].second.size(); j++)
                                {
                                    bufferVecInt.push_back(values[findTarget(varR[i])].second[j]);
                                }
                                continue;
                            }
                            else{
                                for (int j = 0; j < values[findTarget(varR[i])].second.size(); j++)
                                {
                                    bufferVecInt.push_back(values[findTarget(varR[i])].second[j]);
                                }
                                values[findTarget(var[0])].second=bufferVecInt;
                            }
                        }
                        else{  //
                            //std::cout <<"checkpoint845"<<" n\n";
                            //bufferVecInt += values[findTarget(varR[i])];
                            for (int j = 0; j < values[findTarget(varR[i])].second.size(); j++)
                            {
                                //std::cout<<"values.size()"<<" \n";
                                //var[0].push_back(values[findTarget(varR[i])].second[j]);
                                //std::cout <<"checkpoint851"<<" n\n";
                                values[findTarget(var[0])].second.push_back(values[findTarget(varR[i])].second[j]);
                                //std::cout <<"checkpoint853"<<" n\n";
                                //std::cout<<findVector("listZ")<<"  "<<var[0]<<"  n\n";
                            }
                            //for (int x = 0; )

                        }
                    }
                }
                //std::cout <<values[findTarget("listZ")].second.size()<<" listzsize()\n";
                if(findVector(var[0])!="values"){
                    for (int i = 0; i < variables.size(); i++)
                    {
                        if (variables[i].first != var[0] && i == variables.size() - 1)
                        {
                            variables.push_back(std::make_pair(var[0], 0));
                            //std::cout << "check656" << "\n";
                        }
                    }

                    for (int i = 0; i < operations.size(); i++)
                    {
                        if (buffer.find(operations[i]) != -1)
                        {
                            // varR=split(buffer,operations[i]);
                            // std::cout <<var[0]<<"hey"<<var[1];
                            for (int j = 0; j < varR.size(); j++)
                            {
                                varR[j] = removespace(varR[j]);
                            }
                            for (int j = 0; j < varR.size(); j++)
                            {
                                if (is_number(varR[j]) == 1)
                                {
                                    tmp = tmp + stoi(varR[j]);
                                    //std::cout << tmp <<" varR[j] "<<varR[j]<<"  665tmpn\n";
                                }
                                else
                                {
                                    // std::cout << variables[0].first << "n\n";
                                    tmp=tmp+variables[findTarget(varR[j])].second;
                                    /*for (int i = 0; i < variables.size(); i++)
                                    {
                                        // std::cout <<"checkpoint6 "<<lineNo<<" \n";
                                        // std::cout <<variables[i].first<< "n  on lineno: "<<lineNo<<"\n";
                                        // std::cout <<var[j]<< "n  on lineno: "<<lineNo<<"\n";
                                        if (variables[i].first == varR[j])
                                        {
                                            tmp = tmp + variables[i].second;
                                            std::cout << tmp <<" varR[j] "<<varR[j]<<"  678tmpn\n";
                                            // std::cout << tmp << "on lineno: "<<lineNo<<"\n";
                                        }
                                    }*/
                                }
                            }
                        }
                    }

                    for (int i = 0; i < variables.size(); i++)
                    {
                        if (variables[i].first == var[0])
                        {
                            variables[i].second = tmp;
                        }
                        /*
                        else if (i == variables.size() - 1)
                        {
                            variables.push_back(std::make_pair(var[0], tmp));
                        }*/
                    }
                    // if(var[1].find())
                    //  yes +-*/ on the right of =
                }
            }
            //std::cout<<values[findTarget("head2")].second.size()<<" 1182\n";
            //std::cout << actualCodes << "\n";
        }
        else if (actualCodes.starts_with("if"))
        { // for if statement
            ifFlag = true;
            int index;
            std::string buffer;
            std::vector<std::string> temp;
            //std::cout <<"checkpoint927"<<" n\n";
            if (actualCodes.find('(') != -1) // contains () for if statement
            {
                buffer = actualCodes.substr(actualCodes.find('(') + 1, actualCodes.find(')') - actualCodes.find('(') - 1);
                // std::cout<<buffer<<"\n";
                for (int i = 0; i < SIGNS.size(); i++){
                    if(buffer.find(SIGNS[i])!=-1){
                        index = i;
                        break;
                    }
                }
                temp = split(buffer, SIGNS[index]);
                
            }
            else
            { // no () for if statement
                buffer = actualCodes.substr(actualCodes.find(" ") + 1);
                buffer = buffer.erase(buffer.find(":"));
                for (int i = 0; i < SIGNS.size(); i++){
                    if(buffer.find(SIGNS[i])!=-1){
                        index = i;
                        break;
                    }
                }
                temp = split(buffer, SIGNS[index]);
            }
            //std::cout <<"checkpoint1207"<<" n\n";
            for (int i = 0; i < temp.size(); i++)
            {
                temp[i]=removespace(temp[i]);
            }
            //std::cout <<"checkpoint1217"<<" n\n";
            if (is_number(temp[0]) != 1)//not a NO. on the left of condition
            {
                if(temp[0].find("[")!=-1){// yes [] on the left of condition
                    std::string listName = temp[0].substr(0, (temp[0].find("[")));
                    int listIndex = getIndex(temp[0]);
                    boolresult= ifstatement(values[findTarget(listName)].second[listIndex],index,stoi(temp[1]));
                }
                else{
                    //std::cout <<"checkpoint1226"<<" n\n";
                    boolresult = ifstatement(variables[findTarget(temp[0])].second, index, stoi(temp[1]));
                    //std::cout <<"checkpoint1228"<<" n\n";
                }
            }
            else
            {
                //std::cout << "checkpoint1"<< "n \n";
                boolresult = ifstatement(stoi(temp[0]), index, stoi(temp[1]));
            }
            //std::cout <<"checkpoint1234"<<" n\n";
             //std::cout << boolresult<<" torf n\n";
            continue;
        }
        else if(actualCodes.find("=")==-1 && !actualCodes.starts_with("print") && actualCodes.find("(") != -1 && actualCodes.find(")") != -1){
            //std::cout <<"checkpoint1300"<<" n\n";
            run_function_N(actualCodes);
        }
    }
}

int main(int argc, char *argv[])
{
    std::string filename;
    //std::vector<std::string> python;
    if (argc == 2)
    {
        filename = argv[1];
        /*if(filename.find(".py")==-1)
            filename = filename + ".py";*/
    }
    else
    {
        std::cout << "ERROR! could not open the file" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream inputfile(filename);
    if (!inputfile.is_open())
    {
        std::cerr << "ERROR! could not open the file ' " << filename << "'" << std::endl;
        return EXIT_FAILURE;
    }

    while (getline(inputfile, words))
        python.push_back(words);
    inputfile.close();
    main_function(python);

    return 0;
}