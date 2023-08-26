#include <iostream>
#include <fstream>
#include <regex>

inline std::string trim(std::string str);

bool is_number(const std::string &s);

std::vector<std::string> split(std::string s, std::string delimiter);

void handle_statement(const auto &line,
                      std::vector<std::string> &variables,
                      std::vector<std::string> &functions,
                      std::vector<std::string> &params,
                      std::vector<std::string> &types,
                      std::vector<std::string> &values,
                      std::vector<std::vector<std::string>> &arrays,
                      int &line_no
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
const std::string NO_SUCH_VARIABLE = "Error! No Such Variable - Line: ";
const std::string SYNTAX_PRINT = "Error! Syntax dismissed for print function - Line: ";
const std::string SYNTAX_SIGN = "Error! Syntax dismissed for comparison sign! - Line: ";
const std::string VARIABLE_NAME = "Error! Syntax dismissed for variable name! - Line: ";
const std::string SYNTAX_FUNCTION = "Error! Syntax dismissed for function! - Line: ";

/*___ CONST VALUES ___*/
const std::string SIGNS[] = {">=", "<=", ">", "<", "!=", "=="};

int64_t onetime = 0;

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
    bool result = false;
    bool elseRun = false;
    std::string temp_func_var = "";
    std::string temp_func_param = "";
    std::string temp_func_desc = "";
    int line_no = 1;

    for (const auto &line: lines) {
        line_no++;
        if(line.starts_with("#")){
            line_no++;
            continue;
        }
        if (!def_flag && temp_func_desc != "") {
            types.push_back("function");
            values.push_back(std::to_string(functions.size()));
            variables.push_back(temp_func_var);
            functions.push_back(temp_func_desc);
            params.push_back(temp_func_param);
            temp_func_desc = "";
            std::string temp_func_param = "";
            temp_func_var = "";
        }
        if (if_flag) {
            if (!line.starts_with("    ") && !line.starts_with("else"))
                if_flag = false;
        }
        if (line.find("def") != -1 || def_flag) {
            def_flag = true;
            if (line.length() <= 0 && def_flag) {
                def_flag = false;
                continue;
            }
            std::string trimmed_line = trim(line);
            if (trimmed_line.find("(") != -1 && trimmed_line.find(")") != -1 && trimmed_line.ends_with(":")) {
                temp_func_var = trimmed_line.substr(4, trimmed_line.find("(") - 4);
                temp_func_param = trim(line).substr(trim(line).find("(") + 1,
                                                    trim(line).find(")") - trim(line).find("(") - 1);
            }

            if (line.find("    ") != -1) {
                def_flag = true;
                temp_func_desc += "\n" + line;
            }
        } else if (line.find("print") != -1) {
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
                                }
                                else {
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
                            first_var = trim(trimmed_line.substr(0, trimmed_line.find(SIGNS[i])));
                            if (is_number(first_var))
                                first_val = std::stoi(first_var);
                            else {
                                for (int i = 0; i < variables.size(); ++i) {
                                    if (first_var == variables[i])
                                        first_val = std::stoi(values[i]);
                                }
                            }

                            second_var = trimmed_line.substr(trimmed_line.find(SIGNS[i]) + 2, trimmed_line.length());
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
                            if (SIGNS[i] == ">=") {
                                if (first_val >= second_val)
                                    result = true;
                            } else if (SIGNS[i] == "<=") {
                                if (first_val <= second_val)
                                    result = true;
                            } else if (SIGNS[i] == ">") {
                                if (first_val > second_val)
                                    result = true;
                            } else if (SIGNS[i] == "<") {
                                if (first_val < second_val)
                                    result = true;
                            } else if (SIGNS[i] == "!=") {
                                if (first_val != second_val)
                                    result = true;
                            } else if (SIGNS[i] == "==") {
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
            } else if (line.find("else") != -1) {
                if (!result)
                    elseRun = true;
                result = false;
            } else {
                if (result) {
                    handle_statement(line, variables, functions, params, types, values, arrays, line_no);
                } else {
                    if (elseRun)
                        handle_statement(line, variables, functions, params, types, values, arrays, line_no);
                }
            }
        } else if (line.find("=") != -1 || line.find("(")) {
            if(line==""){
                continue;
            }
            handle_statement(line, variables, functions, params, types, values, arrays, line_no);
        } else {
            // ignore
        }
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
                      int &line_no
) {
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
        std::cout << VARIABLE_NAME << line_no << std::endl;
    } else {
        name = trim(name);
        if (name.find("[") != -1) {
            item_num = std::stoi(trim(name.substr(name.find("[") + 1, 1)));
            name = name.substr(0, name.find("["));
            arr_assign = true;
        }
        if (std::find(variables.begin(), variables.end(), name) == variables.end()) {
            variables.push_back(name);
            //here array still empty
        } else {
            var_exist = true;
        }
    }
    if (value.find("+") != -1) {
        // save the added value
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
            for (int i = 0; i < variables.size(); ++i)
                if (trim(variables[i]) == trim(sub1)) {
                    sub11 = std::stoi(values[i]);
                    if (types[i] == "array") {
                        array1_flag = true;
                        arr1 = arrays[sub11];
                    }
                }

        if (is_number(sub2))
            sub22 = std::stoi(sub2);
        else
            for (int i = 0; i < variables.size(); ++i)
                if (trim(variables[i]) == trim(sub2)) {
                    sub22 = std::stoi(values[i]);
                    if (types[i] == "array") {
                        array2_flag = true;
                        arr2 = arrays[sub22];
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

            types.push_back("array");
            values.push_back(std::to_string(arrays.size()));
            arrays.push_back(result);
        } else {
            sum = sub11 + sub22 + sub33;
            types.push_back("int");
            if (var_exist) {
                for (int i = 0; i < variables.size(); ++i) {
                    if (variables[i] == name)
                        values[i] = std::to_string(sum);
                }
            } else {
                values.push_back(std::to_string(sum));
            }
        }

    } else {
        // no number
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
            }

            //onetime = std::stoll(value.substr(0, value.find("]")));

            if (value.find(",") != -1) {
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

                value = value.substr(value.find("[") + 1);

                if (sub_var == "" ) {
                    sub_val = "";
                    types.push_back("array");
                    std::vector<std::string> std;
                    arrays.push_back(std);
                    values.push_back(std::to_string(arrays.size()-1));
                } else {
                    sub_val = value.substr(0, value.length() - 1);

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

                    } else {
                        for (int i = 0; i < variables.size(); ++i) {
                            if (sub_var == variables[i] && types[i] == "array")
                                sub_res = std::stoi(arrays[std::stoi(values[i])][std::stoi(sub_val)]);
                        }
                        types.push_back("int");
                        values.push_back(std::to_string(sub_res));
                    }
                }

            }

        } else {
            if (value.find("(") != -1 && value.find(")") != -1) {
                std::string func_name = value.substr(0, value.find("("));
                func_name = trim(func_name);
                std::string func_param = value.substr(value.find("(") + 1);
                func_param = func_param.substr(0, func_param.length() - 1);
                func_name = trim(func_name);
                func_param = trim(func_param);

                for (int i = 0; i < variables.size(); ++i) {
                    if (func_name == variables[i] && types[i] == "function") {
                        std::string result = std::to_string(
                                handle_function(functions[std::stoi(values[i])], params[std::stoi(values[i])],
                                                values[i], line_no));
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
            } else {

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
                } else {
                    types.push_back("int");
                    values.push_back(value);
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
        function = trim(function.substr(2));
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
                    std::string op2 = trim(split(subline[j], "+")[1]);
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
                    break;
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
    return 0;
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