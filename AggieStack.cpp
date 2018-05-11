#include "AggieStack.h"

using namespace std;

// ************************************ AggieStack *********************************** // 

AggieStack* AggieStack::aggie = 0;

AggieStack::AggieStack() {}

AggieStack::~AggieStack() 
{
	delete aggie;
	aggie = 0;
}

AggieStack* AggieStack::get()
{
    if (aggie == 0) {
    	logger("AggieStack singleton initialised: SUCCESS");
        aggie = new AggieStack();
    }
    return aggie;
}

void AggieStack::split(std::vector<string>& tokens, string& string_to_split, string del) 
{
	size_t pos = 0, found;
	while((found = string_to_split.find_first_of(del, pos)) != string::npos) {
	    tokens.push_back(string_to_split.substr(pos, found - pos));
	    pos = found + 1;
	}
	tokens.push_back(string_to_split.substr(pos));
}

vector<std::vector<string>> AggieStack::tokenize_file(string filename)
{
	ifstream in;
	in.open(filename);
	if (!in) {
		string log = "Unable to open the file " + filename;
		logger(log);
		cerr << "Unable to open the file " << filename << endl;
		exit(1);
	}

	vector<string> lines;
	string x;
	while (getline(in,x))
		lines.push_back(x);

	vector<std::vector<string>> tokens;
	for (auto& l: lines) {
		std::vector<string> t;
		split(t, l, " ");
		tokens.push_back(t);
	}

	in.close();

	return tokens;
}

void AggieStack::set_number_machines(int n)
{
	this->_num_machines = n;
	string log = "set_number_machines() to " + to_string(n) +": SUCCESS";
	logger(log);
}

void AggieStack::set_number_images(int n)
{
	this->_num_images = n;
	string log = "set_number_images() to " + to_string(n) +": SUCCESS";
	logger(log);
}

void AggieStack::set_number_flavors(int n)
{
	this->_num_flavors = n;
	string log = "set_number_flavors() to " + to_string(n) +": SUCCESS";
	logger(log);
}

void AggieStack::set_machines_list(std::vector<Machine>& machines)
{
	this->_machines = machines;
	string log = "set_machines_list(): SUCCESS";
	logger(log);
}

void AggieStack::set_available_machines(std::vector<Machine>& machines)
{
	for (auto& h: machines)
		this->_available_machines[h.get_name()] = h; 
	string log = "set_available_machines(): SUCCESS";
	logger(log);
}

void AggieStack::set_machine_on_racks(std::vector<Machine>& machines)
{
	std::unordered_map<string, Rack>& racks = this->_racks;
	for (auto& mach: machines) {
		if (racks.find(mach.get_rack()) == racks.end()) {	// rack not found
			cerr << "Rack not found for this machine" << endl;
			string log = "set_machine_on_racks(): FAILURE";
			logger(log);
			return;
		} else {
			racks[mach.get_rack()].get_machines().insert(mach.get_name());	
		}
	}
	string log = "set_machine_on_racks(): SUCCESS";
	logger(log);
}

void AggieStack::set_images(std::unordered_map<string, Image>& img_map)
{
	this->_images = img_map;
	string log = "set_images(): SUCCESS";
	logger(log);
}

void AggieStack::set_flavors(std::unordered_map<string, Flavor>& flavor_map)
{
	this->_flavors = flavor_map;
	string log = "set_flavors(): SUCCESS";
	logger(log);
}

int AggieStack::get_number_machines() 
{
	return this->_num_machines;
}

int AggieStack::get_number_images()
{
	return this->_num_images;
}

int AggieStack::get_number_flavors()
{
	return this->_num_flavors;
}

std::vector<Machine>& AggieStack::get_machines_list()
{
	return this->_machines;
}

std::unordered_map<string, Machine>& AggieStack::get_available_machines()
{
	return this->_available_machines;
}

std::unordered_map<string, Image>& AggieStack::get_images()
{
	return this->_images;
}

std::unordered_map<string,Flavor>& AggieStack::get_flavors()
{
	return this->_flavors;
}

std::unordered_map<string,Rack>& AggieStack::get_racks()
{
	return this->_racks;
}

