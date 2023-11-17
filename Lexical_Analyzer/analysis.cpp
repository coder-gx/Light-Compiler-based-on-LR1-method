#include "analysis.h"
#define CODE_FILE_OPEN_ERROR 2
unit::unit(string tp, string v)
{
    this->type = tp;
    this->value = v;
}
//����ע�͵���� û������ �� ����ע�͵������ ��Ӧ��
void analysis::getStrBuffer() {
    //�Լ��������� �Ͳ��ϵ���������������
    //����/n ���� ��������  ����deleNotes 
    //��Ҫ������������� ����Ϊ��������ˣ�������ĸ��û�ж���
    //��ʱ�����Ҫ����һ������أ����� ��� �ѻ���ظ��ָ����ʵ���������ŵĶ���
    //���ˣ�������Ϻ󣬻���ؽ�����Ӧ�����Ӧ���·�
    //Ȼ��dele������Ļ���� �͵�״̬�����з��� 
    char c = '\0';
    int buffer_flag = 0;//�������Ƿ���Ҫ��ת

    while (1)
    {

        c = fgetc(fin);
        if (c == EOF)
        {
            //������
            deleNotes();
            deleSpaces();
            if (buffer_read[buffer_choose].count > 0)
            {
                strcpy(buffer_end.buffer, buffer_read[buffer_choose].buffer);
                buffer_end.count = buffer_read[buffer_choose].count;
                //����״̬������ 
                //ע�����Ļ����� �п����ǲ��������ִ� ��������̫���� 
                //eg: "111*n"����300���ˣ��ͻ�ָ��
                buffer_read[buffer_choose].count = 0;
                fprintf(fout_pre, "%s\n", buffer_read[buffer_choose].buffer);
                spearateStates();
            }
            break;
        }

        //���������
        if (buffer_read[buffer_choose].count == BUFFER_SIZE - 2)
        {
            buffer_read[buffer_choose].buffer[buffer_read[buffer_choose].count] = c;
            int i;
            for (i = 0; i < buffer_read[buffer_choose].count; i++)
            {
                if (isDelimiter(buffer_read[buffer_choose].buffer[i]))
                {
                    int j;//�ֽ��
                    int k;
                    //��buffer_choose��ת�Ƶ�1-buffer_choose�У�
                    for (j = 0, k = i + 1; k <= buffer_read[buffer_choose].count; k++, j++)
                    {
                        buffer_read[1 - buffer_choose].buffer[j] = buffer_read[buffer_choose].buffer[k];
                    }
                    //count��С��������
                    buffer_read[1 - buffer_choose].count = j;
                    buffer_read[buffer_choose].count = i;

                    //�����ս��
                    buffer_read[1 - buffer_choose].buffer[j] = '\0';
                    buffer_read[buffer_choose].buffer[i + 1] = '\0';

                    //��������ת
                    buffer_flag = 1;

                    break;
                }
            }

        }
        else if (c == '\n' && !note_flag)
        {
            cout << 1 << endl;
            buffer_read[buffer_choose].buffer[buffer_read[buffer_choose].count] = '\0';
        }
        else if (c == '\n')
        {
            cout << 2 << endl;
            buffer_read[buffer_choose].buffer[buffer_read[buffer_choose].count] = '\0';
        }
        else {
            buffer_read[buffer_choose].buffer[buffer_read[buffer_choose].count++] = c;
            continue;//������
        }
        //�����������к�/���������Ĵ���
        deleNotes();
        deleSpaces();

        if (buffer_read[buffer_choose].count > 0)
        {
            strcpy(buffer_end.buffer, buffer_read[buffer_choose].buffer);
            buffer_end.count = buffer_read[buffer_choose].count;
            //����״̬������ 
            //ע�����Ļ����� �п����ǲ��������ִ� ��������̫���� 
            //eg: "111*n"����300���ˣ��ͻ�ָ��
            buffer_read[buffer_choose].count = 0;
            fprintf(fout_pre, "%s\n", buffer_read[buffer_choose].buffer);
            spearateStates();
        }

        if (buffer_flag == 1)
        {
            //��һ�� ��������ת
            buffer_read[buffer_choose].count = 0;
            buffer_choose = 1 - buffer_choose;
            buffer_flag = 0;
        }

    }
    //cout << "The result of lexical analysis has been saved in the res_out.txt file." << endl;
    //cout << "The pre-processed code has been saved in the pre-processed_code.txt file." << endl;
    //cout << "The word_lable has been saved in the word-lable.txt file." << endl;
    //cout << "The analysis_res has been saved in the analysis_res.txt file." << endl;
}
//ѭ���õ�һ���µ�strbuffer  ������deleNotes�� �͵�״̬��������
void analysis::deleNotes() {
    //ɾ��ע��
    char note[BUFFER_SIZE];
    int note_count = 0;
    bool flag_qoute = 0;
    //״̬�� �����ǡ���������/����ѭ��
    for (int i = 0; buffer_read[buffer_choose].buffer[i] != '\0'; i++)
    {
        if (buffer_read[buffer_choose].buffer[i] == '"')
        {
            flag_qoute = 1 - flag_qoute;
            if (note_flag != 1)
                continue;
        }
        if (flag_qoute == 1)
            if (note_flag != 1)
                continue;
        if (buffer_read[buffer_choose].buffer[i] == '/' || note_flag == 1)
        {
            if (buffer_read[buffer_choose].buffer[i + 1] == '\0')
            {
                continue;
            }
            if (buffer_read[buffer_choose].buffer[i + 1] == '/' && !note_flag)
            {
                //���� //״̬ ֱ��\0ֹͣ
                int j;

                for (j = i; buffer_read[buffer_choose].buffer[j] != '\0'; j++)
                {
                    note[note_count++] = buffer_read[buffer_choose].buffer[j];
                    buffer_read[buffer_choose].buffer[j] = '\0';
                }
                note[note_count] = '\0';
                fprintf(fout, "[ע��]----[ %s ]\n", note);
                buffer_read[buffer_choose].count -= note_count;
                note_count = 0;

                break;

            }
            if (buffer_read[buffer_choose].buffer[i + 1] == '*' || note_flag == 1)
            {
                //����/* ״̬ 
                note_flag = 1;
                int j;
                for (j = i + 2 * (1 - note_flag); buffer_read[buffer_choose].buffer[j] != '\0'; j++)
                {

                    note[note_count++] = buffer_read[buffer_choose].buffer[j];
                    if (!flag_qoute && buffer_read[buffer_choose].buffer[j] == '*' && buffer_read[buffer_choose].buffer[j + 1] == '/')
                    {
                        note_flag = 0;
                        note[note_count++] = '/';
                        note[note_count] = '\0';
                        fprintf(fout, "[ע��]----[ %s ]\n", note);

                        buffer_read[buffer_choose].count -= note_count;
                        note_count = 0;
                        break;
                    }
                }

                if (note_flag == 0)
                    j = j + 2;

                //��ʼǰ��

                for (; buffer_read[buffer_choose].buffer[j] != '\0'; j++, i++)
                {
                    if (buffer_read[buffer_choose].buffer[j] == '\n')
                    {
                        i--;
                        continue;
                    }
                    buffer_read[buffer_choose].buffer[i] = buffer_read[buffer_choose].buffer[j];
                }

                if (note_flag) {
                    //��ζ�Ŷ���ע�ͣ�ֱ��printf
                    note[note_count] = '\0';
                    fprintf(fout, "[ע��]----[ %s ]\n", note);

                    buffer_read[buffer_choose].buffer[i] = '\0';
                    buffer_read[buffer_choose].count -= note_count;
                    break;




                }
                buffer_read[buffer_choose].buffer[i] = '\0';
            }

        }
    }
}
void analysis::deleSpaces() {
    //��� �Ŀո����ɾȥ
    //����Ҫ�ж�����ǲ��ǽ���ķ�Χ�� ��Ϊ ";"�϶������ǽ��

    bool flag1 = true, flag2 = true;
    for (int i = 0; buffer_read[buffer_choose].buffer[i] != '\0'; i++)
    {
        //����ɾ���ַ����ڵĿո�
        if (buffer_read[buffer_choose].buffer[i] == '"')
            flag1 = !flag1;
        //����ɾ���ո��ַ�
        if (buffer_read[buffer_choose].buffer[i] == '\'')
            flag2 = !flag2;
        if ((buffer_read[buffer_choose].buffer[i] == ' ' || buffer_read[buffer_choose].buffer[i] == '\t') && flag1 && flag2)
        {
            //�ҵ��ո�����ĩβ���ǵ�һ�����ǿո��λ��
            int j = i + 1;
            for (; buffer_read[buffer_choose].buffer[j] != '\0' && (buffer_read[buffer_choose].buffer[j] == ' ' || buffer_read[buffer_choose].buffer[j] == '\t'); j++)
            {
            }
            //����ǵ�ĩβ�ˣ�ֱ���޸�β��λ�ü���
            if (buffer_read[buffer_choose].buffer[j] == '\0')
            {
                buffer_read[buffer_choose].buffer[i] = '\0';
                buffer_read[buffer_choose].count -= (j - i);
                break;
            }
            //����ǿ�ͷ,ֱ��ȫɾ����
            if (i == 0)
            {
                int k = i;
                buffer_read[buffer_choose].count -= (j - i);
                for (; buffer_read[buffer_choose].buffer[j] != '\0'; j++, k++)
                    buffer_read[buffer_choose].buffer[k] = buffer_read[buffer_choose].buffer[j];
                buffer_read[buffer_choose].buffer[k] = '\0';
                // i--����Ϊԭ��i��λ���ǿո����ڱ�ɾ�����ˣ����Ի���һ����λ
                i--;
            }
            else
            {
                //���֮���ж���ո���ɾ��ֻ��һ��
                if (j - i >= 2)
                {
                    int k = i + 1;
                    buffer_read[buffer_choose].count -= (j - (i + 1));
                    for (; buffer_read[buffer_choose].buffer[j] != '\0'; j++, k++)
                        buffer_read[buffer_choose].buffer[k] = buffer_read[buffer_choose].buffer[j];
                    buffer_read[buffer_choose].buffer[k] = '\0';
                    //�Ƚ�j�ƶ���i+1��λ��
                    j = i + 1;
                    //���ﲻ��Ҫi--����
                }

                //�жϿո�ɲ�����ɾ����ֻҪ����������һ���� ��Χ����û�пո�������������� �ļ���
                //�������� a > = b ���ִ���д�����޷���𣬻Ὣ>��=֮��Ŀո���Ե�
                //bool b = 1 > = 2;

                // TODO:���Ҫ�޸ģ�������Ҫ�޸�spaceCanDelete�жϺ����Խ����������
                if (buffer_read[buffer_choose].buffer[j] != '\0' && ((spaceCanDelete(buffer_read[buffer_choose].buffer[j]) || (i > 0 && spaceCanDelete(buffer_read[buffer_choose].buffer[i - 1])))))
                    //if (buffer_read[buffer_choose].buffer[j] != '\0' && ((isDelimiter(buffer_read[buffer_choose].buffer[j]) || (i > 0 && isDelimiter(buffer_read[buffer_choose].buffer[i - 1])))))
                {
                    //�Ѻ�����ƶ���ǰ��
                    int k = i;
                    buffer_read[buffer_choose].count -= (j - i);
                    for (; buffer_read[buffer_choose].buffer[j] != '\0'; j++, k++)
                        buffer_read[buffer_choose].buffer[k] = buffer_read[buffer_choose].buffer[j];
                    buffer_read[buffer_choose].buffer[k] = '\0';
                    // i--����Ϊԭ��i��λ���ǿո����ڱ�ɾ�����ˣ����Ի���һ����λ
                    i--;
                }

            }
        }

    }

}

