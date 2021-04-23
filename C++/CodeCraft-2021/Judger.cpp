#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

vector<string> spt(const string& s, const string& seperator) {
    vector<string> ss;
    int p = 0;
    int cur = 0;
    while (p != s.npos) {
        p = s.find(seperator, p + 1);
        ss.push_back(s.substr(cur, p - cur));
        cur = p + seperator.size();
    }
    return ss;
}

int getInt() {
    string s;
    getline(cin, s);
    return atoi(s.c_str());
}

class Host {
public:
    string name;
    int core;
    int memory;
    int price;
    int consumption;

    Host() {}
    Host(string n, int c, int m, int p, int con) :
        name(n), core(c), memory(m), price(p), consumption(con) {}
};

// 可选服务器列表
class HostList {
public:
    map<string, Host> hostList;
    int max = 0;
    int max_num = 0;

    HostList() {}

    void build(vector<string>& data) {
        for (int i = 0; i < data.size(); i++) {
            string s = data[i];
            vector<string> d = spt(s.substr(1, s.size() - 2), ", ");
            hostList[d[0]] = (Host(d[0], atoi(d[1].c_str()) / 2, atoi(d[2].c_str()) / 2,
                atoi(d[3].c_str()), atoi(d[4].c_str())));
        }
    }

    Host get(string name) {
        return hostList[name];
    }

    bool find(string name) {
        return hostList.find(name) != hostList.end();
    }
};

// 虚拟机类
class VM {
public:
    int core, memory, doubleNode;

    VM() {}
    VM(int c, int m, int d) : core(c), memory(m), doubleNode(d) {}
};

// 虚拟机列表
class VMList {
public:
    map<string, VM> vmList;
    VMList() {}

    void build(vector<string>& data) {
        for (auto i = data.begin(); i != data.end(); i++) {
            vector<string> d = spt((*i).substr(1, (*i).size() - 2), ", ");
            if (d[3] == "1")
                vmList[d[0]] = VM(atoi(d[1].c_str()) / 2, atoi(d[2].c_str()) / 2, atoi(d[3].c_str()));
            else
                vmList[d[0]] = VM(atoi(d[1].c_str()), atoi(d[2].c_str()), atoi(d[3].c_str()));
        }
    }

    VM get(string name) {
        return vmList[name];
    }
};

class Instance {
public:
    string name;
    string type;
    int node;

    Instance() {}
    Instance(string n, string t, int no) : name(n), type(t), node(no) {}
};

class Machine {
public:
    string type;
    int max_core;
    int max_memory;
    int A_core = 0;
    int B_core = 0;
    int A_memory = 0;
    int B_memory = 0;
    map<string, Instance> instance_list;

    Machine(string t, int m_c, int m_m) : type(t), max_core(m_c), max_memory(m_m) {}

    void add(string name, string type, int node) {
        instance_list[name] = (Instance(name, type, node));
    }

    void move(string name) {
        instance_list.erase(name);
    }
};

struct Purchase {
public:
    string type;
    int num;

    Purchase(string t, int n) : type(t), num(n) {}
};

struct Migration {
public:
    string ID;
    int target_host;
    int target_node;

    Migration(string I, int h, int n) : ID(I), target_host(h), target_node(n) {}
};

struct ProcessOrder {
public:
    int target_host;
    int target_node;

    ProcessOrder(int h, int n) : target_host(h), target_node(n) {}
};

struct VmInHost {
    string vmtype;
    int host_num;

    VmInHost() {}
    VmInHost(string v, int h) : vmtype(v), host_num(h) {}
};

class Judge {
public:
    map<string, VmInHost> vm;
    vector<Machine> machines;
    HostList hostList;
    VMList vmList;
    vector<vector<string>> workorders;
    vector<int> add_num;
    vector<Migration> migrations;
    vector<Purchase> purchases;
    vector<ProcessOrder> processorders;
    vector<string> output;
    string work_order_path = "training-1.txt";
    string output_path = "output_trying.txt";
    int d;
    int counter = 0;