void AggieStack::read_machine_config(string filename) 
{
	vector<std::vector<string>> tokens = tokenize_file(filename);

	int num_racks = stoi(tokens[0][0]);
	for (int i = 1; i <= num_racks; i++) {
		Rack r(tokens[i][0], stoi(tokens[i][1]));		// Racks created
		this->_racks[tokens[i][0]] = r;
	}

	std::vector<Machine> machines;
	for (int i = num_racks+2; i < tokens.size(); i++) {
		Machine mach(tokens[i][0], tokens[i][1], tokens[i][2], stoi(tokens[i][3]), stoi(tokens[i][4]), stoi(tokens[i][5]));
		machines.push_back(mach);
	}
	this->set_machines_list(machines);
	this->set_available_machines(machines);	// init
	this->set_machine_on_racks(machines);
	string log = filename + " read successfully: SUCCESS";
	cout << log << endl;
	logger("read_machine_config(): "+log);
}

void AggieStack::read_image_config(string filename) 
{
	vector<std::vector<string>> tokens = tokenize_file(filename);

	this->set_number_images(stoi(tokens[0][0]));
	std::unordered_map<string, Image> img_map;
	for (int i = 1; i < tokens.size(); i++) {
		Image img(tokens[i][0], stoi(tokens[i][1]), tokens[i][2]);
		img_map[tokens[i][0]] = img;
	}
	this->set_images(img_map);	
	string log = filename + " read successfully: SUCCESS";
	cout << log << endl;
	logger("read_image_config(): "+log);
}

void AggieStack::read_flavor_config(string filename) 
{
	vector<std::vector<string>> tokens = tokenize_file(filename);

	this->set_number_flavors(stoi(tokens[0][0]));
	std::unordered_map<string,Flavor> flavor_map;
	for (int i = 1; i < tokens.size(); i++) {
		Flavor fla(tokens[i][0], stoi(tokens[i][1]), stoi(tokens[i][2]), stoi(tokens[i][3]));
		flavor_map[tokens[i][0]] = fla;
	}
	this->set_flavors(flavor_map);
	string log = filename + " read successfully: SUCCESS";
	cout << log << endl;
	logger("read_flavor_config(): "+log);
}

void AggieStack::show_machines()
{
	cout << "Showing All Machines..." << endl;
	int i = 1;
	for (auto& h: this->get_machines_list()) {
		cout << "#" << i << "\t";
		cout << "Name: " << h.get_name() << "\t";
		cout << "Rack: " << h.get_rack() << "\t";
		cout << "IP address: " << h.get_ip() << "\t";
		cout << "Memory: " << h.get_memory() << "\t";
		cout << "Number of Disks: " << h.get_num_disks() << "\t";
		cout << "Number of vCPUs: " << h.get_num_vcpus() << "\t";
		cout << endl;
		i++;
	}
	cout << endl;
	logger("show_machines(): SUCCESS");
}

void AggieStack::show_available_machines()
{
	cout << "Showing Available Machines..." << endl;
	int i = 1;
	for (auto& m: this->get_available_machines()) {
		auto h = m.second;
		cout << "#" << i << "\t";
		cout << "Name: " << h.get_name() << "\t";
		cout << "Rack: " << h.get_rack() << "\t";
		cout << "IP address: " << h.get_ip() << "\t";
		cout << "Memory: " << h.get_memory() << "\t";
		cout << "Number of Disks: " << h.get_num_disks() << "\t";
		cout << "Number of vCPUs: " << h.get_num_vcpus() << "\t";
		cout << endl;
		i++;
	}
	cout << endl;
	logger("show_available_machines(): SUCCESS");
}

void AggieStack::show_images()
{
	cout << "Showing Images..." << endl;
	int i = 1;
	for (auto& img: this->get_images()) {
		auto image = img.second;
		cout << "#" << i << "\t";
		cout << "Name: " << image.get_name() << "\t";
		cout << "Size: " << image.get_size() << "\t";
		cout << "Path: " << image.get_path() << "\t";
		cout << endl;
		i++;
	}
	cout << endl;
	logger("show_images(): SUCCESS");
}

void AggieStack::show_flavors()
{
	cout << "Showing Flavors..." << endl;
	int i = 1;
	for (auto& fla: this->get_flavors()) {
		auto f = fla.second;
		cout << "#" << i << "\t";
		cout << "Type: " << f.get_type() << "\t";
		cout << "RAM: " << f.get_ram() << "\t";
		cout << "Disks: " << f.get_num_disks() << "\t";
		cout << "vCPUs: " << f.get_num_vcpus() << "\t";
		cout << endl;
		i++;
	}
	cout << endl;
	logger("show_flavors(): SUCCESS");
}

