#include "iostream"
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <valarray> 
#include <array>
#include <algorithm>
#include <ctime>
#include <unordered_map>
#include "utils.cpp"
using namespace std;

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
	unordered_map<string, VM> vmList;
	VMList() {}
	int max_core = 0, max_mem = 0;

	void build(vector<string>& data) {
		for (auto i = data.begin(); i != data.end(); i++) {
			vector<string> d = spt((*i).substr(1, (*i).size() - 2), ", ");
			if (d[3] == "1"){
				vmList[d[0]] = VM(atoi(d[1].c_str()) / 2, atoi(d[2].c_str()) / 2, atoi(d[3].c_str()));
				max_core = max(max_core, atoi(d[1].c_str()) / 2);
				max_mem = max(max_mem, atoi(d[2].c_str()) / 2);
			}
			else {
				vmList[d[0]] = VM(atoi(d[1].c_str()), atoi(d[2].c_str()), atoi(d[3].c_str()));
				max_core = max(max_core, atoi(d[1].c_str()));
				max_mem = max(max_mem, atoi(d[2].c_str()));
			}
				
		}
	}

	VM get(string name) {
		return vmList[name];
	}

	bool find(string name) {
		return vmList.find(name) != vmList.end();
	}
};


// 服务器类
class Host {
public:
	string name;
	int core;
	int memory;
	int price;
	int consumption;
	double core_score;
	double memory_score;

	Host() {}
	Host(string n, int c, int m, int p, int con) :
		name(n), core(c), memory(m), price(p), consumption(con) {
			core_score = (double)consumption / ((double)core );
			memory_score = (double)consumption / ((double) memory);
		}
};

// 可选服务器列表
class HostList {
public:
	vector<Host> hostList;
	double max1 = 0;
	int max_num1 = 0;
	double min2 = 0;
	int min_num2 = 0;

	HostList() {}

	void build(vector<string>& data) {
		for (int i = 0; i < data.size(); i++) {
			string s = data[i];
			vector<string> d = spt(s.substr(1, s.size() - 2), ", ");
			hostList.push_back(Host(d[0], atoi(d[1].c_str()) / 2, atoi(d[2].c_str()) / 2,
				atoi(d[3].c_str()), atoi(d[4].c_str())));
		}
	}

	void getFittest1(VMList& vmList) {
		for (int i = 0; i < hostList.size(); i++) {
			if ((double)hostList[i].core / (double)hostList[i].memory > 0.8 && (double)hostList[i].core / (double)hostList[i].memory <= 1.2) {
				if(hostList[i].core >= vmList.max_core && hostList[i].memory >= vmList.max_mem) {
					double c = hostList[i].consumption / hostList[i].price;
					if(max1 < c) {
						max1 = c;
						max_num1 = i;
					}
					if(min2 > hostList[i].price) {
						min2 = hostList[i].price;
						min_num2 = i;
					}
				} 
			}
		}
	}

	int pronum1() {
		return max_num1;
	}
	int pronum2() {
		return min_num2;
	}
};



// 返回每天的输出信息
struct daily_output {
public:
	vector<string> daily_add_vm;
	vector<string> daily_add_host;

	daily_output(vector<string> vm, vector<string> host) : daily_add_vm(vm), daily_add_host(host) {}
};

struct daily_output1 {
public:
	vector<string> daily_add_vm;
	vector<string> daily_add_host_name;
	vector<int> daily_add_host_num;
	vector<string> daily_move_vm;

	daily_output1(vector<string> vm, vector<string> d_a_h_name, vector<int> d_a_h_num, vector<string> move) : daily_add_vm(vm), daily_add_host_name(d_a_h_name), daily_add_host_num(d_a_h_num), daily_move_vm(move) {}
};

// 返回虚拟机放置信息
struct Choose {
public:
	int i, node;

	Choose() {}
	Choose(int i, int node) : i(i), node(node) {}
};

// 虚拟机与服务器对应列表
struct VmInHost {
	string vmtype;
	int host_num;
	int nodeA;
	int nodeB;

	VmInHost() {}
	VmInHost(string v, int h, int A, int B) : vmtype(v), host_num(h), nodeA(A), nodeB(B) {}
};

struct daily_add {
	int num;
	string name;
	string vmtype;
	int count;

	daily_add(int no, string n, string v, int c) : num(no), name(n), vmtype(v), count(c) {}
};

struct daily_del {
	int num;
	string name;

	daily_del(int no, string n) : num(no), name(n) {}
};

class Instance {
public:
    string name;
    string type;
    int node;
	int size;

    Instance() {}
    Instance(string n, string t, int no, int s) : name(n), type(t), node(no), size(s) {}
};

class Machine {
public:
    int consumption;
	int price;
    int max_core;
    int max_memory;
    int A_core = 0;
    int B_core = 0;
    int A_memory = 0;
    int B_memory = 0;
	double A_core_score;
	double A_memory_score;
	double B_core_score;
	double B_memory_score;
    map<string, Instance> instance_list;

    Machine(int c, int p, int m_c, int m_m, double c_s, double m_s) : consumption(c), price(p), max_core(m_c), max_memory(m_m), 
																	A_core_score(c_s), A_memory_score(m_s), B_core_score(c_s), B_memory_score(m_s) {
		A_core = m_c;
		B_core = m_c;
		A_memory = m_m;
		B_memory = m_m;
	}

    void add(string name, string type, int node, int s) {
        instance_list[name] = (Instance(name, type, node,s));
    }

    void move(string name) {
		instance_list.erase(name);
    }
};

bool sortFun(array<double, 3>& c1, array<double, 3>& c2) {
	return c1[2] < c2[2];//升序排列  
}

