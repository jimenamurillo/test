
// ExpressionConverter.cpp

#include "ExpressionConverter.h"
#include <iostream>
#include <stack>
#include <list>
#include <sstream>
#include <locale>
#include <cctype>
#include <regex>

// given a string of infix return RPN
//start of Infix2RPN function
string Infix2RPN(string infixRET) {
    stack<string> empSTACK;  
    stringstream infixSTREAM;
    infixSTREAM.str(infixRET);
    string holdinfix;   
    string OUTPUT = ""; //result
    
    
    //Loops through to review each character in the string: checks the type
    while(infixSTREAM >> holdinfix)
    {
        if (isdigit(holdinfix[0]))     //operand loop
        {
            OUTPUT += holdinfix + " ";
        }
        
        else if  ((holdinfix == "("))
        {empSTACK.push(holdinfix);}
        
        else if ((holdinfix == ")"))
        {
            holdinfix = empSTACK.top();
            empSTACK.pop();
            
            //pop result only while stack is not " ==( and is empty
            while(!empSTACK.empty() && holdinfix !=  "(" ) {
                OUTPUT += holdinfix + " ";
                holdinfix = empSTACK.top();
                empSTACK.pop();
            }
        }
        
        //else operand
        else
        {
            while (!empSTACK.empty() && Precedence(empSTACK.top(),holdinfix) ) //check precedence
            {
                OUTPUT += empSTACK.top();
                empSTACK.pop();
            }
            empSTACK.push(holdinfix);
        }
    }
    
    while (!empSTACK.empty()) {
        OUTPUT += empSTACK.top() + " ";
        empSTACK.pop();
    }
    return OUTPUT;
}
//end of Infix2RPN function


//return string of Infix given rpn's and converts to tree
string RPN2Infix(string RPNinput) {
    ArithNode * treePTR = RPN2ExpressionTree(RPNinput);
    return ExpressionTree2Infix(treePTR);
}


//start ExpressionTree2Infix
string ExpressionTree2Infix(ArithNode * treePTR) { //checks if node is leaf node
    if(!treePTR->isOperator) {
        return (std::to_string(treePTR->operand));
    } else{
        return ("(" + ExpressionTree2Infix(treePTR->left) + " "
                + treePTR->binaryOperator + " "
                + ExpressionTree2Infix(treePTR->right) + " " + ")");
    }
}
//end ExpressionTree2Infix



//move so RPN2ExpressionTree can work with it
ArithNode * Infix2ExpressionTree(string INFIXinput) {
    string RPNinput = Infix2RPN(INFIXinput);    //temp
    return RPN2ExpressionTree(RPNinput);
}

ArithNode * RPN2ExpressionTree(string RPNinput) {
    stack<ArithNode*> nodeSTACK;
    string currentSTATE;
    stringstream RPNstream;
    RPNstream.str(string());
    RPNstream.clear();
    RPNstream.str(RPNinput);
    
    while(RPNstream >> currentSTATE)
    {
        if(isdigit(currentSTATE[0]))
        {
            int temp_int = stoi(currentSTATE);
            ArithNode * nptr = new ArithNode(temp_int);
            nodeSTACK.push(nptr);
        } else {
            char temp_operator = currentSTATE[0];
            ArithNode * nptr = new ArithNode(temp_operator);
            nptr -> right = nodeSTACK.top();
            nodeSTACK.pop();
            nptr -> left = nodeSTACK.top();
            nodeSTACK.pop();
            nodeSTACK.push(nptr);
        } }
    return nodeSTACK.top();
}

//return integer according to infix
int EvaluateInfix(string INFIXinput) {
    string RPNinput = Infix2RPN(INFIXinput); //temp
        return EvaluateRPN(RPNinput);
}
//end infix


// start of: from rpn string, find and evaluate int then return it
int EvaluateRPN(string RPNinput) {
    int operONE, operTWO, OUTPUT;
    string holdinfix;
    stack<int> operSTACK;

    stringstream infixSTREAM;
    infixSTREAM.str(RPNinput);
    
    //loop the stream
    while(infixSTREAM >> holdinfix) {
        if (isdigit(holdinfix[0])) {
            operSTACK.push( (stoi(holdinfix)) );
        } else {
            operTWO = (operSTACK.top());
            operSTACK.pop();
            operONE = operSTACK.top();
            operSTACK.pop();
            OUTPUT = evaluateRPN_helper(holdinfix, operONE, operTWO);
            operSTACK.push(OUTPUT);
        }
    }
    return operSTACK.top();
}
// end analizing rpn function


//Precedence
int Precedence (string op1, string op2) {
    int oper1Prec = GetPrecedence(op1);
    int oper2Prec = GetPrecedence(op2);

    return (oper1Prec > oper2Prec) ? true: false;
}
//end Precedence


//start getPrecedence
int GetPrecedence (string op) {
    int level = 0;
    //precedence
    if (op == "+" || op == "-")
        level = 1;      //lower precedence
    else if (op == "*" || op == "/")
        level = 2;      //lower precedence
    return level;
}
//end getPrecedence

//if function that returns correct results
int evaluateRPN_helper (string op, int oper1, int oper2) {
    if (op == "*")
        return oper1 * oper2;
    else if (op == "/")
        return oper1 / oper2;
    else if (op == "+")
        return oper1 + oper2;
    else
        return oper1 - oper2;
}
//end

// if function on operators
bool isOperator(char Operators) {
    if (Operators == '+' || Operators == '-'
        || Operators == '*' || Operators == '/')
        return true;
    return false;
}
//end
