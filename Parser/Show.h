#pragma once
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include "ui_Show.h"
#include "myQGraphicsView.h"
#include<qlayout.h>


//lr1����ļ���ַ
#define Table_FILE  "results/ACTION_GOTO_Table.csv" //�������ַ
#define REDUCTION_PROCESS_FILE "results/Reduction_Process.txt" //�����Լ��ַ
#define TREE_DOT_FILE  "results/Parse_Tree.dot"   //��ͼdot�ļ���ַ
#define TREE_PNG_FILE   "picture/Parse_Tree.png" //�﷨��ͼƬ��ַ
#define PARSE_ERROR_FILE "error_log/Parse_Error.txt"
#define DFA_DOT_FILE   "results/Parse_DFA.dot"
#define PARSE_DFA_PNG_FILE "pcture/Parse_DFA.png"


//�ʷ���������ļ���ַ
#define TOKEN_LIST_FILE "results/Token_List.txt" 
#define ERROR_LIST_FILE "error_log/Lexical_Error_List.txt"
#define DFA_PNG_FILE    "picture/DFA.png"


//�ķ���������ļ���ַ
#define GRAMMAR_ERROR_FILE "error_log/Grammar_Error_List.txt"
#define EXTENDED_GRAMMAR_FILE "results/Extended_Grammar.txt"
#define FIRST_SET_FILE "results/First_Set.txt"


//�����޸�ҳ��
class Show : public QMainWindow
{
    Q_OBJECT

public:
    Show(QWidget* parent = nullptr);
    ~Show();
    void showToken();//չʾ��Ԫ
    void showDFA();//չʾdfa
    void showTable();//չʾAction goto��
    void showTree(); //չʾ�﷨������
    void showProcess(); //չʾ��Լ����
    void showParseDFA();

private:
    Ui::ShowWindow* ui;                       // ��ҳ��
    QVBoxLayout * layout;
};