bool sortFun1(array<int, 2>& c1, array<int, 2>& c2) {
	return c1[1] > c2[1];//降序排列  
}

bool sortVM(pair<string, Instance>& p1, pair<string, Instance>& p2){
	return p1.second.size > p2.second.size;
}

// 机架
class ServerRack {
public:
	vector<Machine> host;
	vector<int> double_node;
	vector<int> single_node;
	vector<daily_add> d_add;
	vector<daily_del> d_del;
	unordered_map<string, VmInHost> vm;
	map<string, int> daily_add_host;
	vector<string> daily_add_host_name;
	vector<int> daily_add_host_num;
	vector<daily_add> overload_double;
	vector<daily_add> overload_single;
	vector<string> daily_move_vm;
	vector<array<int, 2>> host_temp;
	vector<array<int, 2>> put_temp;
	vector<daily_add> over_temp;
	long long cost = 0;
	int double_add = 0;
	int single_add = 0;

	double time_1 = 0;
	double time_2 = 0;
	double time_3 = 0;

	int one = 0;
	ServerRack() {}

	void addHost(int consumption, int price, int core, int memory, double c_s, double m_s, int node) {
		Machine m(consumption, price, core, memory, c_s, m_s);
		host.push_back(m);
		if(node) {
			double_node.push_back(host.size() - 1);
		}
		else {
			single_node.push_back(host.size() - 1);
		}
	}

	// 每次处理一天的工单
	daily_output1 processWorkOrder(vector<string>& data, HostList& hostList, VMList& vmList, int day) {
		// cout << "day " << day << endl;
		daily_move_vm.clear();
		double t1 = clock();
		qianyi1(vmList,day);
		time_1 += (clock() - t1) / CLOCKS_PER_SEC;
		overload_double.clear();
		overload_single.clear();
		double_add = 0;
		single_add = 0;
		daily_add_host.clear();
		daily_add_host_name.clear();
		daily_add_host_num.clear();
		host_temp.clear();
		over_temp.clear();
		put_temp.clear();
		d_add.clear();
		d_del.clear();
		int c = 0;
		for (int i = 0; i < data.size(); i++) {
			vector<string> d = spt((data[i]).substr(1, (data[i]).size() - 2), ", ");
			if (d[0] == "add") {
				daily_add a(i, d[2], d[1], c++);
				d_add.push_back(a);
			}
			else {
				daily_del a(i, d[1]);
				d_del.push_back(a);
			}
		}
		vector<string> daily_add_vm(d_add.size());
		vector<array<int, 2>> d_add_sorted = sortAdd(vmList);
		t1 = clock();
		for (int z = 0; z < d_add.size(); z++) {
			int i = d_add_sorted[z][0];
			for (int j = 0; j < d_del.size(); j++) {
				if (vmList.find(d_del[j].name) && d_del[j].num < d_add[i].num)
					fakedelet(d_del[j].name, vmList);
			}
			Choose c = putIn(d_add[i].vmtype, hostList, vmList);
			for (int j = 0; j < d_del.size(); j++) {
				if (vmList.find(d_del[j].name) && d_del[j].num < d_add[i].num)
					undelet(d_del[j].name, vmList);
			}
			if (c.i == -1) {
				if(vmList.get(d_add[i].vmtype).doubleNode) {
					overload_double.push_back(d_add[i]);
				}
				else {
					overload_single.push_back(d_add[i]);
				}
			}
			else {
				add(d_add[i].name, d_add[i].vmtype, hostList, vmList, daily_add_vm, c, d_add[i].count);
			}
		}
		for(int i = 0; i < overload_double.size(); i++) {
			Choose c;
			if (vmList.get(overload_double[i].vmtype).doubleNode) {	
				c = putIn1(overload_double[i].vmtype, hostList, vmList); 
			}	
			if(c.i != -1) {
				add(overload_double[i].name, overload_double[i].vmtype, hostList, vmList, daily_add_vm, c, overload_double[i].count);
				overload_double.erase(overload_double.begin() + i--);
			}
		}
		for(int i = 0; i < overload_single.size(); i++) {
			Choose c;	
			if (!vmList.get(overload_single[i].vmtype).doubleNode) {  
				c = putIn1(overload_single[i].vmtype, hostList, vmList); 
			}
			if(c.i != -1) {
				add(overload_single[i].name, overload_single[i].vmtype, hostList, vmList, daily_add_vm, c, overload_single[i].count);
				overload_single.erase(overload_single.begin() + i--);
			}
		}
		time_2 += (clock() - t1) / CLOCKS_PER_SEC;
		t1 = clock();
		selectHostDouble1(hostList, vmList, day);
		selectHostSingle1(hostList, vmList, day);
		unordered_map<int, int> deal;
		vector<array<int, 2>> deal_real;
		while(true) {
			int dd = 0;
			int flag = 0;
			int i = 0;
			for(; i < host_temp.size(); i++) {
				if(!deal[host_temp[i][0]]) {
					dd = host_temp[i][0];
					flag = 1;
					deal[host_temp[i][0]]++;
					break;
				}
			}
			if(!flag) break;
			Host h = hostList.hostList[dd];
			addHost(h.consumption, h.price, h.core, h.memory, h.core_score, h.memory_score, host_temp[i][1]);
			cost += h.price;
			for(int j = 0; j < over_temp.size(); j++) {
				if(put_temp[j][0] == i) {
					Choose c(host.size() - 1, put_temp[j][1]);
					add(over_temp[j].name, over_temp[j].vmtype, hostList, vmList, daily_add_vm, c, over_temp[j].count);
					put_temp.erase(put_temp.begin() + j);
					over_temp.erase(over_temp.begin() + j);
					j--;
				}
			}
			for(int j = i+1; j < host_temp.size(); j++) {
				if(host_temp[j][0] == dd) {
					deal[host_temp[i][0]]++;
					addHost(h.consumption, h.price, h.core, h.memory, h.core_score, h.memory_score, host_temp[j][1]);
					cost += h.price;
					for(int k = 0; k < over_temp.size(); k++) {
						if(put_temp[k][0] == j) {
							Choose c(host.size() - 1, put_temp[k][1]);
							add(over_temp[k].name, over_temp[k].vmtype, hostList, vmList, daily_add_vm, c, over_temp[k].count);
							put_temp.erase(put_temp.begin() + k);
							over_temp.erase(over_temp.begin() + k--);
						}
					}
				}
				
			}
			deal_real.push_back({host_temp[i][0], deal[host_temp[i][0]]});
		}
		for(auto i : deal_real) {
			daily_add_host_name.push_back(hostList.hostList[i[0]].name);
			daily_add_host_num.push_back(i[1]);
		}
		time_3 += (clock() - t1) / CLOCKS_PER_SEC;
		for (auto i : d_del) {
			delet(i.name, vmList);
		}
		costCounter();
		daily_output1 d(daily_add_vm, daily_add_host_name, daily_add_host_num, daily_move_vm);
		return d;
	}

