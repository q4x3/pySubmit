#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3BaseVisitor.h"
#include "bigInteger.h"
#include <map>
#include <algorithm>
#include <iomanip>
#include <vector>

std::map<std::string,antlrcpp::Any>tel;

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
            antlrcpp::Any tmp2 = tel[str1];
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
            if (tel[str2].is<std::string>()) {
                if (tel[str2].as<std::string>()[0] == '"') {
                    std::cout << std::string(tel[str2].as<std::string>(),1,tel[str2].as<std::string>().length()-2);
                    return;
                }
                if (tel[str2].as<std::string>() == "None") {
                    std::cout << "None";
                    return;
                }
            }
            if (tel[str2].is<bigInteger>()) {
                std::cout << tel[str2].as<bigInteger>();
                return;
            }
            if (tel[str2].is<bool>())
                if (tel[str2].as<bool>()) {std::cout << "True"; return;}
                else {std::cout << "False"; return;}
            if (tel[str2].is<double>()) {
                std::cout << fixed << setprecision(6) << tel[str2].as<double>();
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

class EvalVisitor: public Python3BaseVisitor {
    antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {
        //std::cout << "fileinput" << std::endl;
        for (int i = 0;i < ctx->stmt().size();++ i)
            visit(ctx->stmt(i));
        return 0;
    }

    antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
        //std::cout << "funcdef" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        //std::cout << "parameters" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        //std::cout << "typedargslist" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
        //std::cout << "tfpdef" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        //std::cout << "stmt" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        //std::cout << "simplestmt" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        //std::cout << "smallstmt" << std::endl;
        return visitChildren(ctx);
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
                        if (tmp[0] != '"') atm0[i] = tel[tmp];
                    }
                }
                
                for (int i = 0;i < s - 1;++ i) {
                    std::vector<antlrcpp::Any> atm = visit(ctx->testlist(i));
                    for (int j = 0;j < atm.size();++ j) {
                        std::string atms;
                        if (atm[j].is<std::string>()) atms = atm[j].as<std::string>();
                        tel[atms] = atm0[j];
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
                    if (tel[tmpstr].is<bigInteger>()) {tmpint = tel[tmpstr].as<bigInteger>(); sig = 0;}
                    if (tel[tmpstr].is<double>()) {tmpdou = tel[tmpstr].as<double>(); sig = 1;}
                    if (tel[tmpstr].is<std::string>()) {tmpstr = tel[tmpstr].as<std::string>(); sig = 2;}
                }
            }
            if (sig == 0) {
                if (tel[tmpL].is<bigInteger>()) {
                    bigInteger tmpin = tel[tmpL].as<bigInteger>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpint = tmpint + tmpin;
                        tel[tmpL] = tmpint;
                        return tmpint;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpint = tmpin - tmpint;
                        tel[tmpL] = tmpint;
                        return tmpint;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpint = tmpint * tmpin;
                        tel[tmpL] = tmpint;
                        return tmpint;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = (double)tmpin / (double)tmpint;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpint = tmpin / tmpint;
                        tel[tmpL] = tmpint;
                        return tmpint;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpint = tmpin % tmpint;
                        tel[tmpL] = tmpint;
                        return tmpint;
                    }
                }
                if (tel[tmpL].is<double>()) {
                    double tmpdo = tel[tmpL].as<double>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpdou = (double)tmpint + tmpdo;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpdou = tmpdo - (double)tmpint;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpdou = (double)tmpint * tmpdo;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = tmpdo / (double)tmpint;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpint = (long long int)tmpdo / (int)tmpint;
                        tel[tmpL] = tmpint;
                        return tmpint;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpint = (long long int)tmpdo % (long long int)tmpint;
                        tel[tmpL] = tmpint;
                        return tmpint;
                    }
                }
                if (tel[tmpL].is<std::string>()) {
                    tmpstr = tel[tmpL].as<std::string>();
                    tmpstr = std::string(tmpstr, 1, tmpstr.length()-2);
                    std::string str;
                    for (int i = 1;i <= (int)tmpint;++ i) {
                        str += tmpstr;
                    }
                    tel[tmpL] = '"' + str + '"';
                    return '"' + str + '"';
                }
            }
            if (sig == 1) {
                if (tel[tmpL].is<bigInteger>()) {
                    bigInteger tmpin = tel[tmpL].as<bigInteger>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpdou = tmpdou + (double)tmpin;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpdou = (double)tmpin - tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpdou = (double)tmpin * tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = (double)tmpin / tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpdou = (int)tmpin / (int)tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpdou = (long long int)tmpin % (long long int)tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                }
                if (tel[tmpL].is<double>()) {
                    double tmpdo = tel[tmpL].as<double>();
                    if (ctx->augassign()->ADD_ASSIGN()) {
                        tmpdou = tmpdou + tmpdo;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->SUB_ASSIGN()) {
                        tmpdou = tmpdo - tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->MULT_ASSIGN()) {
                        tmpdou = tmpdo * tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->DIV_ASSIGN()) {
                        tmpdou = tmpdo / tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->IDIV_ASSIGN()) {
                        tmpdou = (long long int)tmpdo / (long long int)tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                    if (ctx->augassign()->MOD_ASSIGN()) {
                        tmpdou = (long long int)tmpdo % (long long int)tmpdou;
                        tel[tmpL] = tmpdou;
                        return tmpdou;
                    }
                }
            }
            if (sig == 2) {
                std::string tmps = tel[tmpL].as<std::string>();
                tmpstr = std::string(tmpstr, 1, tmpstr.length() - 2);
                tmps = std::string(tmps, 1, tmps.length() - 2);
                tel[tmpL] = '"' + tmps + tmpstr + '"';
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
        return visitChildren(ctx);
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
        //std::cout << "returnstmt" << std::endl;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        if (ctx->if_stmt()) return visit(ctx->if_stmt());
        //if (ctx->while_stmt()) return visit(ctx->while());
        return visitChildren(ctx);
    }

    antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        int siz = ctx->test().size();
        bool cnt = 0;
        for (int i = 0;i < siz;++ i) {
            antlrcpp::Any tmp = visit(ctx->test(i));
            if (transbool(tmp)) {visit(ctx->suite(i)); cnt = 1; break;}
        }
        if (cnt == 0 && ctx->suite().size() > siz) {
            visit(ctx->suite(siz));
        }
        return 0;
    }

    antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        //std::cout << "whilestmt" << std::endl;
        antlrcpp::Any tmp = visit(ctx->test());
        bool bl = transbool(tmp);
        while(bl) {
            visit(ctx->suite());
            tmp = visit(ctx->test());
            bl = transbool(tmp);
        }
        return 0;
    }

    antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        //std::cout << "suite" << std::endl;
        int s = ctx->stmt().size();
        if (s > 0)
            for (int i = 0;i < s;++ i) visit(ctx->stmt(i));
        if (ctx->simple_stmt()) visit(ctx->simple_stmt());
        return 0;
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
                        if (tel[lhsstr].is<std::string>()) {
                            lsig = 1;
                            lhsstr = tel[lhsstr].as<std::string>();
                        } 
                        if (tel[lhsstr].is<bigInteger>()) {
                            lsig = 3;
                            lhsint = tel[lhsstr].as<bigInteger>();
                        }
                        if (tel[lhsstr].is<double>()) {
                            lsig = 5;
                            lhsdou = tel[lhsstr].as<double>();
                        }
                    }
                } 
                if (lhs.is<bigInteger>()) {
                    lsig = 2;
                    lhsint = lhs.as<bigInteger>();
                }
                if (rhs.is<std::string>()) {
                    rhsstr = rhs.as<std::string>();
                    if (rhsstr[0] == '"') rsig = 0;
                    else {
                        if (tel[rhsstr].is<std::string>()) {
                            rsig = 1;
                            rhsstr = tel[rhsstr].as<std::string>();
                        } 
                        if (tel[rhsstr].is<bigInteger>()) {
                            rsig = 3;
                            rhsint = tel[rhsstr].as<bigInteger>();
                        }
                        if (tel[rhsstr].is<double>()) {
                            rsig = 5;
                            rhsdou = tel[rhsstr].as<double>();
                        }
                    }
                } 
                if (rhs.is<bigInteger>()) {
                    rsig = 2;
                    rhsint = rhs.as<bigInteger>();
                }
                if (lsig == 2 || lsig == 3) {
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
        std::string s = std::string(1000,'0');
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
                    else {
                        if (tel[tmpstr].is<double>()) {con = 1; break;}
                        if (tel[tmpstr].is<std::string>()) {con = 2; break;}
                    }
                }
            }
            if (con == 0) {
                antlrcpp::Any tmp = visit(ctx->term(0));
                bigInteger tmpint;
                if (tmp.is<bigInteger>()) tmpint = tmp.as<bigInteger>();
                else tmpint = tel[tmp.as<std::string>()].as<bigInteger>();
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpterm = visit(ctx->term(i + 1));
                    bigInteger tmpin;
                    if (tmpterm.is<bigInteger>()) tmpin = tmpterm.as<bigInteger>();
                    else tmpin = tel[tmpterm.as<std::string>()].as<bigInteger>();
                    if (s[i] == '+') tmpint = tmpint + tmpin;
                    if (s[i] == '-') tmpint = tmpint - tmpin;
                }
                return tmpint;
            }
            if (con == 1) {
                antlrcpp::Any tmp = visit(ctx->term(0));
                double tmpdou;
                if (tmp.is<double>()) tmpdou = tmp.as<double>();
                if (tmp.is<bigInteger>()) tmpdou = tmp.as<bigInteger>();
                if (tmp.is<std::string>()) tmpdou = tel[tmp.as<std::string>()].as<double>();
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpterm = visit(ctx->term(i + 1));
                    double tmpdo;
                    if (tmpterm.is<double>()) tmpdo = tmpterm.as<double>();
                    if (tmpterm.is<bigInteger>()) tmpdo = tmpterm.as<bigInteger>();
                    if (tmpterm.is<std::string>()) {
                        if (tel[tmpterm.as<std::string>()].is<double>())
                            tmpdo = tel[tmpterm.as<std::string>()].as<double>();
                        else tmpdo = tel[tmpterm.as<std::string>()].as<bigInteger>();
                    }
                    if (s[i] == '+') tmpdou += tmpdo;
                    if (s[i] == '-') tmpdou += tmpdo;
                }
                return tmpdou;
            }
            if (con == 2) {
                std::string tmp = visit(ctx->term(0));
                if (tmp[0] != '"') tmp = std::string(tel[tmp].as<std::string>(), 1, tel[tmp].as<std::string>().length() - 2);
                else tmp = std::string(tmp, 1, tmp.length() - 2);
                for (int i = 0;i < s.length();++ i) {
                    std::string tmpterm = visit(ctx->term(i + 1));
                    if (tmpterm[0] == '"') tmp = tmp + std::string(tmpterm, 1, tmpterm.length() - 2);
                    else tmp = tmp + std::string(tel[tmpterm].as<std::string>(), 1, tel[tmpterm].as<std::string>().length() - 2);
                }
                return '"' + tmp + '"';
            }
        }
        return visit(ctx->term(0));
    }

    antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        std::string s = std::string(1000,'0');
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
                        if (tel[tmpstr].is<double>()) {con = 1; break;}
                        if (tel[tmpstr].is<std::string>()) {con = 2; I = i; break;}
                    }
                }
                if (i < s.length() && s[i] == '/') {con = 1;break;}
            }
            if (con == 0) {
                antlrcpp::Any tmp = visit(ctx->factor(0));
                bigInteger tmpint;
                if (tmp.is<bigInteger>()) tmpint = tmp.as<bigInteger>();
                else tmpint = tel[tmp.as<std::string>()].as<bigInteger>();
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                    bigInteger tmpin;
                    if (tmpfac.is<bigInteger>()) tmpin = tmpfac.as<bigInteger>();
                    else tmpin = tel[tmpfac.as<std::string>()].as<bigInteger>();
                    if (s[i] == '*') tmpint = tmpint * tmpin;
                    if (s[i] == '$') tmpint = tmpint / tmpin;
                    if (s[i] == '%') tmpint = tmpint % tmpin;
                }
                return tmpint;
            }
            if (con == 1) {
                antlrcpp::Any tmp = visit(ctx->factor(0));
                double tmpdou;
                if (tmp.is<double>()) tmpdou = tmp.as<double>();
                if (tmp.is<bigInteger>()) tmpdou = tmp.as<bigInteger>();
                if (tmp.is<std::string>()) tmpdou = tel[tmp.as<std::string>()].as<double>();
                for (int i = 0;i < s.length();++ i) {
                    antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                    double tmpdo;
                    if (tmpfac.is<double>()) tmpdo = tmpfac.as<double>();
                    if (tmpfac.is<bigInteger>()) tmpdo = tmpfac.as<bigInteger>();
                    if (tmpfac.is<std::string>()) {
                        if (tel[tmpfac.as<std::string>()].is<double>())
                            tmpdo = tel[tmpfac.as<std::string>()].as<double>();
                        else tmpdo = tel[tmpfac.as<std::string>()].as<bigInteger>();
                    }
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
                else tmpstr = std::string(tel[tmpstr].as<std::string>(), 1, tel[tmpstr].as<std::string>().length()-2);
                int tmpint;
                if (I) {
                    if (visit(ctx->factor(0)).is<bigInteger>()) tmpint = visit(ctx->factor(0)).as<bigInteger>();
                    else tmpint = tel[visit(ctx->factor(0)).as<std::string>()].as<bigInteger>();
                    for (int i = 0;i < s.length();++ i) {
                        if (i == I-1) continue;
                        int tmpin;
                        antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                        if (tmpfac.is<bigInteger>()) tmpin = tmpfac.as<bigInteger>();
                        else tmpin = tel[tmpfac.as<std::string>()].as<bigInteger>();
                        if (s[i] == '*') tmpint = tmpint * tmpin;
                        if (s[i] == '$') tmpint = tmpint / tmpin;
                        if (s[i] == '%') tmpint = tmpint % tmpin;
                    }
                }
                else {
                    if (visit(ctx->factor(1)).is<bigInteger>()) tmpint = visit(ctx->factor(1)).as<bigInteger>();
                    else tmpint = tel[visit(ctx->factor(1)).as<std::string>()].as<bigInteger>();
                    for (int i = 1;i < s.length();++ i) {
                        if (i == I-1) continue;
                        int tmpin;
                        antlrcpp::Any tmpfac = visit(ctx->factor(i + 1));
                        if (tmpfac.is<bigInteger>()) tmpin = tmpfac.as<bigInteger>();
                        else tmpin = tel[tmpfac.as<std::string>()].as<bigInteger>();
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
            if (tmp.is<bigInteger>()) {
                if (ctx->MINUS()) return (bigInteger)0 - tmp.as<bigInteger>();
            }
            if (tmp.is<double>()) {
                if (ctx->MINUS()) return 0 - tmp.as<double>();
            }
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
            }
            return 0;
        } else {
            tmp1 = visit(ctx->atom());
            if (tmp1.is<std::string>()) {
                std::string str;
                str = tmp1.as<std::string>();
                return str;
            }
            if (tmp1.is<bigInteger>()) {
                bigInteger tmpin = tmp1.as<bigInteger>();
                return tmpin;
            }
            if (tmp1.is<double>())
                return tmp1.as<double>();
            if (tmp1.is<bool>()) {
                bool bl = tmp1.as<bool>();
                return bl;
            }
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
                    if (tmpstr == "None") tel[tmp] = tmpstr;
                    else tel[tmp] = tel[tmpstr];
                }
            }
            return tmp;
        }
        else return visit(ctx->test());
    }
};

#endif //PYTHON_INTERPRETER_EVALVISITOR_H
