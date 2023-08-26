#include <iostream>
#include <fstream>
#include <regex>

inline std::string trim(std::string str);

bool is_number(const std::string &s);

std::vector<std::string> split(std::string s, std::string delimiter);

std::string removeSpecialCharacter(std::string s);

void handle_statement(const auto &line,
                      std::vector<std::string> &variables,
                      std::vector<std::string> &functions,
                      std::vector<std::string> &params,
                      std::vector<std::string> &types,
                      std::vector<std::string> &values,
                      std::vector<std::vector<std::string>> &arrays,
                      int &line_no,
                      bool state
);

int handle_function(std::string function, std::string params, std::string param, int &line_no);


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

/*___ CONST ERROR MESSAGES ___*/
const std::string INPUT_ARG_ERROR = "Error! INPUT argument is not based on template string as below:\n-o test-cases/tc01";
const std::string INPUT_FILE_ERROR = "Error! Could not open the file - Line: ";
const std::string NO_SUCH_VARIABLE = "Error! Type Error - Line: ";
const std::string SYNTAX_PRINT = "Error! Syntax dismissed for print function - Line: ";
const std::string SYNTAX_SIGN = "Error! Syntax dismissed for comparison sign! - Line: ";
const std::string VARIABLE_NAME = "Error! Syntax dismissed for variable name! - Line: ";
const std::string SYNTAX_FUNCTION = "Error! Syntax dismissed for function! - Line: ";

/*___ CONST VALUES ___*/
const std::string SIGNS[] = {">=", "<=", ">", "<", "!=", "=="};

int64_t onetime = 0;
std::vector<std::string> funcLine;