	// 增加一台虚拟机
	void add(string name, string vmtype, HostList &hostList, VMList &vmList, vector<string>& daily_add_vm, Choose c, int num) {
		VM v = vmList.get(vmtype);
		if (c.node == 2) {
			vm[name] = VmInHost(vmtype, c.i, 1, 1);
			daily_add_vm[num] = "(" + to_string(c.i) + ")";
			host[c.i].A_core -= v.core;
			host[c.i].A_memory -= v.memory;
			host[c.i].B_core -= v.core;
			host[c.i].B_memory -= v.memory;
		}
		else if (c.node == 0) {
			vm[name] = VmInHost(vmtype, c.i, 1, 0);
			daily_add_vm[num] = "(" + to_string(c.i) + ", A)";
			host[c.i].A_core -= v.core;
			host[c.i].A_memory -= v.memory;
		}
		else if (c.node == 1) {
			vm[name] = VmInHost(vmtype, c.i, 0, 1);
			daily_add_vm[num] = "(" + to_string(c.i) + ", B)";
			host[c.i].B_core -= v.core;
			host[c.i].B_memory -= v.memory;
		}
		host[c.i].add(name,vmtype, c.node, v.core + v.memory);
	}

	void fakeAdd(string name, VMList &vmList, Choose c) {
		VM v = vmList.get(vm[name].vmtype);
		if(c.node == 2) {
			host[c.i].A_core -= v.core;
			host[c.i].A_memory -= v.memory;
			host[c.i].B_core -= v.core;
			host[c.i].B_memory -= v.memory;
		}
		else if(c.node == 0) {
			host[c.i].A_core -= v.core;
			host[c.i].A_memory -= v.memory;
		}
		else if(c.node == 1) {
			host[c.i].B_core -= v.core;
			host[c.i].B_memory -= v.memory;
		}
	}

	void unAdd(string name, VMList &vmList, Choose c) {
		VM v = vmList.get(vm[name].vmtype);
		if(c.node == 2) {
			host[c.i].A_core += v.core;
			host[c.i].A_memory += v.memory;
			host[c.i].B_core += v.core;
			host[c.i].B_memory += v.memory;
		}
		else if(c.node == 0) {
			host[c.i].A_core += v.core;
			host[c.i].A_memory += v.memory;
		}
		else if(c.node == 1) {
			host[c.i].B_core += v.core;
			host[c.i].B_memory += v.memory;
		}
	}

	// 删除一台虚拟机
	void delet(string name, VMList& vmList) {
		VM v = vmList.get(vm[name].vmtype);
		int i = vm[name].host_num;
		if (vm[name].nodeA) {
			host[i].A_core += v.core;
			host[i].A_memory += v.memory;
		}
		if (vm[name].nodeB) {
			host[i].B_core += v.core;
			host[i].B_memory += v.memory;
		}
		host[i].move(name);
		vm.erase(name);
	}

	void fakedelet(string name, VMList& vmList) {
		VM v = vmList.get(vm[name].vmtype);
		int i = vm[name].host_num;
		if (vm[name].nodeA) {
			host[i].A_core += v.core;
			host[i].A_memory += v.memory;
		}
		if (vm[name].nodeB) {
			host[i].B_core += v.core;
			host[i].B_memory += v.memory;
		}
	}

	void undelet(string name, VMList& vmList) {
		VM v = vmList.get(vm[name].vmtype);
		int i = vm[name].host_num;
		if (vm[name].nodeA) {
			host[i].A_core -= v.core;
			host[i].A_memory -= v.memory;
		}
		if (vm[name].nodeB) {
			host[i].B_core -= v.core;
			host[i].B_memory -= v.memory;
		}
	}

	void move(string name, string vmtype, VMList &vmList, Choose c ) {
		VM v = vmList.get(vmtype);
		int h = vm[name].host_num;
		if(vm[name].nodeA) {
			host[h].A_core += v.core;
			host[h].A_memory += v.memory;
		}
		if(vm[name].nodeB) {
			host[h].B_core += v.core;
			host[h].B_memory += v.memory;
		}
		if (c.node == 2) {
			vm[name].host_num = c.i;
			daily_move_vm.push_back("(" + name + ", " + to_string(c.i) + ")");
			host[c.i].A_core -= v.core;
			host[c.i].A_memory -= v.memory;
			host[c.i].B_core -= v.core;
			host[c.i].B_memory -= v.memory;
		}
		else if (c.node == 0) {
			vm[name].host_num = c.i;
			vm[name].nodeA = 1;
			vm[name].nodeB = 0;
			daily_move_vm.push_back("(" + name + ", " + to_string(c.i) + ", A)");
			host[c.i].A_core -= v.core;
			host[c.i].A_memory -= v.memory;
		}
		else if (c.node == 1) {
			vm[name].host_num = c.i;
			vm[name].nodeA = 0;
			vm[name].nodeB = 1;
			daily_move_vm.push_back("(" + name + ", " + to_string(c.i) + ", B)");
			host[c.i].B_core -= v.core;
			host[c.i].B_memory -= v.memory;
		}
		host[c.i].add(name,vmtype, c.node, v.core + v.memory);
	}

