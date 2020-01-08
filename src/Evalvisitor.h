#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3BaseVisitor.h"
#include "bigInteger.h"
#include <map>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <stdio.h>


std::vector<std::map<std::string,antlrcpp::Any>>var;
std::map<std::string, Python3Parser::ParametersContext*>par;
std::map<std::string, Python3Parser::SuiteContext*>sui;

//返回以str为名的any
antlrcpp::Any tel(std::string str)
{
    if (var[var.size()-1].count(str)) return var[var.size()-1][str];
    if (var[0].count(str)) return var[0][str];
}

//修改以str为名的any的值
void chan(std::string str, antlrcpp::Any a)
{
    var[var.size()-1][str] = a;
    return;
}

//print函数
void anyPrint(antlrcpp::Any tmp)
{
    if (tmp.is<std::string>()) {
        std::string str2;
        str2 = tmp.as<std::string>();
        if (str2[0] == '"') {
            std::cout << std::string(str2,1,str2.length()-2);
            return;
        }
        if (str2 == "None") {
            std::cout << "None";
            return;
        }
        else {
            if (tel(str2).is<std::string>()) {
                if (tel(str2).as<std::string>()[0] == '"') {
                    std::cout << std::string(tel(str2).as<std::string>(),1,tel(str2).as<std::string>().length()-2);
                    return;
                }
                if (tel(str2).as<std::string>() == "None") {
                    std::cout << "None";
                    return;
                }
            }
            if (tel(str2).is<bigInteger>()) {
                std::cout << tel(str2).as<bigInteger>();
                return;
            }
            if (tel(str2).is<bool>())
                if (tel(str2).as<bool>()) {std::cout << "True"; return;}
                else {std::cout << "False"; return;}
            if (tel(str2).is<double>()) {
                std::cout << fixed << setprecision(6) << tel(str2).as<double>();
                return;
            }
        }
    }
    if (tmp.is<bigInteger>()) {
        bigInteger bigint;
        bigint = tmp.as<bigInteger>();
        std::cout << bigint;
        return;
    }
    if (tmp.is<double>()) {
        std::cout << fixed << setprecision(6) << tmp.as<double>();
        return;
    }
    if (tmp.is<bool>()) {
        bool bl;
        bl = tmp.as<bool>();
        if (bl) {std::cout << "True"; return;}
        else {std::cout << "False"; return;}
    }
}

//将一个any转化成bool
bool transbool(antlrcpp::Any tmp)
{
    bool tmpbl;
    if (tmp.is<bool>()) tmpbl = tmp.as<bool>();
    if (tmp.is<bigInteger>()) tmpbl = tmp.as<bigInteger>();
    if (tmp.is<double>()) tmpbl = tmp.as<double>();
    if (tmp.is<std::string>()) {
        std::string str1 = tmp.as<std::string>();
        if (str1 == "None") tmpbl = false;
        if (str1[0] == '"') tmpbl = str1.length()-2;
        if (str1[0] != '"') {
            antlrcpp::Any tmp2 = tel(str1);
            if (tmp2.is<bool>()) tmpbl = tmp2.as<bool>();
            if (tmp2.is<bigInteger>()) tmpbl = tmp2.as<bigInteger>();
            if (tmp2.is<double>()) tmpbl = tmp2.as<double>();
            if (tmp2.is<std::string>()) {
                if (str1 == "None") tmpbl = false;
                if (str1[0] == '"') tmpbl = str1.length()-2;
            }
        }
    }
    return tmpbl;
}

bigInteger transint(antlrcpp::Any tmp)
{
    bigInteger in;
    if (tmp.is<bigInteger>()) in = tmp.as<bigInteger>();
    if (tmp.is<bool>()) {
        if (tmp.as<bool>()) in = 1;
        else in = 0;
    }
    if (tmp.is<double>()) in = (int)tmp.as<double>();
    if (tmp.is<std::string>()) {
        std::string str = tmp.as<std::string>();
        if (str == "None") in = 0;
        else if (str[0] == '"') {
            str = std::string(str, 1, str.length()-2);
            if (str[0] == '-') {
                str = std::string(str, 1, str.length()-1);
                in = bigInteger(str, str.length(), -1);
            } else if (str[0] == '0') {
                in = bigInteger(str, 1, 0);
            } else {
                in = bigInteger(str, str.length(), 1);
            }
        }
        else {
            antlrcpp::Any tmp0 = tel(str);
            if (tmp0.is<bigInteger>()) in = tmp0.as<bigInteger>();
            if (tmp0.is<bool>()) {
                if(tmp0.as<bool>()) in = 1;
                else in = 0;
            }
            if (tmp0.is<double>()) in = (int)tmp0.as<double>();
            if (tmp0.is<std::string>()) {
                std::string str0 = tmp0.as<std::string>();
                if (str0 == "None") in = 0;
                else if(str0[0] == '"') {
                    str0 = std::string(str0, 1, str0.length()-2);
                    if (str0[0] == '-') {
                        str0 = std::string(str0, 1, str0.length()-1);
                        in = bigInteger(str0, str0.length(), -1);
                    } else if (str0[0] == '0') {
                        in = bigInteger(str0, 1, 0);
                    } else {
                        in = bigInteger(str0, str0.length(), 1);
                    }
                }
            }
        }
    }
    return in;
}