int main(int argc, char *argv[]) {

    /*___ FIELDS ___*/
    std::string filename;
    std::vector<std::string> lines;
    std::vector<std::string> function;
    std::vector<std::string> operation;
    std::vector<std::string> types;
    std::vector<std::string> variables;
    std::vector<std::string> functions;
    std::vector<std::string> params;
    std::vector<std::string> values;
    std::vector<std::vector<std::string>> arrays;
    std::ofstream error_log("error.log"); // error-log file

    /*___ PROCESS ARGUMENT ___*/
    /*where input is*/
    if (argc == 2) {
        filename = argv[1];
        //std::cout << argv << "bruh what";
        if (filename.find(".py") == -1) {
            filename = filename + ".py";
        }
    } else {
        std::cout << INPUT_ARG_ERROR << std::endl;
        return EXIT_FAILURE;
    }

    /*___ READ FILE ___*/
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << INPUT_FILE_ERROR << filename << "'" << std::endl;
        return EXIT_FAILURE;
    }
    std::string line;
    while (getline(input_file, line)) {
        lines.push_back(line);
    }
    input_file.close();

    bool def_flag = false;
    bool if_flag = false;
    bool result = 1;
    bool elseRun = false;
    std::string temp_func_var = "";
    std::string temp_func_param = "";
    std::string temp_func_desc = "";
    int line_no = -1;

    int64_t funcLineCounter = 0;

    for (const auto &line: lines) {
        line_no++;
        if(line.starts_with("#") || line == ""){
            //line_no++;
            continue;
        }
        //one more in02 here
        if (line.find("def") != -1 || def_flag) {
            def_flag = true;
            /* more in02 stuff here
            if (line.length() <= 0 && def_flag) {
                def_flag = false;
                continue;
            }
            std::string trimmed_line = trim(line);
            if (trimmed_line.find("(") != -1 && trimmed_line.find(")") != -1 && trimmed_line.ends_with(":")) {
                temp_func_var = trimmed_line.substr(4, trimmed_line.find("(") - 4);
                temp_func_param = trim(line).substr(trim(line).find("(") + 1,trim(line).find(")") - trim(line).find("(") - 1);
            }

            if (line.find("    ") != -1) {
                def_flag = true;
                temp_func_desc += "\n" + line;
            }
            */
        }
        //some in02 here 1 and 4 works
        if (def_flag && temp_func_desc != "") {
            if (funcLineCounter != 0) {
                //values.push_back(std::to_string(functions.size()));
                //variables.push_back(temp_func_var);
                functions.push_back(functions.front()+temp_func_desc);
                functions.erase(functions.begin());
                params.push_back(temp_func_param);
                temp_func_desc = "";
                std::string temp_func_param = "";
                temp_func_var = "";
                funcLineCounter++;
            }
            else {
                types.push_back("function");
                values.push_back(std::to_string(functions.size()));
                variables.push_back(temp_func_var);
                functions.push_back(temp_func_desc);
                params.push_back(temp_func_param);
                temp_func_desc = "";
                std::string temp_func_param = "";
                temp_func_var = "";
                funcLineCounter++;
            }
        }
        if (if_flag) {
            if (line.starts_with("    ") || line.starts_with("else")) { //in04 if the last change here
                if_flag = 0;
                result = 1;
            }
        }
        // current change for in02 here had thing to do with in04
        else {
            if (line.find("def") != -1 || def_flag) {
                def_flag = true;
                std::string trimmed_line = trim(line);
                if (trimmed_line.find("(") != -1 && trimmed_line.find(")") != -1 && trimmed_line.ends_with(":")) {
                    temp_func_var = trimmed_line.substr(4, trimmed_line.find("(") - 4);
                    temp_func_param = trim(line).substr(trim(line).find("(") + 1,trim(line).find(")") - trim(line).find("(") - 1);
                }
                if (line.starts_with(" ") != -1) {
                    def_flag = true;
                    temp_func_desc += "\n" + line;
                }
            }
            if (line.find("return") != -1 || lines[line_no].starts_with(" ") == -1) {
                //if (line.length() <= 0 && def_flag) {
                def_flag = false;
                //values.push_back(std::to_string(functions.size()));
                //variables.push_back(temp_func_var);
                functions.push_back(functions.front()+temp_func_desc);
                functions.erase(functions.begin());
                params.push_back(temp_func_param);
                temp_func_desc = "";
                std::string temp_func_param = "";
                temp_func_var = "";
                funcLineCounter++;
                continue;
                //}
            }
            //*********************************printing print here
            else if (line.find("print") != -1) {
                std::string trimmed_line = trim(line);
                if (trimmed_line.ends_with(';'))
                    trimmed_line = trimmed_line.substr(0, -2);
                if (trimmed_line.substr(0, 5) == "print") {
                    std::string sub = trimmed_line.substr(5);
                    if (sub.starts_with('(') && sub.ends_with(')')) {
                        //with the sign here
                        sub = sub.substr(1, sub.length() - 2);
                        if ((sub.starts_with('"') && sub.ends_with('"')) ||
                            (sub.starts_with('\'') && sub.ends_with('\''))) {
                            sub = sub.substr(1, sub.length() - 2);
                            std::cout << sub;
                        } else {
                            if (sub.starts_with('"') && !sub.ends_with('"')) {
                                std::string sub1 = sub.substr(1);
                                sub1 = trim(sub1);
                                sub1 = sub1.substr(0, sub.find(',') - 2);
                                sub = sub.substr(sub.find(",") + 1);
                                sub = trim(sub);
                                //words for the output
                                std::cout << sub1 << " ";
                            }

                            bool flag;
                            //list out here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                            for (int i = 0; i < variables.size(); ++i) {
                                //std::cout << sub;
                                if (variables[i] == sub) {
                                    if (types[i] == "int") {
                                        //numbers for the output
                                        std::cout << values[i];
                                    } else {
                                        std::cout << "[";
                                        /*if (onetime != 0) {
                                            std::cout << onetime;
                                        }*/
                                        for (int j = 0; j < arrays[std::stoi(values[i])].size(); ++j) {
                                            std::cout << arrays[std::stoi(values[i])][j];

                                            if (j != arrays[std::stoi(values[i])].size() - 1)
                                                std::cout << ", ";
                                        }
                                        std::cout << "]";
                                    }
                                    flag = true;
                                }
                            }
                            if (!flag) {
                                std::cout << NO_SUCH_VARIABLE << line_no << std::endl;
                                error_log << NO_SUCH_VARIABLE << line_no << std::endl;
                            }
                        }
                    } else {
                        std::cout << SYNTAX_PRINT << line_no << std::endl;
                        error_log << NO_SUCH_VARIABLE << line_no << std::endl;
                    }
                } else {
                    std::cout << SYNTAX_PRINT << line_no << std::endl;
                    error_log << SYNTAX_PRINT << line_no << std::endl;
                }
                std::cout << std::endl;
            } else if (line.find("if") != -1 || if_flag) {
                std::string mySign;
                if_flag = true;
                if (line.find("if") != -1) {
                    std::string trimmed_line = trim(line);
                    if (trimmed_line.starts_with("if") && trimmed_line.ends_with(":")) {
                        trimmed_line = trimmed_line.substr(3);
                        trimmed_line = trimmed_line.substr(0, trimmed_line.find(":"));
                        trimmed_line = trim(trimmed_line);
                        std::string first_var;
                        int first_val = 0;
                        std::string second_var;
                        int second_val = 0;

                        for (int i = 0; i < sizeof(SIGNS); ++i) {
                            if (trimmed_line.find(SIGNS[i]) != -1) {
                                //first_var = trimmed_line.substr(1);
                                mySign = SIGNS[i];
                                first_var = trim(trimmed_line.substr(0, trimmed_line.find(SIGNS[i])));
                                if (is_number(first_var))
                                    first_val = std::stoi(first_var);
                                else {
                                    for (int i = 0; i < variables.size(); ++i) {
                                        if (first_var == variables[i])
                                            first_val = std::stoi(values[i]);
                                    }
                                }

                                second_var = trimmed_line.substr(trimmed_line.find(SIGNS[i]) + 2,
                                                                 trimmed_line.length());
                                second_var = second_var.substr(0, second_var.find(")"));
                                second_var = trim(second_var);
                                if (is_number(second_var))
                                    second_val = std::stoi(second_var);
                                else {
                                    for (int i = 0; i < variables.size(); ++i) {
                                        if (second_var == variables[i])
                                            second_val = std::stoi(values[i]);
                                    }
                                }
                                if (mySign == ">=") {
                                    if (first_val >= second_val)
                                        result = true;
                                } else if (mySign == "<=") {
                                    if (first_val <= second_val) {
                                        result = true;
                                    } else {
                                        result = 0;
                                    }
                                } else if (mySign == ">") {
                                    if (first_val > second_val)
                                        result = true;
                                } else if (mySign == "<") {
                                    if (first_val < second_val)
                                        result = true;
                                } else if (mySign == "!=") {
                                    if (first_val != second_val)
                                        result = true;
                                } else if (mySign == "==") {
                                    if (first_val == second_val)
                                        result = true;
                                } else {
                                    std::cout << SYNTAX_SIGN << line_no << std::endl;
                                    error_log << SYNTAX_SIGN << line_no << std::endl;
                                }
                                break;
                            }
                        }
                    }
                }
                    //in04 need to next line here------------------------------------------------------------do if else here
                else if (line.find("else") != -1) {
                    if (!result)
                        elseRun = true;
                    result = 1;
                }/*
                else {
                    if (result) {
                        handle_statement(line, variables, functions, params, types, values, arrays, line_no, result);
                    } else {
                        if (elseRun)
                            handle_statement(line, variables, functions, params, types, values, arrays, line_no, result);
                    }
                }*/
            }
            /* else if (line.find("=") != -1 || line.find("(")) {*/
            //if(line==""){
            //    continue;
            //}
        }
        if (result == 1 && def_flag == 0){
            handle_statement(line, variables, functions, params, types, values, arrays, line_no,result);
        }
        else {

        }
        /*} else {
            // ignore
        }*/
    }
    return EXIT_SUCCESS;
}