	// 将虚拟机放到合适的服务器中
	Choose putIn(string vmtype, HostList& hostList, VMList& vmList) {
		VM v = vmList.get(vmtype);
		vector<array<double, 3>> candidate;
		if (v.doubleNode) {
			for (int j = 0; j < double_node.size(); j++) {
				int i = double_node[j];
				if (host[i].A_core >= v.core && host[i].B_core >= v.core &&
					host[i].A_memory >= v.memory && host[i].B_memory >= v.memory &&
					host[i].A_core != host[i].max_core && host[i].A_memory != host[i].max_memory && host[i].B_core != host[i].max_core && host[i].B_memory != host[i].max_memory) {
					candidate.push_back({ i, 0, 2 });
				}
			}
		}
		else {
			for (int j = 0; j < single_node.size(); j++) {
				int i = single_node[j];
				if (host[i].A_core >= v.core && host[i].A_memory >= v.memory &&
					host[i].A_core != host[i].max_core && host[i].A_memory != host[i].max_memory && host[i].B_core != host[i].max_core && host[i].B_memory != host[i].max_memory) {
					candidate.push_back({ i, 0, 0 });
				}
				if (host[i].B_core >= v.core && host[i].B_memory >= v.memory&&
					host[i].A_core != host[i].max_core && host[i].A_memory != host[i].max_memory && host[i].B_core != host[i].max_core && host[i].B_memory != host[i].max_memory) {
					candidate.push_back({ i, 0, 1 });
				}
			}
		}
		if(candidate.size()) {
			return select(candidate,v);
		}
		Choose c(-1, -1);
		return c;
	}

	Choose putIn1(string vmtype, HostList& hostList, VMList& vmList) {
		VM v = vmList.get(vmtype);
		vector<array<double, 3>> candidate;
		if (v.doubleNode) {
			for (int j = 0; j < double_node.size(); j++) {
				int i = double_node[j];
				if (host[i].A_core >= v.core && host[i].B_core >= v.core &&
					host[i].A_memory >= v.memory && host[i].B_memory >= v.memory) {
					candidate.push_back({ i, 0, 2 });
				}
			}
		}
		else {
			for (int j = 0; j < single_node.size(); j++) {
				int i = single_node[j];
				if (host[i].A_core >= v.core && host[i].A_memory >= v.memory ) {
					candidate.push_back({ i, 0, 0 });
				}
				if (host[i].B_core >= v.core && host[i].B_memory >= v.memory) {
					candidate.push_back({ i, 0, 1 });
				}
			}
		}
		if(candidate.size()) {
			return select(candidate,v);
		}
		Choose c(-1, -1);
		return c;
	}

	Choose select(vector<array<double, 3>>& candidate, VM& v) {
		for (int i = 0; i < candidate.size(); i++) {
			Machine h = host[candidate[i][0]];
			if (candidate[i][2] == 2) {
				candidate[i][1] = pow(h.A_core - v.core, 2) + pow(h.B_core - v.core, 2) + 0.8 * pow(h.A_memory- v.memory, 2) + 0.8 * pow(h.B_memory - v.memory, 2);
			}
			else if (candidate[i][2] == 0) {
				candidate[i][1] = pow(h.A_core - v.core, 2) + 0.8 * pow(h.A_memory- v.memory, 2);
			}
			else {
				candidate[i][1] = pow(h.B_core - v.core, 2) + 0.8 *pow(h.B_memory - v.memory, 2);
			}
		}
		int min = INT32_MAX;
		int c;
		array<double, 3> min_num;
		for (auto i : candidate) {
			if (i[1] < min) {
				min = i[1];
				min_num = i;
			}
		}

		Choose selected((int)min_num[0], (int)min_num[2]);
		return selected;
	}

	Choose select(vector<array<double, 3>>& candidate, VM& v, vector<array<int, 8>>& h_temp) {
		for (int i = 0; i < candidate.size(); i++) {
			array<int, 8> h = h_temp[candidate[i][0]];
			if (candidate[i][2] == 2) {
				candidate[i][1] = pow(h[3] - v.core, 2) + pow(h[5] - v.core, 2) + 0.8 * pow(h[4]- v.memory, 2) + 0.8 * pow(h[6] - v.memory, 2);
			}
			else if (candidate[i][2] == 0) {
				candidate[i][1] = pow(h[3] - v.core, 2) + 0.8 * pow(h[4]- v.memory, 2);
			}
			else {
				candidate[i][1] = pow(h[5] - v.core, 2) + 0.8 *pow(h[6] - v.memory, 2);
			}
		}
		int min = INT32_MAX;
		int c;
		array<double, 3> min_num;
		for (auto i : candidate) {
			if (i[1] < min) {
				min = i[1];
				min_num = i;
			}
		}

		Choose selected((int)min_num[0], (int)min_num[2]);
		return selected;
	}