    Judge() {}

    void readOrder() {
        ifstream fin(work_order_path);
        vector<string> host_input, vm_input;
        string s;
        int v = 0, h = 0;
        getline(fin, s);
        h = atoi(s.c_str());
        for (int i = 0; i < h; i++) {
            getline(fin, s);
            host_input.push_back(s);
        }
        getline(fin, s);
        v = atoi(s.c_str());
        for (int i = 0; i < v; i++) {
            getline(fin, s);
            vm_input.push_back(s);
        }
        getline(fin, s);
        d = atoi(s.c_str());
        for (int i = 0; i < d; i++) {
            int l;
            getline(fin, s);
            l = atoi(s.c_str());
            add_num.push_back(l);
            vector<string> d_order;
            for (int j = 0; j < l; j++) {
                getline(fin, s);
                d_order.push_back(s);
            }
            workorders.push_back(d_order);
        }
        hostList.build(host_input);
        vmList.build(vm_input);
        fin.close();
    }

    int readOutput() {
        ifstream fin(output_path);
        string s;
        while (getline(fin, s)) {
            output.push_back(s);
        }
        return 0;
    }

    int getADay(int num) {
        purchases.clear();
        migrations.clear();
        processorders.clear();
        vector<string> temp = spt(output[counter].substr(1, output[counter].size() - 2), ", ");

        if (temp[0] != "purchase") {
            cout << output[counter] << endl;
            cout << "输出错误，期望得到purchase!" << endl;
            return 1;
        }
        counter++;
        int m = atoi(temp[1].c_str());
        for (int j = 0; j < m; j++) {
            temp = spt(output[counter].substr(1, output[counter].size() - 2), ", ");
            if (atoi(temp[1].c_str()) <= 0 || !hostList.find(temp[0])) {
                cout << output[counter] << endl;
                cout << "错误的新增服务器数量, 或者服务器类型不存在！" << endl;
                return 1;
            }
            counter++;
            purchases.push_back(Purchase(temp[0], atoi(temp[1].c_str())));
        }

        temp = spt(output[counter].substr(1, output[counter].size() - 2), ", ");

        if (temp[0] != "migration") {
            cout << output[counter] << endl;
            cout << "期望得到migration！！" << endl;
            return 1;
        }
        counter++;
        m = atoi(temp[1].c_str());
        for (int j = 0; j < m; j++) {
            temp = spt(output[counter].substr(1, output[counter].size() - 2), ", ");
            int node = 2;
            if (temp.size() == 3) {
                if (vmList.get(vm[temp[0]].vmtype).doubleNode) {
                    cout << output[counter] << endl;
                    cout << "双节点虚拟机放入单节点" << endl;
                    return 1;
                }
                if (temp[2] == "A")
                    node = 0;
                else if (temp[2] == "B")
                    node = 1;
                else {
                    cout << output[counter] << endl;
                    cout << "节点错误" << endl;
                    return 1;
                }
            }
            else {
                if (!vmList.get(vm[temp[0]].vmtype).doubleNode) {
                    cout << output[counter] << endl;
                    cout << "单节点虚拟机放入双节点" << endl;
                    return 1;
                }
            }
            counter++;
            migrations.push_back(Migration(temp[0], atoi(temp[1].c_str()), node));
        }

        for (int j = 0; j < num; j++) {
            temp = spt(output[counter].substr(1, output[counter].size() - 2), ", ");

            int h = atoi(temp[0].c_str());
            int n = 2;
            
            if (temp.size() == 2)
                if (temp[1] == "A") {
                    n = 0;
                }
                else if (temp[1] == "B")
                    n = 1;
                else {
                    cout << output[counter] << endl;
                    cout << "节点错误" << endl;
                    return 1;
                }
            counter++;
            processorders.push_back(ProcessOrder(h, n));
        }
    }

