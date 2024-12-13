#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
using namespace std;


struct Node{
    char symbol;
    uint32_t frq;
    Node *left, *right;

    bool operator<(Node anoter_one) const{
        return frq < anoter_one.frq;
    }
    Node(char x, uint32_t y){symbol = x; frq = y; left=right=NULL;}
    Node(char x, uint32_t y, Node *L, Node *R){symbol = x; frq = y; left= L; right=R;}
};
struct MyCompare
{
    bool operator()(const Node* l, const Node* r) const { return l->frq < r->frq; }
};
void mktables_from_tree(Node *root, map<char, string> *table, map<string, char> *table2, string code = ""){
    if(root->symbol == static_cast<char>(0x00) && root->left != NULL && root->right != NULL){
        mktables_from_tree(root->left, table, table2, code + "0");
        mktables_from_tree(root->right, table, table2, code + "1");
        return;
    }
    else{
        table[0][root->symbol] = code;
        table2[0][code] = root->symbol;
        cout << root->symbol << ":" << code << endl;
        return;
    }
}
void delete_tree(Node *root){
    if (root->left != NULL && root->right != NULL){
        delete_tree(root->left);
        delete_tree(root->right);
    }
    else {delete root;}
    return;
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    string current_str;
    if (argc == 1){
        current_str = "Очень сложный TEXT\n\t12345\n!@#$%^&*()";
    }
    else if((argc == 2)){
        current_str = argv[1];
    }
    //Вывод кодируемго сообщения
    cout << "Input message:" << endl;
    cout << current_str << endl;
    //Запись частоты каждого байта в список
    int char_frequency[256] = {0};
    for (int j = 0; j < current_str.size(); j++){
        char current_byte = current_str[j];
        char_frequency[static_cast<unsigned char>(current_byte)]++;
    }
    //Создание и заполнение списка ненулевыми частотами с соответствующими символами
    list<Node*> char_frq_list;
    for(int i = 0; i < 256; i++){
        if(char_frequency[static_cast<unsigned char>(i)] != 0){
                //Запись ненулевых частот в list
                char_frq_list.push_back(new Node(static_cast<char>(i), char_frequency[i]));
        }
    }
    //Построение дерева
    while (char_frq_list.size() != 1){
        char_frq_list.sort(MyCompare());

        Node *LeftNode= char_frq_list.front();
        char_frq_list.pop_front();
        Node *RightNode = char_frq_list.front();
        char_frq_list.pop_front();

        char_frq_list.push_front(new Node(static_cast<char>(0x00), (LeftNode->frq + RightNode->frq), LeftNode, RightNode));
    }
    //Создание таблицы кодирования и декодирования
    Node *root = char_frq_list.front();
    map<char, string> *encode_table = new map<char, string>;
    map<string, char> *decode_table = new map<string, char>;
    cout << "Encoding table:" << endl;
    mktables_from_tree(root,encode_table, decode_table);
    //Запись кода в буфер
    vector<char> output_buf;
    for (int i = 0; i < current_str.size(); i++){
        string code_to_write = encode_table[0][current_str[i]];
        for (int j = 0; j < code_to_write.size(); j++){
            output_buf.push_back(code_to_write[j]);
        }
    }
    //Вывод буфера с кодом в консоль
    cout << "Encoded text:" << endl;
    for (int i = 0; i < output_buf.size(); i++){
        cout << output_buf[i];
    }
    cout << endl;
    //Создание и заполнение буфера декодированного текста
    vector<char> decoder_buf;
    string current_encode = "";
    for (int i = 0; i < output_buf.size(); i++){
        current_encode = current_encode + output_buf[i];
        if (decode_table->find(current_encode) == decode_table->end()){
            continue;
        }
        else {
            decoder_buf.push_back(decode_table[0][current_encode]);
            current_encode = "";
        }
    }
    //Вывод буфера с декодированным текстом в консоль
    cout << "Decoded text:" << endl;
    for (int i = 0; i < decoder_buf.size(); i++){
        cout << decoder_buf[i];
    }
    cout << endl;
    //Освобождение памяти и завершение программы
    delete_tree(root);
    delete encode_table;
    delete decode_table;
    return 0;
}
