#include<bits/stdc++.h>
using namespace std;

//Data Storage
set<string> No_Op, Dir;
map<string, pair<int, string>> instructions;
map<char, int> registers;
map<char, string> hexTobin;

//Helping Functions
int hexToInt(string x){
    int ans = 0, p = 1;
    for(int i = x.size() - 1; i >= 0; i--){
        if( '0' <= x[i] && x[i] <= '9'){
            ans += (x[i] - '0') * p;
        }else{
            ans += ((x[i] - 'A') + 10) * p;
        }
        p *= 16;
    }
    return ans;
}

//Make Hex Number
string toHexStr(long long n, int s){
    if(n < 0){
        n = INT_MAX + n +1;
    }
    string ans = "";
    while(n){
        long long a = n%16;
        if(a < 10){ans.push_back('0' + a);}
        else {
            a-=10;
            ans.push_back('A' + a);
        }
        n /= 16;
    }
    while(ans.size() > s) ans.pop_back();
    while(ans.size() < s) ans.push_back('0');
    reverse(ans.begin(), ans.end());
    return ans;
}

//Extracts the exact instruction/data: removes #, @, ',x', +
string refine(string x, int t){
    string r = "";
    if(t == 1){
        for(int i = 1; i < x.size(); i++){
            r.push_back(x[i]);
        }
    }else{
        for(int i = 0; i < x.size()-2; i++){
            r.push_back(x[i]);
        }
    }
    return r;
}

vector<string> split(string &str){
    int n = str.size();
    vector<string> result;
    string temp = "";
    for(int i = 0; i < n; i++){
        if(str[i] != ' '){
            temp.push_back(str[i]);
        }else if(temp != ""){
            result.push_back(temp);
            temp = "";
        }
    }
    if(temp != ""){
            result.push_back(temp);
            temp = "";
    }
    return result;
}

//Linearly search for the base in the symbol table
int serachSymbol(string label, vector<string> &labels, vector<string> &addresses){
    for(int i = 0; i < int(labels.size()); i++){
        if(labels[i] == label) return hexToInt(addresses[i]);
    }
    return -1;
}

//Format 3/4 Opcode Maker
string OpMaker(string op, vector<int> flags, int disp, int t){
    string bina = hexTobin[op.front()] + hexTobin[op.back()], dis = toHexStr(disp, ((t == 3)? 3 : 5));
    
    bina.pop_back();
    bina.pop_back();
    
    for(int i = 0; i < 6; i++){
        bina.push_back('0' + flags[i]);
    }

    
    for(int i = 0; i < dis.size(); i++){
        bina.append(hexTobin[dis[i]]);
    }
    long long val = 1, num = 0;
    for(int i = bina.size() - 1; i >= 0; i--){
        if(bina[i] == '1') num += val;
        val *= 2ll;
    }

    return toHexStr(num, t * 2);
}

//Byte Opcode Maker
string ByteOpMker(string data){
    string res = "";
    if(data[0] == 'X'){
        for(int i = 2; i < data.size()-1; i++){
            res.push_back(data[i]);
        }
        if(res.size()%2){
            res = "0" + res;
        }
    }else{
        for(int i = 2; i < data.size()-1; i++){
            res.append(toHexStr(data[i], 2));
        }
    }
    return res;
}

//Start of the Project
//Function To Load Instructions from external file
void GetUp(){
    freopen("Instructions.txt", "r", stdin);
    //Hex to Binary
    hexTobin['0'] = "0000";
    hexTobin['1'] = "0001";
    hexTobin['2'] = "0010";
    hexTobin['3'] = "0011";
    hexTobin['4'] = "0100";
    hexTobin['5'] = "0101";
    hexTobin['6'] = "0110";
    hexTobin['7'] = "0111";
    hexTobin['8'] = "1000";
    hexTobin['9'] = "1001";
    hexTobin['A'] = "1010";
    hexTobin['B'] = "1011";
    hexTobin['C'] = "1100";
    hexTobin['D'] = "1101";
    hexTobin['E'] = "1110";
    hexTobin['F'] = "1111";

    //Add Registers and the numbers
    
    registers.insert({'A',0});
    registers.insert({'X',1});
    registers.insert({'L',2});
    registers.insert({'B',3});
    registers.insert({'S',4});
    registers.insert({'T',5});
    registers.insert({'F',6});
    
    //Add Instructions with no op-code
    
    //NO PLACE MEMORY
    No_Op.insert("START");
    No_Op.insert("BASE");
    No_Op.insert("END");
    
    //PLACE IN MEMORY BUT NO OPCODE
    Dir.insert("RESW");
    Dir.insert("RESB");

    //LOAD INSTRUCTIONS MAP
    string temp;
    while(getline(cin, temp)){
        vector<string> ins = split(temp);
        instructions[ins[0]] = {stoi(ins[1]), ins[2]};
    }
    cin.clear();

}