double transflo(antlrcpp::Any tmp)
{
    double dou;
    if (tmp.is<double>()) dou = tmp.as<double>();
    if (tmp.is<bigInteger>()) dou = (double)tmp.as<bigInteger>();
    if (tmp.is<bool>()) dou = (double)tmp.as<bool>();
    if (tmp.is<std::string>()) {
        std::string str = tmp.as<std::string>();
        if (str == "None") dou = 0;
        else if (str[0] == '"') {
            str = std::string(str, 1, str.length()-2);
            dou = std::atof(str.c_str());
        }
        else {
            antlrcpp::Any tmp0 = tel(str);
            if (tmp0.is<double>()) dou = tmp0.as<double>();
            if (tmp0.is<bigInteger>()) dou = (double)tmp0.as<bigInteger>();
            if (tmp0.is<bool>()) dou = (double)tmp0.as<bool>();
        }
    }
    return dou;
}

std::string transstr(antlrcpp::Any tmp)
{
    std::string str;
    if (tmp.is<std::string>()) {
        str = tmp.as<std::string>();
        if (str[0] != '"') {
            antlrcpp::Any tmp0 = tel(str);
            if (tmp0.is<std::string>()) str = tmp0.as<std::string>();
            if (tmp0.is<bigInteger>()) str = '"' + (std::string)tmp0.as<bigInteger>() + '"';
            if (tmp0.is<double>()) str = '"' + std::to_string(tmp0.as<double>()) + '"';
            if (tmp0.is<bool>()) {
                if (tmp0.as<bool>()) str = '"' + "True" + '"';
                else str = '"' + "False" + '"';
            }
        }
    }
    if (tmp.is<bigInteger>()) str = '"' + (std::string)tmp.as<bigInteger>() + '"';
    if (tmp.is<double>()) str = '"' + std::to_string(tmp.as<double>()) + '"';
    if (tmp.is<bool>()) {
        if (tmp.as<bool>()) str = '"' + "True" + '"';
        else str = '"' + "False" + '"';
    }
    return str;
}