void AggieStack::show_racks()
{
	cout << "Showing Racks..." << endl;
	int i = 1;
	for (auto& rack: this->get_racks()) {
		auto r = rack.second;
		cout << "#" << i << "\t";
		cout << "Type: " << r.get_name() << ", ";
		cout << "Storage: " << r.get_available_storage() << ", ";
		cout << "Machines: ";
		for (auto& m: r.get_machines()) {
			cout << m << " ";
		}
		cout << endl;
		i++;
	}
	cout << endl;
	logger("show_flavors(): SUCCESS");
}

void AggieStack::show_instances(bool is_mach = false)
{
	std::unordered_map<string, Machine> machines = this->get_available_machines();
	int i = 0;
	for (auto& mach: machines) {
		auto& m = mach.second;
		unordered_map<string, Instance*>& instances = m.get_running_instances();
		for (auto& instance : instances) {
			auto& ins = instance.second;
			cout << "#" << i << "\t";
			cout << "Name: " << ins->get_name() << endl;
			if (!is_mach) {
				Image img = this->get_images()[ins->get_image()];
				cout << " \tImage Name: " << img.get_name() << endl;
				cout << " \tImage Size: " << to_string(img.get_size()) << endl;
				cout << " \tImage Path: " << img.get_path() << endl;
				cout << " \tFlavor: " << ins->get_flavor();
			} else {
				cout << " \tMachine Name: " << mach.first;
			}
			cout << endl << endl;
			i++;
		}
	}
	logger("show_instances(): SUCCESS");
}

void AggieStack::show_rack_status(string& rack_name)
{
	if (rack_name.size() == 0) return;
	int i = 0;
	auto& rack = this->get_racks()[rack_name];
	cout << "#" << i << "\t";
	cout << "Name: " << rack_name << endl;
	cout << " \tStorage left: " << rack.get_available_storage() << endl;
	cout << " \tImage Name: ";
	for (auto& img: rack.get_cached_images()) {
		cout << img << " ";
	}
	cout << endl << endl;
	logger("show_rack_status(): SUCCESS");
}

void AggieStack::show_all()
{
	this->show_machines();
	this->show_images();
	this->show_flavors();
	logger("show_all(): SUCCESS");
} 

bool AggieStack::can_host(string machine_name, string flavor)
{
	unordered_map<string,Machine> hdws = this->get_available_machines();
	unordered_map<string,Flavor> flvs = this->get_flavors();

	if (hdws.find(machine_name) == hdws.end()) {
		cerr << "Machine name not found" << endl;
		logger("can_host() [Machine name not found]: FAILURE");
		return false;
	}
	if (flvs.find(flavor) == flvs.end()) {
		cerr << "Flavor " << flavor << " not supported in machine " << machine_name << endl;
		logger("can_host() [Flavor not supported in machine]: FAILURE");
		return false;	
	}

	Machine mach = hdws[machine_name];
	Flavor fl = flvs[flavor];

	if (mach.get_memory() >= fl.get_ram() &&
		mach.get_num_disks() >= fl.get_num_disks() &&
		mach.get_num_vcpus() >= fl.get_num_vcpus()
		) {
		string log = "can_host() for machine " + machine_name + " : SUCCESS";
		logger(log);
		return true;
	} else {
		string log = "can_host() for machine " + machine_name + " : FAILURE";
		logger(log);
		return false;
	}
}

bool AggieStack::create_instance(string instance_name, string image_name, string flavor_name)
{
	string log = "";
	Image image = (this->get_images())[image_name];
	Instance* instance = new Instance(instance_name,image_name,flavor_name); 
	for (auto& r: this->get_racks()) {	// present in cache
		Rack& rack = r.second;
		if (image.get_size() > rack.get_max_storage()) 
			continue;
		std::vector<string>& cache = rack.get_cached_images();
		if (std::find(cache.begin(), cache.end(), image_name) != cache.end()) {	
			log = "Cache hit in rack " + rack.get_name();
			logger(log);
			cout << "Cache hit in rack " << rack.get_name() << endl;
			for (string machine_name: rack.get_machines()) {	// 	if (machine can host the flavor)
				Machine& mach = this->get_available_machines()[machine_name];
				if (mach.add_instance(instance)) {	// Instance added successfully
					log = "Instance " + instance_name + " added successfully: SUCCESS";
					logger(log);
					return true;
				}
			}
		}
	}
	for (auto& r: this->get_racks()) {
		Rack& rack = r.second;
		cout << "Rack: " << rack.get_name() << endl;
		if (image.get_size() > rack.get_max_storage())
			continue;
		if (rack.get_available_storage() >= image.get_size()) {				// if (rack can cache this image) 
			for (string machine_name: rack.get_machines()) {
				Machine& mach = this->get_available_machines()[machine_name];
				if (mach.add_instance(instance)) {							// Instance added successfully
					std::vector<string>& cache = rack.get_cached_images();
					rack.update_cache(image_name, image.get_size());		// cache image in the rack
					rack.update_storage(image.get_size(), false);			// account for image in cache
					log = "Instance " + instance_name + " added successfully on new rack: SUCCESS";
					logger(log);
					return true;
				}
			}
		}
	}
	log = "Instance creation failed: FAILURE";
	logger(log);
	return false;
}

