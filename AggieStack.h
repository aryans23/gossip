#ifndef AGGIESTACK_H
#define AGGIESTACK_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>

using namespace std;

class Machine;
class Image;
class Flavor;
class Rack;
class Instance;

inline string get_current_date_time(string s)
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    if(s == "now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if( s== "date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return string(buf);
}

inline void logger(string log_msg)
{
    string filePath = "aggiestack-log-"+get_current_date_time("date")+".txt";
    string now = get_current_date_time("now");
    ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << log_msg << '\n';
    ofs.close();
}

inline vector<string> split(string s, string separator)
{
	int prev_pos = 0, idx;
	vector<string> words;
	words.clear();
	while ((idx = s.find_first_of(separator, prev_pos)) != string::npos) {
		if (idx > prev_pos) {
			words.push_back(s.substr(prev_pos, idx-prev_pos));
		}
		prev_pos = idx+1;
	}
	if (prev_pos < s.length()) {
		words.push_back(s.substr(prev_pos, string::npos));
	}
	return words;
}

class Machine
{
public:
	Machine();
	Machine(string name, string rack, string ip, int memory, int num_disks, int num_vcpus);
	~Machine();

	void set_name(string name);
	void set_ip(string ip);
	void set_memory(int memory);
	void set_num_disks(int num_disks);
	void set_num_vcpus(int num_vcpus);
	void set_rack(string name);
	
	string get_name();
	string get_ip();
	int get_memory();
	int get_num_disks();
	int get_num_vcpus();
	string get_rack();
	unordered_map<string, Instance*>& get_running_instances();

	void update_machine(Instance* ins, bool inc);
	bool add_instance(Instance* ins);
	bool delete_instance(string ins);
	bool idle();		

private:
	string _name;
	string _ip;
	int _memory;
	int _num_disks;
	int _num_vcpus;
	string _rack;		// on which it is running
	unordered_map<string, Instance*> _instances_running;
};

class Image
{
public:
	Image();
	Image(string name, int size, string path);
	~Image();
	
	void set_name(string name);
	void set_size(int size);
	void set_path(string path);
	
	string get_name();
	int get_size();
	string get_path();

private:
	string _name;
	int _size;
	string _path;
};

class Flavor
{
public:
	Flavor();
	Flavor(string type, int ram, int num_disks, int num_vcpus);
	~Flavor();

	string get_type();
	int get_num_disks();
	int get_ram();
	int get_num_vcpus();
	
	void set_type(string type);
	void set_num_disks(int num_disks);
	void set_ram(int ram);
	void set_num_vcpus(int num_vcpus);

private:
	string _type;
	int _ram;
	int _num_disks;
	int _num_vcpus;
};

class Rack
{
public:
	Rack();
	Rack(string name, int storage);
	~Rack();

	void set_name(string name);
	void set_storage(int storage);

	string get_name();
	int get_available_storage();
	int get_max_storage();
	std::vector<string>& get_cached_images();
	std::set<string>& get_machines();

	bool update_storage(int val, bool inc);
	bool update_cache(string& img, int size);

private:
	string _name;
	int _available_storage;
	int _max_storage;
	std::vector<string> _cached_images;
	std::set<string> _machines;
};

class Instance
{
public:
	Instance();
	Instance(string name, string image, string flavor);
	~Instance();

	void set_name(string name);
	void set_image(string img);
	void set_flavor(string fla);

	string get_name();
	string get_image();
	string get_flavor();

private:
	string _name;
	string _image;
	string _flavor;
};

class AggieStack
{
private:
	static AggieStack *aggie;
	AggieStack();

public:
	~AggieStack();

	// make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
	AggieStack(AggieStack const&) = delete;
    void operator=(AggieStack const&) = delete;

	static AggieStack* get();

	void split(std::vector<string>& tokens, string& string_to_split, string del);
	vector<std::vector<string>> tokenize_file(string filename);
	
	void set_number_machines(int n);
	void set_number_images(int n);
	void set_number_flavors(int n);
	void set_machines_list(std::vector<Machine>& machines);
	void set_available_machines(std::vector<Machine>& machines);
	void set_machine_on_racks(std::vector<Machine>& machines);
	void set_images(std::unordered_map<string, Image>& img_map);
	void set_flavors(std::unordered_map<string,Flavor>& flavor_map);

	int 								 	get_number_machines();
	int 								 	get_number_images();
	int 									get_number_flavors();
	std::vector<Machine>& 				 	get_machines_list();
	std::unordered_map<string, Machine>& 	get_available_machines();
	std::unordered_map<string, Image>& 		get_images();
	std::unordered_map<string, Flavor>& 	get_flavors();
	std::unordered_map<string, Rack>&		get_racks();

	void read_machine_config(string filename);
	void read_image_config(string filename);
	void read_flavor_config(string filename);
	
	void show_machines();
	void show_available_machines();
	void show_images();
	void show_flavors();
	void show_racks();
	void show_instances(bool is_mach);
	void show_rack_status(string& rack_name);		
	void show_all();
	
	bool can_host(string machine_name, string flavor);
	bool create_instance(string instance_name, string image_name, string flavor_name);
	bool delete_instance(string instance_name);
	bool evacuate_rack(string rack_name);
	bool remove_machine(string machine_name);
	bool add_machine(string name, int memory, int disks, int vcpus, string ip, string rack);
	
private:
	int _num_machines;
	int _num_images;
	int _num_flavors;
	std::vector<Machine> _machines;
	std::unordered_map<string, Machine> _available_machines;
	std::unordered_map<string, Image> _images;
	std::unordered_map<string, Flavor> _flavors;
	std::unordered_map<string, Rack> _racks;
};

#endif