// function for trimming white spaces
inline std::string trim(std::string str) {
    str.erase(str.find_last_not_of(' ') + 1);             //suffixing spaces
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    return str;
}

void handle_statement(const auto &line,
                      std::vector<std::string> &variables,
                      std::vector<std::string> &functions,
                      std::vector<std::string> &params,
                      std::vector<std::string> &types,
                      std::vector<std::string> &values,
                      std::vector<std::vector<std::string>> &arrays,
                      int &line_no,
                      bool state
) {
    if (state != 1) {
        return;
    }
    bool var_exist = false;
    bool sub3_falg = false;
    bool arr_assign = false;
    int item_num;
    std::string trimmed_line = trim(line);
    if (trimmed_line.ends_with(';'))
        trimmed_line = trimmed_line.substr(0, -2);
    std::string name = trimmed_line.substr(0, trimmed_line.find("="));
    std::string value = trim(trimmed_line.substr(trimmed_line.find("=") + 1, trimmed_line.length()));
    if (name.find("\\") != -1 || name.find(":") != -1 || name.find("\"") != -1 || name.find("'") != -1 ||
        name.find(";") != -1 || name.find("/") != -1) {
        //std::cout << VARIABLE_NAME << line_no << std::endl;
        //in04 last bit for it to work!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    } else {
        name = trim(name);
        if (name.find("[") != -1) { //have [ here
            item_num = std::stoi(trim(name.substr(name.find("[") + 1, 1)));
            //name = name.substr(0, name.find("[")); in04 final last change here why????
            arr_assign = true;
        }
        //more in04 here with 321 CHECK IF THIS IS BROKEN---CHECK HERE
        if (std::find(variables.begin(), variables.end(), name.substr(0,name.find("["))) == variables.end() && name != "") {
            variables.push_back(name);
            //here array still empty
        } else {
            var_exist = true;
        }
    }
    if (value.find("+") != -1) {
        // save the added value
        std::vector<std::string> subs = split(value,"+");
        std::string sub1 = trim(value.substr(0, value.find("+")));
        std::string sub2 = trim(value.substr(value.find("+") + 1, -2));
        std::string sub3;

        if (sub2.find("+") != -1) {
            sub3 = trim(sub2.substr(sub2.find("+") + 1, -2));
            sub2 = trim(sub2.substr(0, sub2.find("+")));
            sub3_falg = true;
        }
        int sub11 = 0;
        int sub22 = 0;
        int sub33 = 0;
        int sum = 0;
        bool array1_flag = false;
        bool array2_flag = false;
        bool array3_flag = false;
        std::vector<std::string> arr1;
        std::vector<std::string> arr2;
        std::vector<std::string> arr3;
        std::vector<std::string> result;
        if (is_number(sub1))
            sub11 = std::stoi(sub1);
        else
            for (int i = 0; i < variables.size(); ++i) //do the adding list here in04
                if (trim(variables[i]) == trim(sub1.substr(0,sub1.find("[")))) { //more in04 final here
                    //last change here in04--------------------------------------------------------------now
                    if (sub1.find("[") != -1) {
                        //here
                        for (int i = 0; i < variables.size(); ++i) {
                            if (trim(variables[i]) == trim(sub1.substr(0,sub1.find("[")))) {
                                //legit in04 part1
                                //sub1 array number is std::stoi(sub1.substr(sub1.find("[")+1,sub1.find("]")-sub1.find("[")-1))
                                sub11 = std::stoll(arrays[i][std::stoi(sub1.substr(sub1.find("[")+1,sub1.find("]")-sub1.find("[")-1))]);
                            }
                        }
                    }
                    //after 1-4 here broke
                    else if (types[i] == "array" && sub1.find("[") == -1) {
                        array1_flag = true;
                        arr1 = arrays[i];
                    }
                    else {
                            sub11 = std::stoll(values[i]);
                    };
                }
        //in04 do i++ here
        if (is_number(sub2))
            sub22 = std::stoi(sub2);
        else
            for (int i = 0; i < variables.size(); ++i)
                if (trim(variables[i]) == sub2.substr(0,trim(sub2).find("["))) {
                    //in06 here
                    if (types[i] == "array" && sub2.find("[") != -1) {
                        array2_flag = true;
                        arr2 = arrays[i];
                        int test = std::stoi(sub2.substr(sub2.find("[")+1,sub2.find("]")-sub2.find("[")-1));
                        sub22 = std::stoll(arrays[i][std::stoi(sub2.substr(sub2.find("[")+1,sub2.find("]")-sub2.find("[")-1))]);
                        //sub2.substr(trim(sub2).find("[")+1,trim(sub2).find("]")-trim(sub2).find("[")-1))
                    }
                    else if (types[i] == "array") {
                        array2_flag = true;
                        arr2 = arrays[i];
                        sub22 = std::stoll(values[i]);
                    }
                    else {
                        sub22 = std::stoi(values[i]); //last last change here in04
                    }
                }
        if (sub3_falg) {
            bool inside_flag = false;
            if (is_number(sub3))
                sub33 = std::stoi(sub3);
            else if (sub3.find("[") != -1) {
                item_num = std::stoi(sub3.substr(sub3.find("[") + 1, 1));
                sub3 = sub3.substr(0, sub3.find("["));
                inside_flag = true;
            }
            for (int i = 0; i < variables.size(); ++i)
                if (trim(variables[i]) == trim(sub3)) {
                    sub33 = std::stoi(values[i]);
                    if (types[i] == "array") {
                        if (inside_flag) {
                            array3_flag = false;
                            sub33 = std::stoi(arrays[sub33][item_num]);
                        } else {
                            array3_flag = true;
                            arr3 = arrays[sub33];
                        }
                    }
                }
        }
        if (array1_flag && array2_flag) {
            for (int i = 0; i < arr1.size(); ++i) {
                result.push_back(trim(arr1[i]));
            }
            for (int i = 0; i < arr2.size(); ++i) {
                result.push_back(trim(arr2[i]));
            }
            //addding array here final change in04 here
            for (int i = 0; i < variables.size(); ++i) {
                if (trim(variables[i]) == trim(name)) {
                    arrays[i] = result;
                }
            }
            //types.push_back("array");
            //values.push_back(std::to_string(arrays.size()));
            //arrays.push_back(result);
        }
        else {
            //adding array[0] is wrong here in04
            sum = sub11 + sub22 + sub33;
            //in04 stop adding more types here
            //types.push_back("int");
            if (var_exist) {
                //adding listz[0] here-------------------------------------------------------------------------------------------
                if (name.find("[") != -1) {
                    for (int i = 0; i < variables.size(); ++i) {
                        if (trim(variables[i]) == trim(name.substr(0, sub1.find("[")))) {
                            arrays[i][std::stoi(name.substr(name.find("[")+1,name.find("]")-name.find("[")-1))] = std::to_string(sum);
                        }
                    }
                }
                for (int i = 0; i < variables.size(); ++i) {
                    if (variables[i] == name)
                        values[i] = std::to_string(sum);
                }
            } else {
                values.push_back(std::to_string(sum));
                types.push_back("int");
            }
        }
    }
    else {
        // no number
        //change here for in04
        if (value.find("[") != -1 && value.find("]") != -1) {
            value = trim(value);
            //list here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //in01 fix
            if (value.find(",") == -1) {
                //value = value.substr(1, value.length() - 2);
                if(is_number(value.substr(1, value.length() - 2))) {
                    //std::cout << std::stoll(value);
                    std::vector<std::string> vect;
                    vect.push_back(value.substr(value.find("[")+1, value.find("]")-value.find("[")-1));
                    types.push_back("array");
                    //std::cout << value;
                    values.push_back(std::to_string(arrays.size()));
                    arrays.push_back(vect);
                }
                //in04 here fr?
                //std::cout << "bruh";
            }

            //onetime = std::stoll(value.substr(0, value.find("]")));

            if (value.find(",") != -1) {
                //list is pushed to arrays vectors here
                value = value.substr(1, value.length() - 2);
                std::vector<std::string> vect = split(value, ",");
                types.push_back("array");
                //std::cout << value;
                values.push_back(std::to_string(arrays.size()));
                arrays.push_back(vect);
            }
            else {

                //std::cout << value;
                std::string sub_val;
                int sub_res = 0;
                std::string sub_var = value.substr(0, value.find("["));

                //std::string sub_var = value.substr(value.find("["),value.find("]"));
                sub_var = trim(sub_var);
                //std::cout << value << std::endl;

                //value = value.substr(value.find("[") + 1); in04 changed here
                if (sub_var == "" ) {
                    sub_val = "";
                    types.push_back("array");
                    std::vector<std::string> std;
                    std::vector<std::string> nowords{};
                    arrays.push_back(nowords);
                    //arrays.push_back(std);
                    //std::cout << arrays[2].empty();
                    values.push_back(std::to_string(arrays.size()-1));
                }
                else {
                    //possible in02 broke here
                    sub_val = value.substr(value.find("[")+1, value.find("]")-value.find("[") - 1);
                    if (types.size() < variables.size()) {
                        types.push_back("int");
                        std::string ss;
                        int xx;
                        for (int i = 0; i < variables.size(); ++i) {
                            if (variables[i] == sub_val) {
                                xx = std::stoi(values[i]);
                            }
                        }
                        for (int i = 0; i < variables.size(); ++i) {
                            if (variables[i] == sub_var) {
                                values.push_back(arrays[std::stoi(values[i])][xx]);
                            }
                        }
                    }
                    //IN02 LAST ONE IS HERE
                    else {
                        for (int i = 0; i < variables.size(); ++i) {
                            int64_t aNum = -1;
                            if (sub_var == variables[i] && types[i] == "array") {
                                // maybe last in02 change here?-------------------------------------------------------------------------------------------------------
                                aNum = aNum + 1;
                                if(is_number(sub_val)) {
                                    sub_res = std::stoi(arrays[std::stoi(values[i])][std::stoi(sub_val)]);
                                }
                                else {
                                    for (int j = 0; j < variables.size(); ++j) {
                                        if (variables[i] == sub_val) {
                                            sub_res = std::stoll(arrays[std::stoi(values[i])][std::stoll(values[j])]);
                                        }
                                    }
                                }
                            }
                        }
                        types.push_back("int");
                        values.push_back(std::to_string(sub_res));
                    }
                }

            }

        }
        else {
            if (value.find("(") != -1 && value.find(")") != -1) {
                std::string func_name = value.substr(0, value.find("("));
                func_name = trim(func_name);
                std::string func_param = value.substr(value.find("(") + 1);
                func_param = func_param.substr(0, func_param.length() - 1);
                func_name = trim(func_name);
                func_param = trim(func_param);

                for (int i = 0; i < variables.size(); ++i) {
                    if (func_name == variables[i] && types[i] == "function") {
                        std::string result = std::to_string(handle_function(functions[std::stoi(values[i])], params[std::stoi(values[i])],values[i], line_no));
                        if (result != "") {
                            for (int j = 0; j < variables.size(); ++j) {
                                if (func_param == variables[j]) {
                                    values[j] = result;
                                }
                            }
                        } else {
                            std::cout << SYNTAX_FUNCTION << line_no << std::endl;
                        }
                    }
                }
            }
            else {
                //********************************************chcek here too
                if (is_number(value)) {
                    //do nothing
                }
                else {
                    value = removeSpecialCharacter(value);
                }
                if (is_number(name)) {
                    //do nothing
                }
                else {
                    name  = removeSpecialCharacter(name);
                }
                if (arr_assign || var_exist) {
                    for (int i = 0; i < variables.size(); ++i) {
                        if (variables[i] == name) {
                            if (types[i] == "array") {
                                if (is_number(value)) {
                                    arrays[std::stoi(values[i])][item_num] = value;
                                } else {
                                    for (int j = 0; j < variables.size(); ++j) {
                                        if (variables[j] == trim(value)) {
                                            arrays[std::stoi(values[i])][item_num] = values[j];

                                        }
                                    }
                                }
                            } else {
                                values[i] = value;
                            }
                        }
                    }
                }
                //****************************************************************************************************************************************
                else if (name.find("print") == -1) {
                    types.push_back("int");
                    values.push_back(value);
                }
                else{
                    //******************************* MAYBE LAST IN02 CHANGE HERE
                    for (int i = 0; i < variables.size(); ++i) {
                        if (name == variables[i]) {
                            types.push_back("int");
                            values.push_back(value);
                            continue;
                        }
                    }
                }
            }
        }
    }
}