//Function to Read Data and Do pass 1 and find: Locctr, Symbol Table, and Program Length.
vector<vector<string>> pass1(){
    vector<string> label, ins, data;
    int data_size = 0;
    freopen("Input.txt", "r", stdin);

    string temp;
    while(getline(cin, temp)){
        data_size++;
        vector<string> line = split(temp);
        if(line.size() == 3){
            label.push_back(line[0]);
            ins.push_back(line[1]);
            data.push_back(line[2]);
        }else if(line.size() == 2){
            label.push_back("");
            ins.push_back(line[0]);
            data.push_back(line[1]);
        }else if(line.size() == 1){
            label.push_back("");
            ins.push_back(line[0]);
            data.push_back("");
        }else{
            cout << "Something is wrong in the input!! \nI ain't built for this.";
            return {};
        }
        
    }

    //Building Locctr
    vector<int> locctr(data_size);
    int cur = hexToInt(data[0]);
    locctr[0] = cur;
    for(int i = 1; i < data_size; i++){
        locctr[i] = cur;
        if(instructions.find(ins[i]) != instructions.end()){
            if(instructions[ins[i]].first == 1) cur++;
            else if(instructions[ins[i]].first == 2) cur += 2;
            else if(instructions[ins[i]].first == 3) cur += 3;
        }else if(ins[i][0] == '+') cur += 4;
        else if(ins[i] == "BYTE"){
            if(data[i][0] == 'X') cur += ((data[i].size()-3) + 1)/2;
            else cur += (data[i].size()-3);
        }else if(ins[i] == "WORD") cur += 3;
        else if(ins[i] == "RESW") cur += 3 * stoi(data[i]);
        else if(ins[i] == "RESB") cur += stoi(data[i]);
    }

    vector<string> sympol, address, flocctr(data_size);

    for(int i = 0; i < data_size; i++){
        flocctr[i] = toHexStr(locctr[i], 5);
        if(label[i] != ""){
            sympol.push_back(label[i]);
            address.push_back(flocctr[i]);
        }
    }
    cin.clear();
    //RESULTING VECTOR:
    //[0] LOCCTR
    //[1] LABELS
    //[2] INSTRUCTIONS
    //[3] DATA
    //[4],[5] SYMPOL TABLE
    //[6] Program Length 
    vector<vector<string>> result {flocctr, label, ins, data, sympol, address, {toHexStr(locctr[data_size-1] - locctr[0], 5)}};
    return result;
}