//״̬������buffer_end�ж�ȡ��䲢���ֳɵ���  
//����Ҫ����ע�ͣ����/ֻ�ǳ���
//������һ��Ԥ������Ĵ��룬����ָ�Ϊ���ʽ��������ж�

void analysis::spearateStates()
{
    char word[BUFFER_SIZE];
    int count = 0;//��ǰword�е��ַ�����
    bool finish = false;
    int state = 0;//��̬��stateΪ0�ͱ�ʾ���ڳ�̬

    for (int i = 0; i <= buffer_end.count; i++)
    {
        switch (state)
        {
            //��̬
        case 0:
            switch (charKind(buffer_end.buffer[i]))
            {
            case 1://��ĸ
                word[count++] = buffer_end.buffer[i];
                state = 1;
                break;
            case 2://����
                word[count++] = buffer_end.buffer[i];
                state = 2;
                break;
            case 3://$,_
                word[count++] = buffer_end.buffer[i];
                state = 3;
                break;
            case 4://ת���ֻ�����ַ����ڲ�ʹ�ã�������Ϊһ���ַ���������
                word[count++] = buffer_end.buffer[i];
                state = 4;
                break;
            case 5:
                word[count++] = buffer_end.buffer[i];
                state = 5;
                break;
            case 6:
                word[count++] = buffer_end.buffer[i];
                state = 6;
                break;
            case 7:
                word[count++] = buffer_end.buffer[i];
                state = 7;
                break;
            case 8:
                word[count++] = buffer_end.buffer[i];
                state = 8;
                break;
            case 10:
                word[count++] = buffer_end.buffer[i];
                state = 10;
                break;
            default:
                word[count++] = buffer_end.buffer[i];
                break;
            }
            break;
        case 1:
            switch (charKind(buffer_end.buffer[i]))
            {
            case 1:case 2:case 3:
                word[count++] = buffer_end.buffer[i];
                break;
            default:
                word[count] = '\0';
                i--;
                finish = 1;
                state = 9;//����״̬
            }
            break;
        case 2:
            switch (charKind(buffer_end.buffer[i]))
            {
            case 1:
            case 2:
                word[count++] = buffer_end.buffer[i];
                break;
            case 7:
                if (buffer_end.buffer[i] == '.')
                {
                    word[count++] = buffer_end.buffer[i];
                    break;
                }
                else
                {
                    word[count] = '\0';
                    i--;
                    finish = 1;
                    state = 9;//����״̬
                }
                break;
            case 8:
                //������+-��ǰ����Ee
                if ((buffer_end.buffer[i] == '+' || buffer_end.buffer[i] == '-') && (buffer_end.buffer[i - 1] == 'e' || buffer_end.buffer[i - 1] == 'E'))
                {
                    word[count++] = buffer_end.buffer[i];
                    break;
                }
                else
                {
                    word[count] = '\0';
                    i--;
                    finish = 1;
                    state = 9;//����״̬
                    break;
                }
            default:
                word[count] = '\0';
                i--;
                finish = 1;
                state = 9;//����״̬
                break;
            }
            break;
        case 3://����$_����ĸ��һ���Ĳ���
            switch (charKind(buffer_end.buffer[i]))
            {
            case 1:case 2:case 3:
                word[count++] = buffer_end.buffer[i];
                break;
            default:
                word[count] = '\0';
                i--;
                finish = 1;
                state = 9;//����״̬
                break;
            }
            break;
        case 4:
            //�ַ�����ת����������5̬�ڲ����������ﴦ��������'\'
            word[count] = '\0';
            i--;
            finish = 1;
            state = 9;//����״̬
            break;
        case 5:
            word[count++] = buffer_end.buffer[i];
            if (buffer_end.buffer[i] == '"')
            {
                //��ʱһ�����ǳ�̬�����Բ���Ҫ�ж�i��1�Ĺ�ϵ
                if (buffer_end.buffer[i - 1] == '\\')
                {
                }
                else
                {
                    word[count] = '\0';
                    finish = 1;
                    state = 9;
                }
            }
            break;
        case 6:
            word[count++] = buffer_end.buffer[i];
            if (buffer_end.buffer[i] == '\'')
            {
                //����һ�������'\''�����ǵ��ж�
                if (buffer_end.buffer[i - 1] == '\\')
                {
                }
                else
                {
                    word[count] = '\0';
                    finish = 1;
                    state = 9;
                }
            }
            break;
        case 7:
            //Ҫ�������ַ���ֱ�ӽ���
            word[count] = '\0';
            i--;
            finish = 1;
            state = 9;
            break;
        case 8:
            switch (charKind(buffer_end.buffer[i]))
            {
            case 8:case 11:
                word[count++] = buffer_end.buffer[i];
                break;
            default:
                word[count] = '\0';
                i--;
                finish = 1;
                state = 9;
                break;
            }
            break;
        case 9://����̬
            //��ʱword�Ѿ��õ������������\0��β����״̬���ɳ�ʼ״̬
            state = 0;
            count = 0;
            finish = 0;
            i--;
            kindJudge(word);
            break;
        case 10://�ո�����
            switch (charKind(buffer_end.buffer[i]))
            {
            case 10:
                word[count++] = buffer_end.buffer[i];
                break;
            default:
                word[count] = '\0';
                i--;
                finish = 1;
                state = 9;
                break;
            }
            break;

        default:
            break;
        }
        if (buffer_end.buffer[i + 1] == '\0')
        {
            word[count] = '\0';
            kindJudge(word);
            break;
        }
    }

}