bool AggieStack::delete_instance(string instance_name)
{
	string log = "";
	for (auto& r: this->get_racks()) {	
		Rack& rack = r.second;
		for (string machine_name: rack.get_machines()) {
			auto& mach = this->get_available_machines()[machine_name];
			auto instances = mach.get_running_instances();
			if (instances.find(instance_name) == instances.end()) {
				continue;
			} else {
				mach.delete_instance(instance_name);
				log = "Instance " + instance_name + " deleted: SUCCESS";
				logger(log);
				return true;
			}
		}
	}
	log = "Instance " + instance_name + " deletion: FAILED";
	logger(log);
	return false;
}

bool AggieStack::remove_machine(string machine_name) 
{
	string log = "";
	auto& machines = this->get_available_machines();
	if (machines.find(machine_name) == machines.end()) {
		log = "Machine " + machine_name + " to be deleted not found: FAILURE";
		logger(log);
		cerr << "Machine " << machine_name << " to be deleted not found" << endl;
		return false;
	}
	
	Machine& mach = machines[machine_name];
	string rack_name = mach.get_rack();

	if (!mach.idle()) {		// won't go in when evac mode  
		log = "Machine " + machine_name + " to be deleted has running instances: FAILURE";
		logger(log);
		cerr << "Machine " <<  machine_name << " to be deleted has running instances" << endl;
		return false;	
	}

	// delete from _machines
	std::vector<Machine>& v = this->get_machines_list();
	std::vector<Machine>::iterator it;
	for (it = v.begin(); it != v.end(); it++) {
		if (it->get_name() == machine_name) {
			break;
		}
	}
	v.erase(it);

	// delete from _available_machines
	machines.erase(machine_name);

	// delete from _racks
	Rack& rack = this->get_racks()[rack_name];
	rack.get_machines().erase(machine_name);
	
	log = "Machine " + machine_name + " deleted successfully: SUCCESS";
	logger(log);

	return true;
}

bool AggieStack::evacuate_rack(string rack_name)
{
	string log = "";
	auto& rack_map = this->get_racks();
	if (rack_map.find(rack_name) == rack_map.end()) {
		log = "Rack " + rack_name + " (to be evacuated) not found: FAILURE";
		logger(log);
		cerr << "Rack Name to be evacuated not found" << endl;
		return false;
	}
	auto& rack = rack_map[rack_name];
	std::vector<string> machine_tobe_deleted;
	std::vector<string> evacuated_instances;

	auto& machines = rack.get_machines();
	for (auto& m: machines) {
		auto& instances = this->get_available_machines()[m].get_running_instances();
		for (auto& i: instances) {				// deleting all the instances
			auto& ins = i.second;
			evacuated_instances.push_back(ins->get_name()+" "+ins->get_image()+" "+ins->get_flavor());
		}
		machine_tobe_deleted.push_back(m);
	}

	for (int i = 0; i < evacuated_instances.size(); i++) {
		auto& e_ins = evacuated_instances[i];
		std::vector<string> tmp = ::split(e_ins," ");

		// deleting the marked instances
		if (this->delete_instance(tmp[0])) {
			log = "Instance " + tmp[0] + " evacuation was successfull: SUCCESS";
			logger(log);
			cout << "Instance " << tmp[0] << " evacuation was successfull" << endl;
		} else {
			log = "Instance " + tmp[0] + " evacuation failed: FAILURE";
			logger(log);
			cout << "Instance " << tmp[0] << " evacuation failed" << endl;
		}
	}

	for (auto& m: machine_tobe_deleted) {
		// removing the machine from the rack
		if (this->remove_machine(m)) {
			log = "Machine " + m + " removed successfully: SUCCESS";
			logger(log);
			cout << "Machine " << m << " removed successfully" << endl;
		} else {
			log = "Machine " + m + " removal falied: FAILURE";
			logger(log);
			cout << "Machine " << m << " removal failed" << endl;
		}
	}

	// delete rack from the central map
	rack_map.erase(rack_name);

	for (int i = 0; i < evacuated_instances.size(); i++) {
		auto& e_ins = evacuated_instances[i];
		std::vector<string> tmp = ::split(e_ins," ");

		// creating the instances in another rack
		if (!this->create_instance(tmp[0],tmp[1],tmp[2])) {
			log = "Some instances were killed due to insufficient infrastructure: FAILURE";
			logger(log);
			cerr << "The following instances were killed due to insufficient infrastructure" << endl;
			for ( ; i < evacuated_instances.size(); i++) {
				tmp = ::split(e_ins," ");
				cout << "\t" << tmp[0] << ", " << tmp[1] << ", " << tmp[2] << endl;
			}
			return false;
		}
	}

	log = "Rack " + rack_name + " evacuated successfully: SUCCESS";
	logger(log);
	return true;
}