	// 当现有服务器不足时，购买新服务器
	int selectHostDouble1(HostList& hostList, VMList& vmList, int day) {
		if(overload_double.size() <= 50) {
			int cost_best = 100000000;
			int host_best = 0;
			vector<vector<array<int,2>>> p_temp_temp(hostList.hostList.size());
			vector<vector<array<int,2>>> h_temp_temp(hostList.hostList.size());
			vector<array<int, 2>> p_temp;
			vector<array<int, 2>> h_temp;
			vector<array<double, 3>> candidate;
			vector<array<int, 8>> hs;
				double c = 0;
				int flag = 0;
				hs.clear();
				p_temp.clear();
				h_temp.clear();

				int core_total = 0, memory_total = 0, total_num = 0;
				for(int k = 0; k < overload_double.size(); k++) {
					VM v = vmList.get(overload_double[k].vmtype);
					core_total += v.core;
					memory_total += v.memory;
					total_num++;
				}

				for (int k = 0; k < overload_double.size(); k++) {
					candidate.clear();
					VM v = vmList.get(overload_double[k].vmtype);
					for (int j = 0; j < hs.size(); j++) {
						if (hs[j][3] >= v.core && hs[j][5] >= v.core &&
							hs[j][4] >= v.memory && hs[j][6] >= v.memory) {
							candidate.push_back({j,0,2});
						}
					}
					if(candidate.size()) {
						Choose c = select(candidate, v, hs);
						p_temp.push_back({c.i, c.node});
						hs[c.i][3] -= v.core;
						hs[c.i][4] -= v.memory;
						hs[c.i][5] -= v.core;
						hs[c.i][6] -= v.memory;
						core_total -= v.core;
						memory_total -= v.memory;
					}
					else {
						cost_best = 100000000;
						host_best = 0;
						for(int i = 0; i < hostList.hostList.size(); i++) {
							if(v.core <= hostList.hostList[i].core && v.memory <= hostList.hostList[i].memory) {
								c = hostList.hostList[i].core_score * core_total + 0.5 * hostList.hostList[i].memory_score * memory_total;
								if(c < cost_best) {
									cost_best = c;
									host_best = i;
								}
							}
							
						}
						Host h = hostList.hostList[host_best];
						hs.push_back({h.consumption, h.core, h.memory, h.core, h.memory,h.core, h.memory, 0});
						h_temp.push_back({host_best,2});
						k--;
					}
				}
			
			for(int k = 0; k < p_temp_temp[host_best].size(); k++) {
				p_temp_temp[host_best][k][0] += host_temp.size();
			}
			host_temp.insert(host_temp.end(), h_temp.begin(), h_temp.end());
			put_temp.insert(put_temp.end(), p_temp.begin(), p_temp.end());
			over_temp.insert(over_temp.end(), overload_double.begin(), overload_double.end());
		}
		else {
			int cost_best = 100000000;
			int host_best = 0;
			vector<vector<array<int,2>>> p_temp_temp(hostList.hostList.size());
			vector<vector<array<int,2>>> h_temp_temp(hostList.hostList.size());
			vector<array<int, 2>> p_temp;
			vector<array<int, 2>> h_temp;
			vector<array<double, 3>> candidate;
			vector<array<int, 8>> hs;
			int i;
			for (i = 0; i < hostList.hostList.size(); i++) {
				int c = 0;
				int flag = 0;
				int num = 0;
				hs.clear();
				p_temp.clear();
				h_temp.clear();
				Host h = hostList.hostList[i];

				int core_total = 0, memory_total = 0;
				for(int k = 0; k < overload_double.size(); k++) {
					VM v = vmList.get(overload_double[k].vmtype);
					if(v.core > hostList.hostList[i].core || v.memory > hostList.hostList[i].memory ){
						flag = 1;
						break;
					}
					else {
						core_total += v.core;
						memory_total += v.memory;
					}
				}
				if(flag) continue;
				for (int k = 0; k < overload_double.size(); k++) {
					candidate.clear();
					VM v = vmList.get(overload_double[k].vmtype);
					for (int j = 0; j < hs.size(); j++) {
						if (hs[j][3] >= v.core && hs[j][5] >= v.core &&
							hs[j][4] >= v.memory && hs[j][6] >= v.memory) {
							candidate.push_back({j,0,2});
						}
					}
					if(candidate.size()) {
						Choose c = select(candidate, v, hs);
						p_temp.push_back({c.i, c.node});
						hs[c.i][3] -= v.core;
						hs[c.i][4] -= v.memory;
						hs[c.i][5] -= v.core;
						hs[c.i][6] -= v.memory;
					}
					else {
						hs.push_back({h.consumption, h.core, h.memory, h.core, h.memory,h.core, h.memory, 0});
						h_temp.push_back({i,2});
							if(day)
							c+= h.consumption;
							else 
							c+= h.price;
						k--;
					}
				}
				if(c < cost_best) {
					cost_best = c;
					host_best = i;
				}
				p_temp_temp[i] = p_temp;
				h_temp_temp[i] = h_temp;
			}
			for(int k = 0; k < p_temp_temp[host_best].size(); k++) {
				p_temp_temp[host_best][k][0] += host_temp.size();
			}
			host_temp.insert(host_temp.end(), h_temp_temp[host_best].begin(), h_temp_temp[host_best].end());
			put_temp.insert(put_temp.end(), p_temp_temp[host_best].begin(), p_temp_temp[host_best].end());
			over_temp.insert(over_temp.end(), overload_double.begin(), overload_double.end());
		}
	}