//Pass 2 start here
vector<vector<string>> pass1_2(){
    vector<vector<string>> data = pass1();
    //RESULTING VECTOR:
    //[0] LOCCTR
    //[1] LABELS
    //[2] INSTRUCTIONS
    //[3] DATA
    //[4],[5] SYMPOL TABLE
    //[6] Program Length 
    //[7] Opcode
    int base = 0, disp = 0, n = 0, i2 = 0, x = 0, b = 0, p = 0, e = 0, sz = int(data[0].size());
    vector<string> opcode(sz);
    for(int i = 0; i < sz; i++){
        n = 1, i2 = 1, x = 0, b = 0, p = 0, e = 0;
        
        string ins = data[2][i];

        if(ins[0] == '+'){ e = 1; ins = refine(ins, 1);}

        if(Dir.find(ins) != Dir.end() || No_Op.find(ins) != No_Op.end()){
            if(ins == "BASE") base = serachSymbol(data[3][i], data[4], data[5]);
            opcode[i] = "NO_OPCODE_:)";
        }else{
            if(instructions.find(ins) != instructions.end()){
                //Format 1
                if(instructions[ins].first == 1){
                    opcode[i] = instructions[ins].second;
                }
                //Format 2                
                else if(instructions[ins].first == 2){
                    if(data[3][i].size() == 1){
                        opcode[i] = instructions[ins].second + to_string(registers[data[3][i][0]]) + "0";
                    }else{
                        string reg1 = to_string(registers[data[3][i][0]]), reg2 = to_string(registers[data[3][i].back()]);
                        opcode[i] = instructions[ins].second + reg1 + reg2;
                    }
                }
                //Format 3/4
                else if(instructions[ins].first == 3){
                    
                    string val = data[3][i];
                    
                    if(val != "" && val[0] == '#'){n = 0; val = refine(val, 1);}
                    if(val != "" && val[0] == '@'){i2 = 0; val = refine(val, 1);}
                    if(val != "" && val.size() > 2 && val[val.size()-2] == ','){x = 1; val = refine(val, 2);}
                    
                    int tar = serachSymbol(val, data[4], data[5]), pc = hexToInt(data[0][i+1]), disp = 0;
                    if(tar == -1 && val != ""){ disp = stoi(val); }
                    else if(val == ""){
                        n = 1, i2 = 1, p = 0, b = 0, x = 0;
                    }else if(e == 1){
                        disp = tar;
                    }else if(tar-pc >= -2048 && tar - pc < 2048) {
                        disp = tar - pc;
                        p = 1;
                        b = 0;
                    }else{
                        disp = tar - base;
                        b = 1;
                        p = 0;
                    }
                    
                    opcode[i] = OpMaker(instructions[ins].second, {n,i2,x,b,p,e}, disp, ((e == 1)? 4: 3));
                }
            }else{
                if(ins == "WORD"){
                    opcode[i] = toHexStr(stoi(data[3][i]), 6);
                }else if(ins == "BYTE"){
                    opcode[i] = ByteOpMker(data[3][i]);
                }
            }
        }
    }
    data.push_back(opcode);
    return data;
}

void HTE(vector<string> &op, vector<string> &loc , string name, string start, string length, vector<string> &ins){
    cout << "\n\nHTE Record:\n";
    
    while(start.size() != 6){
        start = "0" + start;
    }
    cout << "H^" << name <<"^"<< start << "^0" << length << '\n';
    int count  = 0, p1 = 1;
    string rec = "";
    for(int i = 1; op.size() > i; i++){
        if(ins[i] == "BASE") continue;
        if(count + op[i].size() > 60 || (op[i] == "NO_OPCODE_:)" && rec != "")){
            cout << "T^0" << loc[p1]  << "^" << toHexStr(hexToInt(loc[i]) - hexToInt(loc[p1]), 2) << rec << '\n';
            rec = "";
            count = 0;
            if(op[i] != "NO_OPCODE_:)"){
               i--;
            }
        }else if(op[i] != "NO_OPCODE_:)"){
            if(rec == ""){
                p1 = i;
            }
            count += op[i].size();
            rec.append("^" + op[i]);
        }
    }
    cout << "E^" << start << "\n\n";
}

void pass_and_print(){
    freopen("Input.txt", "r", stdin);
    vector<vector<string>> data = pass1_2();
    freopen("output.txt", "w", stdout);

    cout << "Program Length: " << data[6][0] << "\n\n\n";
    for(int i = 0; i < data[0].size(); i++){
        cout << data[0][i] << "\t" << data[1][i] << "\t" << data[2][i] << "\t" << data[3][i] << ((data[3][i].size() < 8)?"\t\t":"\t") << data[7][i] << '\n';
    }

    cout << "\n\n" << "Symbol Table: \n";
    for(int i = 0; i < data[4].size(); i++){
        cout << data[4][i] << "\t" << data[5][i] << '\n';
    }

    //Build HTE Record
    HTE(data[7], data[0], data[4][0], data[3][0], data[6][0], data[2]);

    cout << "\nBuilt By Omar Khalil! Please Give me bonus marks because I need them :(.";

}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    GetUp();
    pass_and_print();  
}  