bool AggieStack::add_machine(string name, int memory, int disks, int vcpus, string ip, string rack)
{
	Machine mach(name, rack, ip, memory, disks, vcpus);
	this->get_machines_list().push_back(mach);
	this->get_available_machines()[name] = mach;
	this->get_racks()[rack].get_machines().insert(name);	
	string log = "add_machine() for machine " + name + " successfull: SUCCESS";
	logger(log);
	return true;
}

// ********************************** Machine *********************************** // 

Machine::Machine()
{
}

Machine::Machine(string name, string rack, string ip, int memory, int num_disks, int num_vcpus):
	_name(name), _rack(rack), _ip(ip), _memory(memory), _num_disks(num_disks), _num_vcpus(num_vcpus) 
{
}

Machine::~Machine() {}

void Machine::set_name(string name)
{
	this->_name = name;
}

void Machine::set_ip(string ip) 
{
	this->_ip = ip;
}
	
void Machine::set_memory(int memory) 
{
	this->_memory = memory;
}

void Machine::set_num_disks(int num_disks) 
{
	this->_num_disks = num_disks;
}

void Machine::set_num_vcpus(int num_vcpus) 
{
	this->_num_vcpus = num_vcpus;
}

void Machine::set_rack(string rack) 
{
	this->_rack = rack;
}

string Machine::get_name() 
{
	return this->_name;
}
	
string Machine::get_ip() 
{
	return this->_ip;
}

int Machine::get_memory() 
{
	return this->_memory;
}

int Machine::get_num_disks()
{
	return this->_num_disks;
}

int Machine::get_num_vcpus()
{
	return this->_num_vcpus;
}

string Machine::get_rack()
{
	return this->_rack;
}

unordered_map<string, Instance*>& Machine::get_running_instances()
{
	return this->_instances_running;
}

void Machine::update_machine(Instance* ins, bool inc)
{
	Flavor fla = AggieStack::get()->get_flavors()[ins->get_flavor()];
	int new_num_disks = inc ? (this->get_num_disks() + fla.get_num_disks()) : (this->get_num_disks() - fla.get_num_disks());
	int new_memory = inc ? (this->get_memory() + fla.get_ram()) : (this->get_memory() - fla.get_ram());
	int new_num_vcpus = inc ? (this->get_num_vcpus() + fla.get_num_vcpus()) : (this->get_num_vcpus() - fla.get_num_vcpus());
	this->set_num_disks(new_num_disks);
	this->set_memory(new_memory);
	this->set_num_vcpus(new_num_vcpus);
}

bool Machine::add_instance(Instance* ins) 
{
	auto& instances = this->_instances_running;
	if (instances.find(ins->get_name()) != instances.end()) {
		cerr << "Another Instance of this name is already present in this machine" << endl;
		return false;
	}

	if (AggieStack::get()->can_host(this->get_name(), ins->get_flavor())) {
		instances[ins->get_name()] = ins;		// instance added
		this->update_machine(ins, false);		// Machine updated (dec)
		return true;
	}

	return false;
}

