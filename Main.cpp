#include <iostream>
#include <algorithm>
#include "AggieStack.h"

using namespace std;

int main(int argc, char **argv) 
{
	string hdwr_file = "input_p1/hdwr-config.txt";
	string img_file = "input_p1/image-config.txt";
	string flv_file = "input_p1/flavor-config.txt";

	AggieStack* aggie = AggieStack::get();

	clock_t start_time;
	start_time = clock();
	ifstream input;
	bool interactive = false;		// by default
	if (argv && argc < 1) {
		cout << "Unknown Error" << endl;
		return 0;
	}

	if (1 == argc) {
		cout << "\t\t\t\t---------------------- Usage ---------------------" << endl;
		cout << "\t\t\t\t    To read input file, type: ./Main <command_file>" << endl;
		cout << "\t\t\t\tTo go to Interactive Environment, type: ./Main -I" << endl;
		cout << "\t\t\t\t           For help, type: ./Main -help" << endl;
		cout << "\t\t\t\t--------------------------------------------------" << endl;
		return 0;
	} else if (2 == argc && string(argv[1]) == "-help") {
		cout << "\t\t\t\t---------------------- Usage ---------------------" << endl;
		cout << "\t\t\t\t    To read input file, type: ./Main <command_file>" << endl;
		cout << "\t\t\t\tTo go to Interactive Environment, type: ./Main -I" << endl;
		cout << "\t\t\t\t--------------------------------------------------" << endl;
		return 0;
	} else if (2 == argc && (string(argv[1]) == "-I" || string(argv[1]) == "-i")) {
		interactive = true;
		cout << "\t\t\t\t-------- AggieStack CLI ----------" << endl;
		cout << "\t\t\t\tVersion 0.2 for the AggieStack CLI" << endl;
		cout << "\t\t\t\t     Developed by Aryan Sharma" << endl;
		cout << "\t\t\t\t----------------------------------" << endl;
		cout << endl << "Entering Interactive Mode" << endl 
			<< "Type command and hit ENTER to execute" << endl 
			<< "Type \"quit/q\" to quit the program." << endl << endl;
	} else {
		string command_file = argv[1];
		input.open(command_file);
		if (!input.is_open()) {
			cerr << "Cannot Open file: " << command_file << endl;
			return 0;
		} else {
			cout << "File read successfully" << endl;
		}
	}

	unsigned long int ios = 0;
	double curr_time = 0;
	string line;
	vector<string> words;
	int c = 1;

	while (true) {
		if (interactive) {
			cout << "In [" << c << "] ";
			c++;						// incrementing command line
			getline(cin, line);
			if (line == "QUIT" || line == "quit" || line == "q") {
				cout << endl;
				break;
			}
		} 
		else {
			if (!getline(input, line)) 
				break;	
		}

		if (!line.empty() && std::find_if(line.begin(), line.end(), [](char c) { return !std::isdigit(c); }) == line.end()) 
		{
			switch (stoi(line)) {
				case 1: 
				{
					line = "aggiestack read all config";
					break;
				}
				case 2:
				{
					string h;
					cout << "Enter hardware config file (relative/absolute path): ";
					cin >> h;
					line = "aggiestack config --hardware " + h;
					break;
				}
				case 3:
				{
					string f;
					cout << "Enter flavor config file (relative/absolute path): ";
					cin >> f;
					line = "aggiestack config --flavor " + f;
					break;
				}
				case 4:
				{
					string ig;
					cout << "Enter image config file (relative/absolute path): ";
					cin >> ig;
					line = "aggiestack config --image " + ig;
					break;
				}
				case 5:
					line = "aggiestack show hardware";
					break;
				case 6:
					line = "aggiestack show images";
					break;
				case 7:
					line = "aggiestack show flavors";
					break;
				case 8:
					line = "aggiestack show racks";
					break;
				case 9:
				{
					string ig, f, in;
					cout << "Enter image: ";
					cin >> ig;
					cout << "Enter flavor: ";
					cin >> f;
					cout << "Enter instance name: ";
					cin >> in;
					line = "aggiestack server create --image " + ig + " --flavor " + f + " " + in;
					break;
				}
				case 10:
				{
					string in;
					cout << "Enter instance name to be deleted: ";
					cin >> in;
					line = "aggiestack server delete " + in;
					break;
				}
				case 11:
					line = "aggiestack server list";
					break;
				case 12:
				{
					string r;
					cout << "Enter Rack name: ";
					cin >> r;
					line = "aggiestack server show imagecaches " + r;
					break;
				}
				case 13:
					line = "aggiestack server show hardware";
					break;
				case 14:
					line = "aggiestack admin show instances";
					break;
				case 15:
					line = "aggiestack admin show hardware";
					break;
				case 16:
				{
					string r;
					cout << "Enter Rack name: ";
					cin >> r;
					line = "aggiestack admin evacuate " + r;
					break;
				}
				case 17:
				{
					string m; 
					cout << "Enter machine name: ";
					cin >> m;
					line = "aggiestack admin remove " + m;
					break;
				}
				case 18:
				{
					string m,me,d,v,ip_addr, r;
					cout << "Enter Name: ";
					cin >> m;
					cout << "Enter memory: ";
					cin >> me;
					cout << "Enter number of disks: ";
					cin >> d;
					cout << "Enter number of vCPUs: ";
					cin >> v;
					cout << "Enter IP: ";
					cin >> ip_addr;
					cout << "Enter rack: ";
					cin >> r;
					line = "aggiestack admin add --mem " + me + " --disk " + d + " --vcpus " + v + " --ip " + ip_addr + " --rack " + r + " " + m;
					break;
				}
				default:
					cerr << "Wrong command entered" << endl;
					break;
			}
		}

		cout << "COMMAND: " << line << endl;
		std::transform(line.begin(), line.end(), line.begin(), ::tolower);
		if(line[0] == '#') continue;	// comments
		if(line.size() == 0) continue;

		if (line == "read all" || line == "aggiestack read all config" || line == "a read all") {
			aggie->read_machine_config(hdwr_file);
			aggie->read_image_config(img_file);
			aggie->read_flavor_config(flv_file);
			cout << "read all" << endl;
			continue;
		}

		if (line == "help" || line == "h") {
			cout << "---------------------- Commands ---------------------" << endl;
			cout << "1.  aggiestack read all config" << endl;
			cout << "2.  aggiestack config --hardware <hardware_config_file.txt>" << endl;
			cout << "3.  aggiestack config --flavors <flavor_config_file.txt>" << endl;
			cout << "4.  aggiestack config --images <image_config_file.txt>" << endl;
			cout << "5.  aggiestack show hardware" << endl;
			cout << "6.  aggiestack show images" << endl;
			cout << "7.  aggiestack show flavors" << endl;
			cout << "8.  aggiestack show racks" << endl;
			cout << "9.  aggiestack server create --image IMAGE --flavor FLAVOR_NAME INSTANCE_NAME" << endl;
			cout << "10. aggiestack server delete INSTANCE_NAME" << endl;
			cout << "11. aggiestack server list" << endl;
			cout << "12. aggiestack server show imagecaches RACK_NAME" << endl;
			cout << "13. aggiestack server show hardware" << endl;
			cout << "14. aggiestack admin show instances" << endl;
			cout << "15. aggiestack admin show hardware" << endl;
			cout << "16. aggiestack admin evacuate RACK_NAME " << endl;
			cout << "17. aggiestack admin remove MACHINE " << endl;
			cout << "18. aggiestack admin add --mem MEMORY --disk NUM_DISKS --vcpus VCPUs --ip IP --rack RACK_NAME MACHINE" << endl;
			cout << "--------------------------------------------------" << endl;
			continue;
		}

		words = split(line," ");		// extract each word into vector words

		if (words[0] != "aggiestack" && words[0] != "a") {
			cout << "Invalid/Unsupported command! aggiestack required" << endl << endl;
			continue;
		}
		if (words[1] == "config") 
		{
			if (words[2] == "--hardware") {
				aggie->read_machine_config(words[3]);
			} 
			else if (words[2] == "--images") {
				aggie->read_image_config(words[3]);
			} 
			else if (words[2] == "--flavors") {
				aggie->read_flavor_config(words[3]);
			} 
			else {
				cout << "Invalid/Unsupported command!" << endl << endl;
				continue;
			}
		} 
		else if (words[1] == "show") 
		{
			if (words[2] == "hardware") {
				aggie->show_machines();
			} 
			else if (words[2] == "images") {
				aggie->show_images();
			} 
			else if (words[2] == "flavors") {
				aggie->show_flavors();
			} 
			else if (words[2] == "racks") {
				aggie->show_racks();
			} 
			else {				
				cout << "Invalid/Unsupported command!" << endl << endl;
				continue;
			}
		} 
		else if (words[1] == "server") 
		{
			if (words[2] == "create") 
			{
				string instance_name, image_name, flavor_name;
				for (int i = 3; i < words.size(); i++) {
					if (words[i][0] == '-') {
						if (words[i] == "--image") {
							image_name = words[i+1];
							i++;
						} 
						else if (words[i] == "--flavor") {
							flavor_name = words[i+1];
							i++;
						}
					} 
					else {
						instance_name = words[i];
					}
				}
				if (aggie->create_instance(instance_name, image_name, flavor_name)) {
					cout << "Instance created successfully" << endl;
				} else {
					cerr << "Instance not created" << endl;
				}
			} 
			else if (words[2] == "delete") 
			{
				if (aggie->delete_instance(words[3])) {
					cout << "Instance deleted successfully" << endl;
				} else {
					cout << "Instance not found" << endl;
				}
			} 
			else if (words[2] == "list") 
			{
				aggie->show_instances(false);
			} 
			else 
			{				
				cout << "Invalid/Unsupported command!" << endl << endl;
				continue;
			}
		} 
		else if (words[1] == "admin") 
		{
			if (words[2] == "show") 
			{
				if (words[3] == "hardware") {
					aggie->show_available_machines();
				} 
				else if (words[3] == "instances") {
					aggie->show_instances(true);
				} 
				else if (words[3] == "imagecaches"){
					aggie->show_rack_status(words[4]);
				} 
				else {
					cout << "Invalid/Unsupported command!" << endl << endl;
					continue;	
				}
			} 
			else if (words[2] == "evacuate") 
			{
				aggie->evacuate_rack(words[3]);
			}
			else if (words[2] == "remove") 
			{
				aggie->remove_machine(words[3]);	
			} 
			else if (words[2] == "add") 
			{
				string name, ip, rack;
				int memory, disk, vcpus;
				for (int i = 3; i < words.size(); i++) {
					if (words[i][0] == '-') {
						if (words[i] == "--mem") {
							memory = stoi(words[i+1]);
							i++;
						} 
						else if (words[i] == "--disk") {
							disk = stoi(words[i+1]);
							i++;
						}
						else if (words[i] == "--vcpus") {
							vcpus = stoi(words[i+1]);
							i++;
						}
						else if (words[i] == "--ip") {
							ip = words[i+1];
							i++;
						}
						else if (words[i] == "--rack") {
							rack = words[i+1];
							i++;
						}
					} 
					else {
						name = words[i];
					}
				}
				if (aggie->add_machine(name, memory, disk, vcpus, ip, rack)) {
					cout << "Machine created successfully" << endl;
				} else {
					cerr << "Machine not added successfully" << endl;
				}
			}
			else {				
				cout << "Invalid/Unsupported command!" << endl << endl;
				continue;
			}
		} 
		else {
			cout << "Invalid/Unsupported command!" << endl << endl;
			continue;
		}
		words.clear();
		cout << endl << endl;
	}

	if (!interactive) input.close();
	cout << " "<< endl;
	cout << "---------- Summary ----------" << endl;
	cout << "Total number of Commands = " << c-1 << endl;
	cout << "Total time elapsed = " << ((double)(clock()-start_time)/CLOCKS_PER_SEC*1000) << " ms" << endl;
	cout << "-----------------------------" << endl;
	return 0;
}

/*
	g++ --std=c++11 -o aggiestack -g AggieStack.cpp Main.cpp
*/