	int selectHostSingle1(HostList& hostList, VMList& vmList, int day) {
		if(overload_single.size() <= 50) {
			int cost_best = 100000000;
			int host_best = 0;
			vector<vector<array<int,2>>> p_temp_temp(hostList.hostList.size());
			vector<vector<array<int,2>>> h_temp_temp(hostList.hostList.size());
			vector<array<int, 2>> p_temp;
			vector<array<int, 2>> h_temp;
			vector<array<double, 3>> candidate;
			vector<array<int, 8>> hs;

				double c = 0;
				int flag = 0;
				hs.clear();
				p_temp.clear();
				h_temp.clear();

				int core_total = 0, memory_total = 0;
				for(int k = 0; k < overload_single.size(); k++) {
					VM v = vmList.get(overload_single[k].vmtype);
					core_total += v.core;
					memory_total += v.memory;
				}
				for (int k = 0; k < overload_single.size(); k++) {
					candidate.clear();
					VM v = vmList.get(overload_single[k].vmtype);
					for (int j = 0; j < hs.size(); j++) {
						if (hs[j][3] >= v.core && hs[j][4] >= v.memory ) {
							candidate.push_back({j,0,0});
						}
						else if(hs[j][5] >= v.core && hs[j][6] >= v.memory) {
							candidate.push_back({j,0,1});
						}
					}
					if(candidate.size()) {
						Choose c = select(candidate, v, hs);
						p_temp.push_back({c.i, c.node});
						if(c.node == 0) {
							hs[c.i][3] -= v.core;
							hs[c.i][4] -= v.memory;
							core_total -= v.core;
							memory_total -= v.memory;
						}
						else if(c.node == 1) {
							hs[c.i][5] -= v.core;
							hs[c.i][6] -= v.memory;
							core_total -= v.core;
							memory_total -= v.memory;
						}
					}
					else {
						cost_best = 100000000;
						host_best = 0;
						for(int i = 0; i < hostList.hostList.size(); i++) {
							if(v.core <= hostList.hostList[i].core && v.memory <= hostList.hostList[i].memory) {
								c = hostList.hostList[i].core_score * core_total + 0.5 * hostList.hostList[i].memory_score * memory_total;
								if(c < cost_best) {
									cost_best = c;
									host_best = i;
								}
							}
							
						}
						Host h = hostList.hostList[host_best];
						hs.push_back({h.consumption, h.core, h.memory, h.core, h.memory,h.core, h.memory, 0});
						h_temp.push_back({host_best,0});
						k--;
					}
				}
			for(int k = 0; k < p_temp.size(); k++) {
				p_temp[k][0] += host_temp.size();
			}
			host_temp.insert(host_temp.end(), h_temp.begin(), h_temp.end());
			put_temp.insert(put_temp.end(), p_temp.begin(), p_temp.end());
			over_temp.insert(over_temp.end(), overload_single.begin(), overload_single.end());
		}
		else {
			int cost_best = 100000000;
			int host_best = 0;
			vector<vector<array<int,2>>> p_temp_temp(hostList.hostList.size());
			vector<vector<array<int,2>>> h_temp_temp(hostList.hostList.size());
			vector<array<int, 2>> p_temp;
			vector<array<int, 2>> h_temp;
			vector<array<double, 3>> candidate;
			vector<array<int, 8>> hs;
			int i;
			for (i = 0; i < hostList.hostList.size(); i++) {
				int c = 0;
				int flag = 0;
				int num = 0;
				hs.clear();
				p_temp.clear();
				h_temp.clear();
				Host h = hostList.hostList[i];

				int core_total = 0, memory_total = 0;
				for(int k = 0; k < overload_single.size(); k++) {
					VM v = vmList.get(overload_single[k].vmtype);
					if(v.core > hostList.hostList[i].core || v.memory > hostList.hostList[i].memory ){
						flag = 1;
						break;
					}
					else {
						core_total += v.core;
						memory_total += v.memory;
					}
				}
				if(flag) continue;

				for (int k = 0; k < overload_single.size(); k++) {
					candidate.clear();
					VM v = vmList.get(overload_single[k].vmtype);
					for (int j = 0; j < hs.size(); j++) {
						if (hs[j][3] >= v.core && hs[j][4] >= v.memory ) {
							candidate.push_back({j,0,0});
						}
						else if(hs[j][5] >= v.core && hs[j][6] >= v.memory) {
							candidate.push_back({j,0,1});
						}
					}
					if(candidate.size()) {
						Choose c = select(candidate, v, hs);
						p_temp.push_back({c.i, c.node});
						if(c.node == 0) {
							hs[c.i][3] -= v.core;
							hs[c.i][4] -= v.memory;
						}
						else if(c.node == 1) {
							hs[c.i][5] -= v.core;
							hs[c.i][6] -= v.memory;
						}
					}
					else {
						hs.push_back({h.consumption, h.core, h.memory, h.core, h.memory,h.core, h.memory, 0});
						h_temp.push_back({i,0});
						if(day){
						c+= h.consumption;
						}
						else {
							c += h.price;
						}
						k--;
					}
				}
				if(c < cost_best) {
					cost_best = c;
					host_best = i;
				}
				p_temp_temp[i] = p_temp;
				h_temp_temp[i] = h_temp;
			}
			for(int k = 0; k < p_temp_temp[host_best].size(); k++) {
				p_temp_temp[host_best][k][0] += host_temp.size();
			}
			host_temp.insert(host_temp.end(), h_temp_temp[host_best].begin(), h_temp_temp[host_best].end());
			put_temp.insert(put_temp.end(), p_temp_temp[host_best].begin(), p_temp_temp[host_best].end());
			over_temp.insert(over_temp.end(), overload_single.begin(), overload_single.end());
		}
	}

	vector<array<double, 3>> sortHostSingle() {
		vector<array<double, 3>> R(single_node.size());
		for (int j = 0; j < single_node.size(); j++) {
			int i = single_node[j];
			R[j][0] = i;
			R[j][1] = pow(host[i].A_core, 2) + pow(host[i].A_memory,2) + pow(host[i].B_core, 2) + pow(host[i].B_memory, 2);
			R[j][2] = pow(host[i].max_core - host[i].A_core, 2) + pow(host[i].max_memory - host[i].A_memory,2) + pow(host[i].max_core - host[i].B_core, 2) + pow(host[i].max_memory - host[i].B_memory, 2);
		}
		sort(R.begin(), R.end(), sortFun);
		return R;
	}