bool Machine::delete_instance(string ins_name) 
{
	auto& instances = this->_instances_running;
	if (instances.find(ins_name) == instances.end()) {
		cerr << "Instance to delete not found on this machine" << endl;
		return false;
	}
	Instance* tmp = instances[ins_name];
	this->update_machine(tmp, true);			// Machine updated (inc)
	instances.erase(instances.find(ins_name));	// instance deleted
	delete tmp;
	return true;
}

bool Machine::idle()
{
	return this->get_running_instances().empty();
}

// ************************************ Image *********************************** // 

Image::Image() {}

Image::Image(string name, int size, string path): _name(name), _size(size), _path(path) 
{}

Image::~Image() {}

void Image::set_name(string name)
{
	this->_name = name;
}

void Image::set_size(int size)
{
	this->_size = size;
}

void Image::set_path(string path)
{
	this->_path = path;
}

string Image::get_name()
{
	return _name;
}

int Image::get_size()
{
	return _size;
}

string Image::get_path()
{
	return _path;
}

// ************************************ Flavor *********************************** // 

Flavor::Flavor() {}

Flavor::Flavor(string type, int ram, int num_disks, int num_vcpus): 
	_type(type), _ram(ram), _num_disks(num_disks), _num_vcpus(num_vcpus) 
{}

Flavor::~Flavor() {}

void Flavor::set_type(string type)
{
	this->_type = type;
}

string Flavor::get_type()
{
	return _type;
}
	
void Flavor::set_ram(int ram)
{
	this->_ram = ram;
}

int Flavor::get_ram()
{
	return _ram;
}

void Flavor::set_num_disks(int num_disks)
{
	this->_num_disks = num_disks;
}

int Flavor::get_num_disks()
{
	return _num_disks;
}

void Flavor::set_num_vcpus(int num_vcpus)
{
	this->_num_vcpus = num_vcpus;
}

int Flavor::get_num_vcpus()
{
	return _num_vcpus;	
}

// ************ Rack *********************************** //  

Rack::Rack(): _name(""), _available_storage(0), _max_storage(0), _machines(std::set<string> ()) {}

Rack::Rack(string name, int storage): _name(name), _max_storage(storage)
{
	_available_storage = storage;
	_machines = std::set<string> ();
}

Rack::~Rack() {}

void Rack::set_name(string name) 
{
	this->_name = name;
}

void Rack::set_storage(int storage) 
{
	this->_available_storage = storage;
	this->_max_storage = storage;
}

string Rack::get_name()
{
	return this->_name;
}

int Rack::get_available_storage() 
{
	return this->_available_storage;
}

int Rack::get_max_storage()
{
	return this->_max_storage;
}

std::vector<string>& Rack::get_cached_images()
{
	return this->_cached_images;
}

std::set<string>& Rack::get_machines()
{
	return this->_machines;
}

bool Rack::update_storage(int val, bool inc)
{
	val = inc ? val : -val;
	if (this->_max_storage < this->_available_storage + val ||
		this->_available_storage + val < 0) {
		cerr << "Rack storage error: Should be positive and less than max storage" << endl;
		return false;
	}
	this->_available_storage += val;
	return true;
}

bool Rack::update_cache(string& img, int size)
{
	std::vector<string>& cache = this->get_cached_images();
	if (this->get_max_storage() < size) {
		cerr << "Can't store in this rack: exceeds max storage" << endl;
		return false;
	}
	if (this->get_available_storage() >= size) {
		cache.push_back(img);
		return true;
	} else { 
		bool not_added = true;
		while (true) {
			auto img_iter = cache.begin();
			int val = (AggieStack::get()->get_images())[*img_iter].get_size();
			this->update_storage(val, true);
			cache.erase(img_iter);					// removing the first element
			if (this->get_available_storage() >= size) {
				cache.push_back(img);
				this->update_storage(size, false);
				return true;
			}
		}
	}
	return false;
}

// ************************************ Instance *********************************** // 

Instance::Instance() {}

Instance::Instance(string name, string image, string flavor): 
	_name(name), _image(image), _flavor(flavor)
{}

Instance::~Instance() {}

void Instance::set_name(string name)
{
	this->_name = name;
}

void Instance::set_image(string img)
{
	this->_image = img;
}

void Instance::set_flavor(string flavor)
{
	this->_flavor = flavor;
}

string Instance::get_name()
{
	return this->_name;
}

string Instance::get_image()
{
	return this->_image;
}

string Instance::get_flavor()
{
	return this->_flavor;
}