class EvalVisitor: public Python3BaseVisitor {
    antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
        //std::cout << "fileinput" << std::endl;
        std::map<std::string, antlrcpp::Any> glo;
        var.push_back(glo);
        for (int i = 0;i < ctx->stmt().size();++ i)
            visit(ctx->stmt(i));
        var.pop_back();
        return 0;
    }

    antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        //std::cout << "funcdef" << std::endl;
        std::string str = ctx->NAME()->toString();
        par[str] = ctx->parameters();
        sui[str] = ctx->suite();
        return 0;
    }

    antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        //std::cout << "parameters" << std::endl;
        if (ctx->typedargslist()) return visit(ctx->typedargslist());
        vector<antlrcpp::Any>vec;
        return vec;
    }

    antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        vector<antlrcpp::Any>vec;
        int siz1 = ctx->tfpdef().size(), siz2 = ctx->test().size();
        for (int i = 0;i < siz1-siz2;++ i) {
            std::string str = ctx->tfpdef(i)->NAME()->toString();
            chan(str, (int)0);
            vec.push_back(str);
        }
        for (int i = siz1-siz2;i < siz1;++ i) {
            std::string str = ctx->tfpdef(i)->toString();
            antlrcpp::Any tmp = visit(ctx->test(i));
            chan(str, tmp);
            vec.push_back(str);
        }
        return vec;
    }

    antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
        //std::cout << "tfpdef" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        //std::cout << "stmt" << std::endl;
        if (ctx->compound_stmt()) return visit(ctx->compound_stmt());
        if (ctx->simple_stmt()) return visit(ctx->simple_stmt());
    }

    antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        //std::cout << "simplestmt" << std::endl;
        return visit(ctx->small_stmt());
    }

    antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        //std::cout << "smallstmt" << std::endl;
        if (ctx->expr_stmt()) return visit(ctx->expr_stmt());
        if (ctx->flow_stmt()) {
            antlrcpp::Any tmp = visit(ctx->flow_stmt());
            return tmp;
        }
    }

    antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        //std::cout << "" << std::endl;
        if (!ctx->augassign()) {
            int s = ctx->testlist().size();
            if (s > 1) {
                std::vector<antlrcpp::Any> atm0 = visit(ctx->testlist(s-1));
                for (int i = 0;i < atm0.size();++ i) {
                    if (atm0[i].is<std::string>()) {
                        std::string tmp = atm0[i].as<std::string>();
                        if (tmp[0] != '"') atm0[i] = tel(tmp);
                    }
                }
                
                for (int i = 0;i < s - 1;++ i) {
                    std::vector<antlrcpp::Any> atm = visit(ctx->testlist(i));
                    for (int j = 0;j < atm.size();++ j) {
                        std::string atms;
                        if (atm[j].is<std::string>()) atms = atm[j].as<std::string>();
                        chan(atms, atm0[j]);
                    }
                    
                }
            }
        } else if (ctx->testlist().size() > 1) {
            std::vector<antlrcpp::Any> vec0 = visit(ctx->testlist(0));
            std::vector<antlrcpp::Any> vec1 = visit(ctx->testlist(1));
            std::string tmpL = vec0[0].as<std::string>();
            antlrcpp::Any tmp = vec1[0];
            double tmpdou;
            bigInteger tmpint;
            std::string tmpstr;
            int sig;//0 int,1 double,2 string
            if (tmp.is<bigInteger>()) {tmpint = tmp.as<bigInteger>(); sig = 0;}
            if (tmp.is<double>()) {tmpdou = tmp.as<double>(); sig = 1;}
            if (tmp.is<std::string>()) {
                tmpstr = tmp.as<std::string>();
                if (tmpstr[0] == '"') sig = 2;
                else {
                    if (tel(tmpstr).is<bigInteger>()) {tmpint = tel(tmpstr).as<bigInteger>(); sig = 0;}
                    if (tel(tmpstr).is<double>()) {tmpdou = tel(tmpstr).as<double>(); sig = 1;}
                    if (tel(tmpstr).is<std::string>()) {tmpstr = tel(tmpstr).as<std::string>(); sig = 2;}
                }
            }
            if (sig == 0) {
                if (tel(tmpL).is<bigInteger>()) {
                    bigInteger tmpin = tel(tmpL).as<bigInteger>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpint = tmpint + tmpin;
                        chan(tmpL, tmpint);
                        return tmpint;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpint = tmpin - tmpint;
                        chan(tmpL, tmpint);
                        return tmpint;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpint = tmpint * tmpin;
                        chan(tmpL, tmpint);
                        return tmpint;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = (double)tmpin / (double)tmpint;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpint = tmpin / tmpint;
                        chan(tmpL, tmpint);
                        return tmpint;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpint = tmpin % tmpint;
                        chan(tmpL, tmpint);
                        return tmpint;
                    }
                }
                if (tel(tmpL).is<double>()) {
                    double tmpdo = tel(tmpL).as<double>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpdou = (double)tmpint + tmpdo;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpdou = tmpdo - (double)tmpint;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpdou = (double)tmpint * tmpdo;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = tmpdo / (double)tmpint;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpint = (long long int)tmpdo / (int)tmpint;
                        chan(tmpL, tmpint);
                        return tmpint;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpint = (long long int)tmpdo % (long long int)tmpint;
                        chan(tmpL, tmpint);
                        return tmpint;
                    }
                }
                if (tel(tmpL).is<std::string>()) {
                    tmpstr = tel(tmpL).as<std::string>();
                    tmpstr = std::string(tmpstr, 1, tmpstr.length()-2);
                    std::string str;
                    for (int i = 1;i <= (int)tmpint;++ i) {
                        str += tmpstr;
                    }
                    chan(tmpL, '"' + str + '"');
                    return '"' + str + '"';
                }
            }
            if (sig == 1) {
                if (tel(tmpL).is<bigInteger>()) {
                    bigInteger tmpin = tel(tmpL).as<bigInteger>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpdou = tmpdou + (double)tmpin;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpdou = (double)tmpin - tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpdou = (double)tmpin * tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = (double)tmpin / tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpdou = (int)tmpin / (int)tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpdou = (long long int)tmpin % (long long int)tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                }
                if (tel(tmpL).is<double>()) {
                    double tmpdo = tel(tmpL).as<double>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpdou = tmpdou + tmpdo;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpdou = tmpdo - tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpdou = tmpdo * tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = tmpdo / tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpdou = (long long int)tmpdo / (long long int)tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpdou = (long long int)tmpdo % (long long int)tmpdou;
                        chan(tmpL, tmpdou);
                        return tmpdou;
                    }
                }
            }
            if (sig == 2) {
                std::string tmps = tel(tmpL).as<std::string>();
                tmpstr = std::string(tmpstr, 1, tmpstr.length() - 2);
                tmps = std::string(tmps, 1, tmps.length() - 2);
                tel(tmpL) = '"' + tmps + tmpstr + '"';
                chan(tmpL, '"' + tmps + tmpstr + '"');
                return '"' + tmps + tmpstr + '"';
            }
        }
        std::vector<antlrcpp::Any>vec = visit(ctx->testlist(0));
        return vec[0];
    }

    antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
        //std::cout << "augassign" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        //std::cout << "flowstmt" << std::endl;
        if (ctx->break_stmt()) return std::string("break");
        if (ctx->continue_stmt()) return std::string("continue");
        if (ctx->return_stmt()) return visit(ctx->return_stmt());
    }

    antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        //std::cout << "breakstmt" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
        //std::cout << "continuestmt" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
        vector<antlrcpp::Any>vec;
        if (ctx->testlist()) {
            vec = visit(ctx->testlist()).as<vector<antlrcpp::Any>>();
        }
        return vec;
    }

    antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        if (ctx->if_stmt()) return visit(ctx->if_stmt());
        if (ctx->while_stmt()) return visit(ctx->while_stmt());
        if (ctx->funcdef()) visit(ctx->funcdef());
        return 0;
    }

    antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        int siz = ctx->test().size();
        bool cnt = 0;
        for (int i = 0;i < siz;++ i) {
            antlrcpp::Any tmp = visit(ctx->test(i));
            if (transbool(tmp)) {
                antlrcpp::Any tmp1 = visit(ctx->suite(i));
                if (tmp1.is<std::string>() && tmp1.as<std::string>() == "break") return tmp1;
                cnt = 1; 
                break;
            }
        }
        if (cnt == 0 && ctx->suite().size() > siz) {
            antlrcpp::Any tmp1 = visit(ctx->suite(siz));
            if (tmp1.is<std::string>() && tmp1.as<std::string>() == "break") return tmp1;
            if (tmp1.is<std::string>() && tmp1.as<std::string>() == "continue") return tmp1;
        }
        return 0;
    }

    antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        //std::cout << "whilestmt" << std::endl;
        antlrcpp::Any tmp = visit(ctx->test());
        bool bl = transbool(tmp);
        while(bl) {
            antlrcpp::Any tmp1 = visit(ctx->suite());
            tmp = visit(ctx->test());
            bl = transbool(tmp);
            if (tmp1.is<std::string>() && tmp1.as<std::string>() == "break") break;
        }
        return 0;
    }

    antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        //std::cout << "suite" << std::endl;
        antlrcpp::Any tmp;
        int s = ctx->stmt().size();
        if (s > 0)
            for (int i = 0;i < s;++ i) {
                tmp = visit(ctx->stmt(i));
                if (tmp.is<std::string>() && tmp.as<std::string>() == "break") return tmp;
                if (tmp.is<std::string>() && tmp.as<std::string>() == "continue") return tmp;
            }
        if (ctx->simple_stmt()) {
            tmp = visit(ctx->simple_stmt());
            if (tmp.is<std::string>() && tmp.as<std::string>() == "break") return tmp;
            if (tmp.is<std::string>() && tmp.as<std::string>() == "continue") return tmp;
            antlrcpp::Any tmp = visit(ctx->simple_stmt()->small_stmt()->flow_stmt()->return_stmt());
            if (tmp.is<vector<antlrcpp::Any>>()) {tmp = tmp.as<vector<antlrcpp::Any>>(); return tmp;}
        }
        return tmp;
        //return visitChildren(ctx);
    }

    antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        return visit(ctx->or_test());
    }

    antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {
        //std::cout << "ortest" << std::endl;
        int siz = ctx->and_test().size();
        bool bl = 0;
        if (siz == 1) return visit(ctx->and_test(0));
        for (int i = 0;i < siz;++ i) {
            antlrcpp::Any tmp = visit(ctx->and_test(i));
            if (transbool(tmp)) {bl = 1; break;}
        }
        return bl;
    }

    antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
        //std::cout << "andtest" << std::endl;
        int siz = ctx->not_test().size();
        bool bl = 1;
        if (siz == 1) return visit(ctx->not_test(0));
        for (int i = 0;i < siz;++ i) {
            antlrcpp::Any tmp = visit(ctx->not_test(i));
            if (!transbool(tmp)) {bl = 0; break;}
        }
        return bl;
    }

    antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
        antlrcpp::Any tmp;
        
        if (ctx->not_test()) {
            tmp = visit(ctx->not_test());
            return !transbool(tmp);
        }
        if (ctx->comparison()) {
            tmp = visit(ctx->comparison());
            return tmp;
        }
    }

    antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
        std::string s = std::string(1000,'0');
        bool bl = 1;
        if (ctx->comp_op().size() > 0) {
            for (int i = 0;i < ctx->comp_op().size();++ i) {
                if (visit(ctx->comp_op(i)).as<int>() == 1) s[ctx->comp_op(i)->LESS_THAN()->getSymbol()->getTokenIndex()] = '1';
                if (visit(ctx->comp_op(i)).as<int>() == 2) s[ctx->comp_op(i)->GREATER_THAN()->getSymbol()->getTokenIndex()] = '2';
                if (visit(ctx->comp_op(i)).as<int>() == 3) s[ctx->comp_op(i)->EQUALS()->getSymbol()->getTokenIndex()] = '3';
                if (visit(ctx->comp_op(i)).as<int>() == 4) s[ctx->comp_op(i)->GT_EQ()->getSymbol()->getTokenIndex()] = '4';
                if (visit(ctx->comp_op(i)).as<int>() == 5) s[ctx->comp_op(i)->LT_EQ()->getSymbol()->getTokenIndex()] = '5';
                if (visit(ctx->comp_op(i)).as<int>() == 6) s[ctx->comp_op(i)->NOT_EQ_2()->getSymbol()->getTokenIndex()] = '6';
            }
            s.erase(std::remove(s.begin(), s.end(), '0'), s.end());
            for (int i = 0;i < s.length();++ i) {
                antlrcpp::Any lhs = visit(ctx->arith_expr(i)), rhs = visit(ctx->arith_expr(i + 1));
                std::string lhsstr, rhsstr;
                bigInteger lhsint, rhsint;
                double lhsdou, rhsdou;
                int lsig = 0, rsig = 0;//0为string，1为string的name，2为int，3为int的name, 4为double, 5为double的name
                if (lhs.is<std::string>()) {
                    lhsstr = lhs.as<std::string>();
                    if (lhsstr[0] == '"') lsig = 0;
                    else {
                        if (tel(lhsstr).is<std::string>()) {
                            lsig = 1;
                            lhsstr = tel(lhsstr).as<std::string>();
                        } 
                        else if (tel(lhsstr).is<bigInteger>()) {
                            lsig = 3;
                            lhsint = tel(lhsstr).as<bigInteger>();
                        }
                        else if (tel(lhsstr).is<double>()) {
                            lsig = 5;
                            lhsdou = tel(lhsstr).as<double>();
                        }
                    }
                } 
                if (lhs.is<bigInteger>()) {
                    lsig = 2;
                    lhsint = lhs.as<bigInteger>();
                }
                if (lhs.is<double>()) {
                    lsig = 4;
                    lhsdou = lhs.as<double>();
                }
                
                if (rhs.is<std::string>()) {
                    rhsstr = rhs.as<std::string>();
                    if (rhsstr[0] == '"') rsig = 0;
                    else {
                        if (tel(rhsstr).is<std::string>()) {
                            rsig = 1;
                            rhsstr = tel(rhsstr).as<std::string>();
                        } 
                        else if (tel(rhsstr).is<bigInteger>()) {
                            rsig = 3;
                            rhsint = tel(rhsstr).as<bigInteger>();
                        }
                        else if (tel(rhsstr).is<double>()) {
                            rsig = 5;
                            rhsdou = tel(rhsstr).as<double>();
                        }
                    }
                } 
                if (rhs.is<bigInteger>()) {
                    rsig = 2;
                    rhsint = rhs.as<bigInteger>();
                }
                if (rhs.is<double>()) {
                    rsig = 4;
                    rhsdou = rhs.as<double>();
                }
                if (lsig == 2 || lsig == 3) {
                    if (rsig == 2 || rsig == 3) {
                    if (s[i] == '1')
                        if (lhsint > rhsint || lhsint == rhsint) {bl = 0; break;}
                    if (s[i] == '2')
                        if (lhsint < rhsint || lhsint == rhsint) {bl = 0; break;}
                    if (s[i] == '3')
                        if (lhsint != rhsint) {bl = 0; break;}
                    if (s[i] == '4')
                        if (lhsint < rhsint) {bl = 0; break;}
                    if (s[i] == '5')
                        if (lhsint > rhsint) {bl = 0; break;}
                    if (s[i] == '6')
                        if (lhsint == rhsint) {bl = 0; break;}
                    }
                    if (rsig == 4 || rsig == 5) {
                    if (s[i] == '1')
                        if ((double)lhsint > rhsdou || (double)lhsint == rhsdou) {bl = 0; break;}
                    if (s[i] == '2')
                        if ((double)lhsint < rhsdou || (double)lhsint == rhsdou) {bl = 0; break;}
                    if (s[i] == '3')
                        if ((double)lhsint != rhsdou) {bl = 0; break;}
                    if (s[i] == '4')
                        if ((double)lhsint < rhsdou) {bl = 0; break;}
                    if (s[i] == '5')
                        if ((double)lhsint > rhsdou) {bl = 0; break;}
                    if (s[i] == '6')
                        if ((double)lhsint == rhsdou) {bl = 0; break;}
                    }
                }
                if (lsig == 0 || lsig == 1) {
                    if (s[i] == '1')
                        if (lhsstr > rhsstr || lhsstr == rhsstr) {bl = 0; break;}
                    if (s[i] == '2')
                        if (lhsstr < rhsstr || lhsstr == rhsstr) {bl = 0; break;}
                    if (s[i] == '3')
                        if (lhsstr != rhsstr) {bl = 0; break;}
                    if (s[i] == '4')
                        if (lhsstr < rhsstr) {bl = 0; break;}
                    if (s[i] == '5')
                        if (lhsstr > rhsstr) {bl = 0; break;}
                    if (s[i] == '6')
                        if (lhsstr == rhsstr) {bl = 0; break;}
                }
                if (lsig == 4 || lsig == 5) {
                    if (rsig == 4 || rsig == 5) {
                    if (s[i] == '1')
                        if (lhsdou > rhsdou || lhsdou == rhsdou) {bl = 0; break;}
                    if (s[i] == '2')
                        if (lhsdou < rhsdou || lhsdou == rhsdou) {bl = 0; break;}
                    if (s[i] == '3')
                        if (lhsdou != rhsdou) {bl = 0; break;}
                    if (s[i] == '4')
                        if (lhsdou < rhsdou) {bl = 0; break;}
                    if (s[i] == '5')
                        if (lhsdou > rhsdou) {bl = 0; break;}
                    if (s[i] == '6')
                        if (lhsdou == rhsdou) {bl = 0; break;}
                    }
                    if (rsig == 2 || rsig == 3) {
                    if (s[i] == '1')
                        if (lhsdou > (double)rhsint || lhsdou == (double)rhsint) {bl = 0; break;}
                    if (s[i] == '2')
                        if (lhsdou < (double)rhsint || lhsdou == (double)rhsint) {bl = 0; break;}
                    if (s[i] == '3')
                        if (lhsdou != (double)rhsint) {bl = 0; break;}
                    if (s[i] == '4')
                        if (lhsdou < (double)rhsint) {bl = 0; break;}
                    if (s[i] == '5')
                        if (lhsdou > (double)rhsint) {bl = 0; break;}
                    if (s[i] == '6')
                        if (lhsdou == (double)rhsint) {bl = 0; break;}
                    }
                }
            }
            return bl;
        } else return visit(ctx->arith_expr(0));
    }

    antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
        if (ctx->LESS_THAN()) return 1;
        if (ctx->GREATER_THAN()) return 2;
        if (ctx->EQUALS()) return 3;
        if (ctx->GT_EQ()) return 4;
        if (ctx->LT_EQ()) return 5;
        if (ctx->NOT_EQ_2()) return 6;
        return 0;
    }

    antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        std::string s = std::string(10000,'0');
        if (ctx->term().size() > 1) {
            for (int i = 0;i < ctx->ADD().size();++ i)
                s[ctx->ADD(i)->getSymbol()->getTokenIndex()] = '+';
            for (int i = 0;i < ctx->MINUS().size();++ i)
                s[ctx->MINUS(i)->getSymbol()->getTokenIndex()] = '-';
            s.erase(std::remove(s.begin(), s.end(), '0'), s.end());
            int con = 0;//when con = 0,all int;con = 1,double;con = 2,string;
            for (int i = 0;i <= s.length();++ i) {
                antlrcpp::Any tmp = visit(ctx->term(i));
                if (tmp.is<double>()) {con = 1; break;}
                if (tmp.is<std::string>()) {
                    std::string tmpstr = tmp.as<std::string>();
                    if (tmpstr[0] == '"') {con = 2; break;}
                    if (tmpstr[0] != '"' && tmpstr != "None") {
                        if (tel(tmpstr).is<double>()) {con = 1; break;}
                        if (tel(tmpstr).is<std::string>()) {con = 2; break;}
                    }
                }
            }
            if (con == 0) {
                antlrcpp::Any tmp = visit(ctx->term(0));
                bigInteger tmpint;
                tmpint = transint(tmp);
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpterm = visit(ctx->term(i + 1));
                    bigInteger tmpin;
                    tmpin = transint(tmpterm);
                    if (s[i] == '+') tmpint = tmpint + tmpin;
                    if (s[i] == '-') tmpint = tmpint - tmpin;
                }
                return tmpint;
            }
            if (con == 1) {
                antlrcpp::Any tmp = visit(ctx->term(0));
                double tmpdou;
                tmpdou = transflo(tmp);
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpterm = visit(ctx->term(i + 1));
                    double tmpdo;
                    tmpdo = transflo(tmpterm);
                    if (s[i] == '+') tmpdou += tmpdo;
                    if (s[i] == '-') tmpdou -= tmpdo;
                }
                return tmpdou;
            }
            if (con == 2) {
                std::string tmp = visit(ctx->term(0));
                if (tmp[0] != '"') tmp = std::string(tel(tmp).as<std::string>(), 1, tel(tmp).as<std::string>().length() - 2);
                else tmp = std::string(tmp, 1, tmp.length() - 2);
                for (int i = 0;i < s.length();++ i) {
                    std::string tmpterm = visit(ctx->term(i + 1));
                    if (tmpterm[0] == '"') tmp = tmp + std::string(tmpterm, 1, tmpterm.length() - 2);
                    else tmp = tmp + std::string(tel(tmpterm).as<std::string>(), 1, tel(tmpterm).as<std::string>().length() - 2);
                }
                return '"' + tmp + '"';
            }
        }
        return visit(ctx->term(0));
    }

    antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        std::string s = std::string(10000,'0');
        if (ctx->factor().size() > 1) {
            for (int i = 0;i < ctx->STAR().size();++ i)
                s[ctx->STAR(i)->getSymbol()->getTokenIndex()] = '*';
            for (int i = 0;i < ctx->DIV().size();++ i)
                s[ctx->DIV(i)->getSymbol()->getTokenIndex()] = '/';
            for (int i = 0;i < ctx->IDIV().size();++ i)
                s[ctx->IDIV(i)->getSymbol()->getTokenIndex()] = '$';
            for (int i = 0;i < ctx->MOD().size();++ i)
                s[ctx->MOD(i)->getSymbol()->getTokenIndex()] = '%';
            s.erase(std::remove(s.begin(), s.end(), '0'), s.end());
            int I, con = 0;//when con = 0,all int;con = 1,double;con = 2,string;
            for (int i = 0;i <= s.length();++ i) {
                antlrcpp::Any tmp = visit(ctx->factor(i));
                if (tmp.is<double>()) {con = 1; break;}
                if (tmp.is<std::string>()) {
                    std::string tmpstr = tmp.as<std::string>();
                    if (tmpstr[0] == '"') {con = 2; I = i; break;}
                    else {
                        if (tel(tmpstr).is<double>()) {con = 1; break;}
                        if (tel(tmpstr).is<std::string>() && tel(tmpstr).as<std::string>() != "None") {con = 2; I = i; break;}
                    }
                }
                if (i < s.length() && s[i] == '/') {con = 1;break;}
            }
            if (con == 0) {
                antlrcpp::Any tmp = visit(ctx->factor(0));
                bigInteger tmpint;
                tmpint = transint(tmp);
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                    bigInteger tmpin;
                    tmpin = transint(tmpfac);
                    if (s[i] == '*') tmpint = tmpint * tmpin;
                    if (s[i] == '$') tmpint = tmpint / tmpin;
                    if (s[i] == '%') tmpint = tmpint % tmpin;
                }
                return tmpint;
            }
            if (con == 1) {
                antlrcpp::Any tmp = visit(ctx->factor(0));
                double tmpdou;
                tmpdou = transflo(tmp);
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                    double tmpdo;
                    tmpdo = transflo(tmpfac);
                    if (s[i] == '*') tmpdou *= tmpdo;
                    if (s[i] == '/') tmpdou /= tmpdo;
                    if (s[i] == '$') tmpdou = (long long int)tmpdou / (long long int)tmpdo;
                    if (s[i] == '%') tmpdou = (long long int)tmpdou % (long long int)tmpdo;
                }
                return tmpdou;
            }
            if (con == 2) {
                std::string tmpstr = visit(ctx->factor(I)).as<std::string>();
                if(tmpstr[0] == '"') tmpstr = std::string(tmpstr, 1, tmpstr.length()-2);
                else tmpstr = std::string(tel(tmpstr).as<std::string>(), 1, tel(tmpstr).as<std::string>().length()-2);
                int tmpint;
                if (I) {
                    tmpint = transint(visit(ctx->factor(0)));
                    for (int i = 0;i < s.length();++ i) {
                        if (i == I-1) continue;
                        int tmpin;
                        antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                        tmpin = transint(tmpfac);
                        if (s[i] == '*') tmpint = tmpint * tmpin;
                        if (s[i] == '$') tmpint = tmpint / tmpin;
                        if (s[i] == '%') tmpint = tmpint % tmpin;
                    }
                }
                else {
                    tmpint = transint(visit(ctx->factor(1)));
                    for (int i = 1;i < s.length();++ i) {
                        if (i == I-1) continue;
                        int tmpin;
                        antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                        tmpin = transint(tmpfac);
                        if (s[i] == '*') tmpint = tmpint * tmpin;
                        if (s[i] == '$') tmpint = tmpint / tmpin;
                        if (s[i] == '%') tmpint = tmpint % tmpin;
                    }
                }
                std::string tmps;
                for (int i = 1;i <= tmpint;++ i)
                    tmps += tmpstr;
                return '"' + tmps + '"';
            }
            
        } else return visit(ctx->factor(0));
    }

    antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        if (ctx->atom_expr()) {
            antlrcpp::Any tmp = visit(ctx->atom_expr());
            return tmp;
        }
        if (ctx->factor()) {
            antlrcpp::Any tmp = visit(ctx->factor());
            if (tmp.is<bigInteger>()) {
                if (ctx->MINUS()) return (bigInteger)0 - tmp.as<bigInteger>();
            }
            if (tmp.is<double>()) {
                if (ctx->MINUS()) return 0 - tmp.as<double>();
            }
            if (tmp.is<bool>()) {
                if (ctx->MINUS()) {
                    if (tmp.as<bool>()) return (bigInteger)(-1);
                    else return (bigInteger)0;
                }
            }
            if (tmp.is<std::string>()) {
                std::string str = tmp.as<std::string>();
                tmp = tel(str);
                if (tmp.is<bigInteger>()) {
                    if (ctx->MINUS()) return (bigInteger)0 - tmp.as<bigInteger>();
                }
                if (tmp.is<double>()) {
                    if (ctx->MINUS()) return 0 - tmp.as<double>();
                }
                if (tmp.is<bool>()) {
                    if (ctx->MINUS()) {
                        if (tmp.as<bool>()) return (bigInteger)(-1);
                         else return (bigInteger)0;
                    }
                }
            }
            return tmp;
        }
    }

    antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        antlrcpp::Any tmp1;
        std::vector<antlrcpp::Any>tmp2;
        std::string str1;
        if (ctx->trailer()) {
            tmp1 = visit(ctx->atom());
            tmp2 = visit(ctx->trailer()).as<vector<antlrcpp::Any>>();
            if (tmp1.is<std::string>())
                str1 = tmp1.as<std::string>();
            if (str1 == "print") {
                if (!tmp2.size()) std::cout << std::endl;
                else for (int i = 0;i < tmp2.size();++ i) {
                    anyPrint(tmp2[i]);
                    if (i < tmp2.size()-1) std::cout << " ";
                    else std::cout << std::endl;
                }
                return 0;
            }
            if (str1 == "int")  {
                bigInteger a = transint(tmp2[0]);
                return a;
            }
            if (str1 == "float") {
                double a = transflo(tmp2[0]);
                return a;
            }
            if (str1 == "str") {
                std::string a = transstr(tmp2[0]);
                return a;
            }
            if (str1 == "bool") return transbool(tmp2[0]);


            std::map<std::string,antlrcpp::Any>ma;
            var.push_back(ma);
            vector<antlrcpp::Any>lis = visitParameters(par[str1]);
            vector<antlrcpp::Any>tra = visit(ctx->trailer());
            for (int i = 0;i < tra.size();++ i) {
                if (tra[i].is<std::string>()&&tra[i].as<std::string>()!="None") continue;
                chan(lis[i].as<std::string>(), tra[i]);
            }
            antlrcpp::Any tmp = visit(sui[str1]);
            vector<antlrcpp::Any>tp;
            if (tmp.is<vector<antlrcpp::Any>>())
                tp = tmp.as<vector<antlrcpp::Any>>();
            var.pop_back();
            return tp;
        } else {
            tmp1 = visit(ctx->atom());
            return tmp1;
        }
    }

    antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        std::vector<antlrcpp::Any>tra;
        if (ctx->arglist()) tra = visit(ctx->arglist()).as<std::vector<antlrcpp::Any>>();
        return tra;
    }

    antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        std::string tmp;
        if (ctx->NAME()) {
            //std::cout << "atomname" << std::endl;
            tmp = ctx->NAME()->toString();
            return tmp;
        }
        if (ctx->STRING(0)) {
            std::string tmps;
            for (int i = 0;i < ctx->STRING().size();++ i) {
                tmp = ctx->STRING(i)->toString();
                tmps += std::string(tmp, 1, tmp.length()-2);
            }
            tmps = '"' + tmps + '"';
            return tmps;
        }
        if (ctx->NUMBER()) {
            tmp = ctx->NUMBER()->toString();
            bool dou = 0;
            int I;
            for (int i = 0;i < tmp.length();++ i)
                if (tmp[i] == '.') {dou = 1; I = i; break;}
            if (!dou) {
                std::reverse(tmp.begin(),tmp.end());
                bigInteger tmpint(tmp, tmp.length(), 1);
                return tmpint;
            } else {
                double tmpdou = 0;
                double k = 1;
                for (int i = I - 1;i >= 0;-- i) {
                    tmpdou += (double)(tmp[i] - '0') * k;
                    k *= 10;
                }
                k = 0.1;
                for (int i = I + 1;i < tmp.length();++ i) {
                    tmpdou += (double)(tmp[i] - '0') * k;
                    k /= 10;
                }
                return tmpdou;
            }
        }
        if (ctx->TRUE()) {
            bool blt = true;
            return blt;
        }
        if (ctx->FALSE()) {
            bool blf = false;
            return blf;
        }
        if (ctx->NONE()) {
            tmp = ctx->NONE()->toString();
            return tmp;
        }
        if (ctx->test()) {
            antlrcpp::Any tmpany = visit(ctx->test());
            if (tmpany.is<bigInteger>()) {
                return tmpany.as<bigInteger>();
            }
            if (tmpany.is<std::string>()) {
                return tmpany.as<std::string>();
            }
            if (tmpany.is<double>()) {
                return tmpany.as<double>();
            }
            if (tmpany.is<bool>()) {
                return tmpany.as<bool>();
            }
        }
    }

    antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
        std::vector<antlrcpp::Any>tlist;
        for (int i = 0;i < ctx->test().size();++ i) {
            antlrcpp::Any tmp = visit(ctx->test(i));
            tlist.push_back(tmp);
        }
        return tlist;
    }

    antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        std::vector<antlrcpp::Any>arg;
        for (int i = 0;i < ctx->argument().size();++ i) {
            antlrcpp::Any tmp = visit(ctx->argument(i));
            arg.push_back(tmp);
        }
        return arg;
    }

    antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        //std::cout << "argument" << std::endl;
        if (ctx->NAME()) {
            std::string tmp = ctx->NAME()->toString();
            antlrcpp::Any tmp0 = visit(ctx->test());
            if (tmp0.is<std::string>()) {
                std::string tmpstr = tmp0.as<std::string>();
                if (tmpstr[0] != '"') {
                    if (tmpstr == "None") chan(tmp, tmpstr);
                    else chan(tmp, tel(tmpstr));
                }
            }
            return tmp;
        }
        else {
            antlrcpp::Any tmp = visit(ctx->test());
            if (tmp.is<std::string>() && tmp.as<std::string>()[0] != '"' && tmp.as<std::string>() != "None") {
                tmp = tel(tmp.as<std::string>());
            }
            return tmp;
        }
    }
};

#endif //PYTHON_INTERPRETER_EVALVISITOR_H