unit analysis::Generate_unit(int kind, char* str) {
    //�ж��Ƿ�������
    if (kind == Integer)
    {
        unit r("<INT>", str);
        return r;
    }
    else if (kind == FloatPoint)
    {
        unit r("<FLOAT>", str);
        return r;
    }
    else if (kind == Str)
    {
        unit r("<STRING>", str);
        return r;
    }
    else if (kind == Char)
    {
        unit r("<Char>", str);
        return r;
    }
    //�ж��Ƿ��Ǳ�ʶ��
    else if (kind == SignWord)
    {
        unit r("<ID>", str);
        return r;
    }
    //�ж��Ƿ����
    else if (kind == WrongWord)
    {
        unit r("<WRONG>", str);
        return r;
    }
    //���⴦��main
    else if (kind == KeyWord && !strcmp(str, "main"))
    {
        unit r("<ID>", str);
        return r;
    }
    //�������ַ���
    else
    {
        unit r(str, str);
        return r;
    }
}

//���Զ����е��ã��жϴ��Զ�������ĵ������Ͳ�������ļ�������<���ͣ�ԭֵ>��ͬʱ���analysis_res�ĸ�ֵ
void analysis::kindJudge(char* str)
{
    int kind = 0;
    if (isKeyWord(str) == 1) //�ж��Ƿ�Ϊ�ؼ���
    {
        kind = KeyWord;
        //fprintf(fout, "  [ %s ]  ----  [ �ؼ��� ]\n", str);
    }
    else if (isSignWord(str) == 1)  //�ж��Ƿ�Ϊ��ʶ��
        kind = SignWord;
    else if (isInt(str) == 1)//�ж��Ƿ�Ϊ����
        kind = Integer;
    else if (isFloat(str) == 1)//�ж��Ƿ�Ϊ������
        kind = FloatPoint;
    else if (isMonocularOperator(str) == 1)//�ж��Ƿ�Ϊ��Ŀ�����
        kind = MonocularOperator;
    else if (isBinocularOperator(str) == 1)//�ж��Ƿ�Ϊ˫Ŀ�����
        kind = BinocularOperator;
    else if (isDelimiter(str) == 1)//�ж��Ƿ�Ϊ���        
        kind = Delimiter;
    else if (isBlank(str) == 1)//�ж��Ƿ��ǿո�
        kind = Blank;
    else if (isSeparator(str[0]) == 1 && strlen(str) == 1)
        kind = Separator;
    else if (isBracketsLeft(str[0]) == 1 && strlen(str) == 1)
        kind = BracketsLeft;
    else if (isBracketsRight(str[0]) == 1 && strlen(str) == 1)
        kind = BracketsRight;
    else if (isBracketsLeftBig(str[0]) == 1 && strlen(str) == 1)
        kind = BracketsLeftBig;
    else if (isBracketsRightBig(str[0]) == 1 && strlen(str) == 1)
        kind = BracketsRightBig;
    else if (isEnd(str[0]) == 1 && strlen(str) == 1)
        kind = End;
    else if (isBracketsLeftSquare(str[0]) == 1 && strlen(str) == 1)
        kind = Brackets_Left_Square;
    else if (isBracketsRightSquare(str[0]) == 1 && strlen(str) == 1)
        kind = Brackets_Right_Square;
    else if (isPointArrow(str) == 1)
        kind = Point_Arrow;
    else if (isRegion(str) == 1)
        kind = Region;
    else if (isRegionXigou(str) == 1)
        kind = Region_Xigou;
    else if (strlen(str) == 1 && isColon(str[0]) == 1)
        kind = Colon;
    else if (isPoint(str[0]) == 1 && strlen(str) == 1)
        kind = Point;
    else if (isStr(str))
        kind = Str;
    else if (isChar(str))
        kind = Char;
    else
        kind = WrongWord;
    if (kind != Blank)//�͵��﷨�������е��м��������ո�
    {
        unit t = Generate_unit(kind, str);
        analysis_res.push_back(t);
    }
    printResult(kind, str, 0);
}