	vector<array<double, 3>> sortHostSingle_rest(VMList& vmList) {
		vector<array<double, 3>> R(single_node.size());
		for (int j = 0; j < single_node.size(); j++) {
			int i = single_node[j];
			R[j][0] = i;
			R[j][1] = 0;
			R[j][2] = pow(host[i].A_core, 2) + pow(host[i].A_memory,2) + pow(host[i].B_core, 2) + pow(host[i].B_memory, 2);
		}
		sort(R.begin(), R.end(), sortFun);
		return R;
	}

	vector<array<double, 3>> sortHostDouble() {
		vector<array<double, 3>> R(double_node.size());
		for (int j = 0; j < double_node.size(); j++) {
			int i = double_node[j];
			R[j][0] = i;
			R[j][1] = pow(host[i].A_core, 2) + pow(host[i].A_memory,2) + pow(host[i].B_core, 2) + pow(host[i].B_memory, 2);
			R[j][2] = pow(host[i].max_core - host[i].A_core, 2) + pow(host[i].max_memory - host[i].A_memory,2) + pow(host[i].max_core - host[i].B_core, 2) + pow(host[i].max_memory - host[i].B_memory, 2);
		}
		sort(R.begin(), R.end(), sortFun);
		return R;
	}

	vector<array<double, 3>> sortHostDouble_rest(VMList& vmList) {
		vector<array<double, 3>> R(double_node.size());
		for (int j = 0; j < double_node.size(); j++) {
			int i = double_node[j];
			R[j][0] = i;
			R[j][1] = 2;
			R[j][2] = pow(host[i].A_core, 2) + pow(host[i].A_memory,2) + pow(host[i].B_core, 2) + pow(host[i].B_memory, 2);
		}
		sort(R.begin(), R.end(), sortFun);
		return R;
	}

	vector<array<int, 2>> sortAdd(VMList& vmList) {
		vector<array<int, 2>> s_add(d_add.size());
		for(int i = 0; i < d_add.size(); i++) {
			s_add[i][0] = i;
			s_add[i][1] = vmList.get(d_add[i].vmtype).core + vmList.get(d_add[i].vmtype).memory;
		}
		sort(s_add.begin(), s_add.end(), sortFun1);
		return s_add;
	}
	
	void qianyi1(VMList &vmList, int day) {
		int qian_yi_yu_liang;
		double ha;
		if(!day && !one) {
			qian_yi_yu_liang = vm.size();
			ha=0.66;
			
		}
		
		else{
			qian_yi_yu_liang= vm.size() * 0.03;
			ha = 0.02;
		}
		
		vector<array<double, 3>> sorted_host = sortHostDouble();
		vector<array<double, 3>> sorted_host_rest = sortHostDouble_rest(vmList);
		vector<Choose> tar;
		vector<string> mo_list;
		vector<int> mo_list_n;
		vector<array<double, 3>> candidate;
		Choose c;
		for(int i = 0; i < sorted_host.size() && qian_yi_yu_liang>vm.size() * ha; i++) {
			int flag = 0;
			tar.clear();
			mo_list.clear();
			mo_list_n.clear();
			candidate.clear();
			vector<pair<string,Instance>> vmv(host[sorted_host[i][0]].instance_list.begin(), host[sorted_host[i][0]].instance_list.end());
			sort(vmv.begin(), vmv.end(), sortVM);
			if(qian_yi_yu_liang  > host[sorted_host[i][0]].instance_list.size()) {
				for(auto j = vmv.begin(); j!= vmv.end(); j++) {
					VM v = vmList.get(j->second.type);
					candidate.clear();
					for(int k = 0; sorted_host_rest[k][2] < sorted_host[i][1] && k < sorted_host_rest.size(); k++) {
						flag = 0;
						if(host[sorted_host_rest[k][0]].A_core >= v.core &&
							host[sorted_host_rest[k][0]].B_core >= v.core &&
							host[sorted_host_rest[k][0]].A_memory >= v.memory &&
							host[sorted_host_rest[k][0]].B_memory >= v.memory) {
								candidate.push_back({sorted_host_rest[k][0], 0, 2});
								flag = 1;
								if(candidate.size() >= 10) break;
							}
					}
					c = select(candidate, v);
					if(flag) {
						move(j->second.name, vm[j->second.name].vmtype, vmList, c);
						qian_yi_yu_liang--;
						mo_list.push_back(j->first);
						mo_list_n.push_back(sorted_host[i][0]);
					}
					
				}
				for(int j = 0; j < mo_list.size(); j++) {
					host[mo_list_n[j]].move(mo_list[j]);
				}
			}
		}
		sorted_host = sortHostSingle();
		sorted_host_rest = sortHostSingle_rest(vmList);
		for(int i = 0; i < sorted_host.size()&&qian_yi_yu_liang > 0; i++) {
			int flag = 0;
			tar.clear();
			mo_list.clear();
			mo_list_n.clear();
			candidate.clear();
			vector<pair<string,Instance>> vmv(host[sorted_host[i][0]].instance_list.begin(), host[sorted_host[i][0]].instance_list.end());
			sort(vmv.begin(), vmv.end(), sortVM);
			if(qian_yi_yu_liang > host[sorted_host[i][0]].instance_list.size()) {
				for(auto j=vmv.begin(); j!=vmv.end(); j++) {
					VM v = vmList.get(j->second.type);
					candidate.clear();
					for(int k = 0; sorted_host_rest[k][2] < sorted_host[i][1] && k < sorted_host_rest.size(); k++) {
						flag = 0;
						if(	host[sorted_host_rest[k][0]].A_core >= v.core &&
							host[sorted_host_rest[k][0]].A_memory >= v.memory) {
								candidate.push_back({sorted_host_rest[k][0], 0, 0});
								flag = 1;
								if(candidate.size() >= 10) break;
							}
						else if(host[sorted_host_rest[k][0]].B_core >= v.core &&
							host[sorted_host_rest[k][0]].B_memory >= v.memory) {
								candidate.push_back({sorted_host_rest[k][0], 0, 1});
								flag = 1;
								if(candidate.size() >= 10) break;
							}	
					}
					c = select(candidate, v);
					if(flag) {
						move(j->second.name, vm[j->second.name].vmtype, vmList, c);
						qian_yi_yu_liang--;
						mo_list.push_back(j->first);
						mo_list_n.push_back(sorted_host[i][0]);
					}
				}
				for(int j = 0; j < mo_list.size(); j++) {
					host[mo_list_n[j]].move(mo_list[j]);
				}
			}
		}
		if(!day&&!one) {
			if(qian_yi_yu_liang < ceil(0.97 * vm.size())) {
				one = 1;
			}
		}
	}