    int add(string name, string vmtype, int host, int node) {
        if (machines.size() <= host) {
            cout << output[counter] << endl;
            cout << "服务器ID不存在" << endl;
            return 1;
        }
        VM v = vmList.get(vmtype);
        vm[name] = VmInHost(vmtype, host);
        if (node == 2) {
            machines[host].A_core += v.core;
            machines[host].A_memory += v.memory;
            machines[host].B_core += v.core;
            machines[host].B_memory += v.memory;
        }
        else if (node == 0) {
            machines[host].A_core += v.core;
            machines[host].A_memory += v.memory;
        }
        else if (node == 1) {
            machines[host].B_core += v.core;
            machines[host].B_memory += v.memory;
        }

        if (machines[host].B_core > machines[host].max_core || machines[host].A_core > machines[host].max_core ||
            machines[host].B_memory > machines[host].max_memory || machines[host].A_memory > machines[host].max_memory) {
            cout << "内存或cpu超限" << endl;
            return 1;
        }
        machines[host].add(name, vmtype, node);
        return 0;
    }

    int del(string name) {
        if (vm.find(name) == vm.end()) {
            cout << "虚拟机不存在" << endl;
            return 1;
        }
        VM v = vmList.get(vm[name].vmtype);
        int host = vm[name].host_num;
        if (machines[host].instance_list[name].node == 2) {
            machines[host].A_core -= v.core;
            machines[host].A_memory -= v.memory;
            machines[host].B_core -= v.core;
            machines[host].B_memory -= v.memory;
        }
        else if (machines[host].instance_list[name].node == 0) {
            machines[host].A_core -= v.core;
            machines[host].A_memory -= v.memory;
        }
        else if (machines[host].instance_list[name].node == 1) {
            machines[host].B_core -= v.core;
            machines[host].B_memory -= v.memory;
        }
        vm.erase(name);
        machines[host].move(name);
        return 0;
    }

    void addHost(string type, int core, int memory) {
        Machine m(type, core, memory);
        machines.push_back(m);
    }

    int panduan() {
        for (int i = 0; i < workorders.size(); i++) {
            int c = 0;
            for (int j = 0; j < workorders[i].size(); j++) {
                vector<string> d = spt((workorders[i][j]).substr(1, (workorders[i][j]).size() - 2), ", ");
                if (d[0] == "add") {
                    c++;
                }
            }
            if (getADay(c)) {
                cout << "从vector中读取输出错误" << endl;
                return 1;
            }
            for (auto j : purchases) {
                for (int k = 0; k < j.num; k++) {
                    addHost(j.type, hostList.get(j.type).core, hostList.get(j.type).memory);
                }
            }
            for (auto j : migrations) {
                if (del(j.ID)) {
                    cout << "迁移虚拟机过程中，虚拟机ID错误" << endl;
                    return 1;
                }
                if (add(j.ID, vm[j.ID].vmtype, j.target_host, j.target_node)) {
                    cout << "迁移虚拟机过程中，迁入服务器错误" << endl;
                    return 1;
                }
            }
            c = 0;
            for (int j = 0; j < workorders[i].size(); j++) {
                vector<string> d = spt((workorders[i][j]).substr(1, (workorders[i][j]).size() - 2), ", ");
                if (d[0] == "add") {
                    if (add(d[2], d[1], processorders[c].target_host, processorders[c].target_node)) {
                        cout << "添加虚拟机超限" << endl;
                        return 1;
                    }
                    c++;
                }
                else {
                    del(d[1]);
                }
            }
        }
        return 0;
    }

};

int main() {
    Judge judge;
    judge.readOrder();
    if (judge.readOutput()) {
        cout << "读取输出错误！！" << endl;
        return 1;
    }
    if (judge.panduan()) {
        cout << "判断不通过" << endl;
        return 1;
    }
    cout << "success!!!" << endl;
}