void analysis::showAnalysisRes()
{
    vector<unit>::iterator it;
    for (it = analysis_res.begin(); it != analysis_res.end(); it++)
    {
        char ch1[BUFFER_SIZE];
        strcpy(ch1, (*it).type.c_str());
        char ch2[BUFFER_SIZE];
        strcpy(ch2, (*it).value.c_str());
        fprintf(fout_analysis_res, "%s----%s\n", ch1, ch2);
    }
}
void analysis::printResult(int kind, char* str, int opt)
{
    if (opt == 1)
    {
        switch (kind)
        {
        case KeyWord:
            fprintf(fout, "[�ؼ���]----[%s]\n", str);
            break;
        case SignWord:
            fprintf(fout, "[��ʶ��]----[%s]\n", str);
            break;
        case Integer:
            fprintf(fout, "[����]----[%s]\n", str);
            break;
        case FloatPoint:
            fprintf(fout, "[������]----[%s]\n", str);
            break;
        case MonocularOperator:
            fprintf(fout, "[��Ŀ�����]----[%s]\n", str);
            break;
        case BinocularOperator:
            fprintf(fout, "[˫Ŀ�����]----[%s]\n", str);
            break;
        case Delimiter:
            fprintf(fout, "[���]----[%s]\n", str);
            break;
        case WrongWord:
            fprintf(fout, "[�����]----[%s]\n", str);
            break;
        case Blank:
            fprintf(fout, "[�ո�]----[%s]\n", str);
            break;
        case Separator:
            fprintf(fout, "[�ָ���]----[%s]\n", str);
            break;
        case BracketsLeft:
            fprintf(fout, "[������]----[%s]\n", str);
            break;
        case BracketsRight:
            fprintf(fout, "[������]----[%s]\n", str);
            break;
        case BracketsLeftBig:
            fprintf(fout, "[�������]----[%s]\n", str);
            break;
        case BracketsRightBig:
            fprintf(fout, "[�Ҵ�����]----[%s]\n", str);
            break;
        case End:
            fprintf(fout, "[������]----[%s]\n", str);
            break;
        case Str:
            fprintf(fout, "[�ַ���]----[%s]\n", str);
            break;
        case Char:
            fprintf(fout, "[�ַ�]----[%s]\n", str);
            break;
        case Brackets_Left_Square:
            fprintf(fout, "[������]----[%s]\n", str);
            break;
        case Brackets_Right_Square:
            fprintf(fout, "[�ҷ�����]----[%s]\n", str);
            break;
        case Point_Arrow:
            fprintf(fout, "[��ͷ]----[%s]\n", str);
            break;
        case Region:
            fprintf(fout, "[���]----[%s]\n", str);
            break;
        case Region_Xigou:
            fprintf(fout, "[������]----[%s]\n", str);
            break;
        case Colon:
            fprintf(fout, "[ð��]----[%s]\n", str);
            break;
        case Point:
            fprintf(fout, "[���]----[%s]\n", str);
            break;
        default:
            fprintf(fout, "[����]----[%s]\n", str);
            break;
        }
    }
    else
    {
        int WordKindCode = 0;
        WordKindCode = getWordKindCode(kind, str);
        switch (kind)
        {
        case KeyWord:
            fprintf(fout, "[�ؼ���]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case SignWord:
            fprintf(fout, "[��ʶ��]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Integer:
            fprintf(fout, "[����]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case FloatPoint:
            fprintf(fout, "[������]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case MonocularOperator:
            fprintf(fout, "[��Ŀ�����]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case BinocularOperator:
            fprintf(fout, "[˫Ŀ�����]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Delimiter:
            fprintf(fout, "[���]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case WrongWord:
            fprintf(fout, "[�����]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Blank:
            fprintf(fout, "[�ո�]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Separator:
            fprintf(fout, "[�ָ���]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case BracketsLeft:
            fprintf(fout, "[������]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case BracketsRight:
            fprintf(fout, "[������]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case BracketsLeftBig:
            fprintf(fout, "[�������]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case BracketsRightBig:
            fprintf(fout, "[�Ҵ�����]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case End:
            fprintf(fout, "[������]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Str:
            fprintf(fout, "[�ַ���]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Char:
            fprintf(fout, "[�ַ�]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Brackets_Left_Square:
            fprintf(fout, "[������]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Brackets_Right_Square:
            fprintf(fout, "[�ҷ�����]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Point_Arrow:
            fprintf(fout, "[��ͷ]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Region:
            fprintf(fout, "[���]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Region_Xigou:
            fprintf(fout, "[������]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Colon:
            fprintf(fout, "[ð��]----[%d]----[%s]\n", WordKindCode, str);
            break;
        case Point:
            fprintf(fout, "[���]----[%d]----[%s]\n", WordKindCode, str);
            break;
        default:
            fprintf(fout, "[����]----[%s]\n", str);
        }
    }
}

int analysis::getWordKindCode(int kind, char* str)
{
    int ret = 0;
    switch (kind)
    {
    case KeyWord:
        ret = WordCode[str];
        break;
    case SignWord:
        ret = WordCode["signword"];
        break;
    case Integer:
        ret = WordCode["integer"];
        break;
    case FloatPoint:
        ret = WordCode["float"];
        break;
    case MonocularOperator:
        ret = WordCode[str];
        break;
    case BinocularOperator:
        ret = WordCode[str];
        break;
    case Delimiter:
        ret = WordCode[";"];
        break;
    case WrongWord:
        ret = WordCode["wrongword"];
        break;
    case Blank:
        ret = WordCode["blank"];
        break;
    case Separator:
        ret = WordCode[","];
        break;
    case BracketsLeft:
        ret = WordCode["("];
        break;
    case BracketsRight:
        ret = WordCode[")"];
        break;
    case BracketsLeftBig:
        ret = WordCode["{"];
        break;
    case BracketsRightBig:
        ret = WordCode["}"];
        break;
    case End:
        ret = WordCode["#"];
        break;
    case Str:
        ret = WordCode["string"];
        break;
    case Char:
        ret = WordCode["char"];
        break;
    case Brackets_Left_Square:
        ret = WordCode["["];
        break;
    case Brackets_Right_Square:
        ret = WordCode["]"];
        break;
    case Point_Arrow:
        ret = WordCode["->"];
        break;
    case Region:
        ret = WordCode["::"];
        break;
    case Region_Xigou:
        ret = WordCode["::~"];
        break;
    case Colon:
        ret = WordCode[":"];
        break;
    case Point:
        ret = WordCode["."];
        break;
    default:
        ret = -100;
        break;
    }
    //TODO:�����޸�
    return ret;
}

analysis::analysis()
{
    buffer_choose = 0;
    note_flag = 0;

    string cfile_name = "code_in.txt";
    string c;
    cout << "ϵͳĬ�϶���Դ�����ļ�code_in.txt" << endl << endl;
    cout << "������Ҫ���дʷ����﷨������Դ�����ļ����������ѡ��Ĭ���ļ���������e��\n";
    cin >> c;
    if (c != "e")
    {
        cfile_name = c;
        cout << "���ڶ����ļ�" << cfile_name << endl;
    }

    else {
        cout << "���ڶ���Ĭ���ļ�" << cfile_name << endl;
    }

    fin = fopen(cfile_name.c_str(), "r");
    if ((fin = fopen(cfile_name.c_str(), "r")) == NULL)
    {

        throw CODE_FILE_OPEN_ERROR;
        return;
    }

    fout = fopen("../work_dir/Lexical_Result.txt", "w");
    fout_pre = fopen("../work_dir/Pre-Processed_Code.txt", "w");
    fout_lable = fopen("../work_dir/Word_Lable.txt", "w");
    fout_analysis_res = fopen("../work_dir/Lex_to_Parse.txt", "w");
    //map����ֵ
    const int keyword_size = 24;
    const int monocular_operator_size = 13;
    const int binocular_operator_size = 14;
    int cnt = 0;
    //�ؼ���
    for (int i = 0; i < keyword_size; i++)
        WordCode[keyword[i]] = ++cnt;
    //���
    for (int i = 0; i < monocular_operator_size; i++)
        WordCode[monocular_operator[i]] = ++cnt;
    for (int i = 0; i < binocular_operator_size; i++)
        WordCode[binocular_operator[i]] = ++cnt;
    WordCode["signword"] = ++cnt;
    WordCode["wrongword"] = ++cnt;
    WordCode["blank"] = ++cnt;
    WordCode[";"] = ++cnt;
    WordCode[","] = ++cnt;
    WordCode["("] = ++cnt;
    WordCode[")"] = ++cnt;
    WordCode["{"] = ++cnt;
    WordCode["}"] = ++cnt;
    WordCode["#"] = ++cnt;
    WordCode["."] = ++cnt;
    WordCode["["] = ++cnt;
    WordCode["]"] = ++cnt;
    WordCode["->"] = ++cnt;
    WordCode[";;"] = ++cnt;
    WordCode["::~"] = ++cnt;
    WordCode[":"] = ++cnt;
    //����
    WordCode["integer"] = ++cnt;
    WordCode["float"] = ++cnt;

    //��ӡ�����ֱ���ֵ
    map<string, int>::iterator iter;
    iter = WordCode.begin();
    while (iter != WordCode.end()) {
        //cout << iter->first << " : " << iter->second << endl;
        fprintf(fout_lable, "%s : %d\n", iter->first.c_str(), iter->second);
        iter++;
    }
    /*
    * ����isFloat�жϺ���e�ĸ�����
    * Ԥ�����1��0��0��0��0��1��1��1
    * ʵ�����1��0��0��0��1��1��1*/
    /*
    char t[][20] = { "7e5","8eeee","e80","7e","+70","+70.0","70.5e-56.9","70e56"};
    for (int i = 0; i < 8; i++)
    {
        if (i == 6)
            cout << "wrong" << endl;
        cout << isFloat(t[i]) << endl;
    }
        */

}
analysis::~analysis()
{
    fclose(fin);
    fclose(fout);
}