	// 成本计算，提交时不需要
	void costCounter() {
		for (auto i : host) {
            if (i.A_core != i.max_core || i.A_memory != i.max_memory || i.B_core != i.max_core || i.B_memory != i.max_memory) {
                cost += i.consumption;
            }
		}
	}
};

// 整理输出
void output(daily_output1 output, vector<string>& res) {
	res.clear();
	res.push_back("(purchase, " + to_string(output.daily_add_host_name.size()) + ")");
	for(int i = 0 ; i < output.daily_add_host_name.size(); i++) {
		res.push_back("(" + output.daily_add_host_name[i] + ", " + to_string(output.daily_add_host_num[i]) + ")");
	}
	res.push_back("(migration, " + to_string(output.daily_move_vm.size()) + ")");
	for(auto i:output.daily_move_vm) {
		res.push_back(i);
	}
	for (auto s : output.daily_add_vm) {
		res.push_back(s);
	}
}

int main()
{
	vector<string> res;
	vector<string> host_input, vm_input;
	vector<vector<string>> workorder_input;
	// 创建关键对象
	HostList hostList;
	VMList vmList;
	ServerRack serverRack;

	string s;
	int v = 0, h = 0, d = 0, k = 0;
	string mode = "test";
	if (mode == "train") {
		ifstream in("training-1.txt");
		getline(in, s);
		h = atoi(s.c_str());
		for(int i = 0; i < h; i++) {
			getline(in,s);
			host_input.push_back(s);
		}
		getline(in,s);
		v = atoi(s.c_str());
		for(int i = 0; i < v; i++) {
			getline(in,s);
			vm_input.push_back(s);
		}
		hostList.build(host_input);
		vmList.build(vm_input);
		hostList.getFittest1(vmList);
		ofstream out("res.txt");

		getline(in, s);
		d = atoi(spt(s, " ")[0].c_str());
		k = atoi(spt(s, " ")[1].c_str());
		for(int i = 0; i < k; i++) {
			getline(in, s);
			int l = atoi(s.c_str());
			vector<string> d_order;
			for(int j = 0; j < l; j++) {
				getline(in, s);
				d_order.push_back(s);
			}
			workorder_input.push_back(d_order);
		}
		double start = clock();
		for(int i = 0; i < d; i++) {
			output(serverRack.processWorkOrder(workorder_input[i], hostList, vmList, i < d / 2), res);
			// 输出结果
			for (auto j : res)
				out << j << endl;
			if(i + k < d) {
				getline(in, s);
				int l = atoi(s.c_str());
				vector<string> d_order;
				for(int j = 0; j < l; j++) {
					getline(in, s);
					d_order.push_back(s);
				}
				workorder_input.push_back(d_order);
			}
		}
		double end = clock();
		cout << serverRack.cost << endl; // 输出成本
		cout << serverRack.host.size() << endl; // 输出服务器数量
		cout << (end - start) / CLOCKS_PER_SEC << 's' << endl;
		cout << "qianyi " << serverRack.time_1 << " put " << serverRack.time_2 << " buy " << serverRack.time_3 << endl;
		fflush(stdout);

	}
	else {
		h = getInt();
		for (int i = 0; i < h; i++) {
			getline(cin, s);
			host_input.push_back(s);
		}
		v = getInt();
		for (int i = 0; i < v; i++) {
			getline(cin, s);
			vm_input.push_back(s);
		}
		// 创建服务器和虚拟机列表
		hostList.build(host_input);
		vmList.build(vm_input);
		hostList.getFittest1(vmList);
		getline(cin, s);
		d = atoi(spt(s, " ")[0].c_str());
		k = atoi(spt(s, " ")[1].c_str());
		for(int i = 0; i < k; i++) {
			getline(cin, s);
			int l = atoi(s.c_str());
			vector<string> d_order;
			for(int j = 0; j < l; j++) {
				getline(cin, s);
				d_order.push_back(s);
			}
			workorder_input.push_back(d_order);
		}
		for (int i = 0; i < d; i++) {
			output(serverRack.processWorkOrder(workorder_input[i], hostList, vmList, i < d / 2), res);
			// 输出结果
			for (auto j : res)
				cout << j << endl;
			fflush(stdout);
			if(i + k < d) {
				getline(cin, s);
				int l = atoi(s.c_str());
				vector<string> d_order;
				for(int j = 0; j < l; j++) {
					getline(cin, s);
					d_order.push_back(s);
				}
				workorder_input.push_back(d_order);
			}
		}
		
	}
	return 0;
}