int handle_function(std::string function, std::string params, std::string param, int &line_no) {

    std::vector<std::string> types;
    std::vector<std::string> variables;
    std::vector<std::string> values;
    std::vector<std::vector<std::string>> arrays;

    std::string arg = "";
    int output = 0;

    if (function.starts_with("\n"))
        //in05 chnaged here
        function = trim(function.substr(1));
    std::vector<std::string> line = split(function, "\n");

    for (int i = 0; i < line.size(); ++i) {
        if (line[i].find("return") != -1) {
            return output;
        }
        if (line[i].find("=")) {
            std::vector<std::string> subline = split(line[i], "=");
            for (int j = 1; j < subline.size(); ++j) {
                if (subline[j].find("+")) {
                    std::string op1 = trim(split(subline[j], "+")[0]);
                    int op11 = 0;
                    int op22 = 0;
                    if (is_number(op1)) {
                        //in05 broke here
                        if (typeid(op1).name() == typeid(op11).name()) {
                            std::string nani = "";
                        }
                        op11 = std::stoi(op1);
                    } else {
                        for (int k = 0; k < split(params, ",").size(); ++k) {
                            if (split(params, ",").size() > 1) {
                                if (op1 == split(params, ",")[k]) {
                                    op11 = std::stoi(split(param, ",")[k]);
                                }
                            } else {
                                op11 = std::stoi(trim(param));
                            }
                        }
                    }
                    std::string op2;
                    if (subline[j].find("+") != -1) {
                        op2 = trim(split(subline[j], "+")[1]);  //length error at in05
                    }
                    else {
                        op2 = trim(subline[j]);
                    }
                    if (is_number(op2)) {
                        op22 = std::stoi(op2);
                    } else {
                        for (int k = 0; k < split(params, ",").size(); ++k) {
                            if (split(params, ",").size() > 1) {
                                if (op2 == split(params, ",")[k]) {
                                    op22 = std::stoi(split(param, ",")[k]);
                                }
                            } else {
                                op22 = std::stoi(trim(param));
                            }
                        }
                    }
                    output = op11 + op22;
                    //return output;
                    //break; more in02 here--------------------------------MAY break
                }
                else {
                    if(subline[j].find(",") != -1) {
                        subline[j] = subline[j].substr(1, subline[j].length() - 2);
                        std::vector<std::string> vals = split(subline[j], ",");
                        arrays.push_back(vals);
                        types.push_back("array");
                    }else if(is_number(trim(subline[j]))){
                        values.push_back(subline[j]);
                        types.push_back("int");
                    }
                }
            }
        }
    }
    return output;
}

bool is_number(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
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

std::string removeSpecialCharacter(std::string s) {
    for (int i = 0; i < s.size(); i++) {

        // Finding the character whose
        // ASCII value fall under this
        // range
        if (s[i] < 'A' || s[i] > 'Z' &&
                          s[i] < 'a' || s[i] > 'z')
        {
            // erase function to erase
            // the character
            s.erase(i, 1);
            i--;
        }
    }
    //std::cout << s;
    return s;
}