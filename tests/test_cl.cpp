#include<iostream>
#include<fstream>
#include<iterator>
#include<string>
#include <utility>
#define __NO_STD_VECTOR // Use cl::vector instead of STL version

#include<CL/cl.hpp>

using namespace std;

void printPlatforms( cl::vector<cl::Platform> platformList ){
	for(  auto plf : platformList ){
		std::string platformVendor;
		std::string platformName;
		std::string platformVersion;
		plf.getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
		plf.getInfo((cl_platform_info)CL_PLATFORM_NAME, &platformName);
		plf.getInfo((cl_platform_info)CL_PLATFORM_VERSION, &platformVersion);
		cout << "name:    " << platformName << endl;
		cout << "version: " << platformVersion << endl;
		cout << "vendor:  " << platformVendor << endl;
	}
}

void printDevices( cl::vector<cl::Device> devices, cl::Platform platform ){

	// name of the current platform for debugging messages
	std::string platformName;
	platform.getInfo((cl_platform_info)CL_PLATFORM_NAME, &platformName);

	// print informations on devices
	cout << devices.size() << " devices found on " << platformName << ".\n";
	for( auto dev : devices ){
		string devName;
		string devOpenCLVersion;
		string devProfile;
		string devVersion;
		string driverVersion;
		cl::vector<size_t> maxWorkItemSizes;
		dev.getInfo( CL_DEVICE_MAX_WORK_ITEM_SIZES, &maxWorkItemSizes );
		dev.getInfo( CL_DEVICE_NAME             , &devName );
		dev.getInfo( CL_DEVICE_OPENCL_C_VERSION , &devOpenCLVersion );
		dev.getInfo( CL_DEVICE_PROFILE          , &devProfile );
		dev.getInfo( CL_DEVICE_VERSION          , &devVersion );
		dev.getInfo( CL_DRIVER_VERSION          , &driverVersion );
		cout << "device    : " << devName       << "\n";
		cout << "OpenCl ver: " << devOpenCLVersion << "\n";
		cout << "profile   : " << devProfile    << "\n";
		cout << "device ver: " << devVersion    << "\n";
		cout << "driver ver: " << driverVersion << "\n";
		cout << "max work item sizes:";
		for( size_t size : maxWorkItemSizes )
			cout << " " << size;
		cout << "\n";
	}
}

int main(){

	const std::string hw("Hello World\n");

	// introduction message
	cout << "This program will look for openCL environment, then OpenCL compatible devices" << endl;

	// ###########################
	// # list availiable platforms
	cout << "# Looking for platforms." << endl;
	cl::vector< cl::Platform > platformList;
	cl::Platform::get(&platformList);
	cout << platformList.size() << " platform(s) found:" << endl;
	if( platformList.size() == 0 ){
		cerr << "! No platform found" << endl;
		return -1;
	}

	printPlatforms(platformList);


	// ##############
	// # list devices
	cout << "# Looking for devices on each platforms\n";
	cl::vector<cl::Device> devices;	
	for( auto plf : platformList ){
		// get devices
		cl_int error;
		error = plf.getDevices(CL_DEVICE_TYPE_ALL, &devices);


		if( error != CL_SUCCESS ){
			cerr << "! Failed to find devices\n";
			cerr << "error code is " << error;
			return -1;
		}else if( devices.size() == 0 ){
			cerr << "! No device found\n";
			return -1;
		}else{
			printDevices( devices, plf );
		}
	}

	// #################
	// # start a context
	cout << "# starting context\n";
	cl_int error;
	cl::Context context( devices, NULL, NULL, NULL, &error );
	if( error != CL_SUCCESS ){
		cerr << "error while creating context, error code is " << error << "\n";
		return -1;
	}

	// ##################
	// # make test buffer
	char* outH = new char[hw.length()+1];
	cl::Buffer outCL(
			context,
			CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
			hw.length()+1,
			outH,
			&error);
	if( error != CL_SUCCESS ){
		cerr << "failed to load buffer\n";
		return -1;
	}

	// #############
	// # load kernel
	// open file
	std::ifstream file("tests/test_kernel.cl");
	std::string prog(
			std::istreambuf_iterator<char>(file),
			(std::istreambuf_iterator<char>())
			);
	// load kernels code
	cl::Program::Sources source(1, std::make_pair(prog.c_str(), prog.length()+1));
	cl::Program program(context, source);
	error = program.build(devices,"");
	if( error != CL_SUCCESS ){
		cerr << "Failed to load kernel\n";
		return -1;
	}

	// create kernel
	cl::Kernel kernel(program, "hello", &error);
	if( error != CL_SUCCESS ){
		cerr << "Failed to create kernel\nerror code \n"<< error << "\n";
		return -1;
	}

	error = kernel.setArg(0, outCL);
	if( error != CL_SUCCESS ){
		cerr << "Failed to set kernel arguments\n";
		return -1;
	}

	cl::CommandQueue queue(context, devices[0], 0, &error);
	if( error != CL_SUCCESS ){
		cerr << "Failed to create command queue\n";
		return -1;
	}

	cl::Event event;
	error = queue.enqueueNDRangeKernel(
			kernel,
			cl::NullRange,
			cl::NDRange(hw.length()+1),
			cl::NDRange(1, 1),
			NULL,
			&event);
	if( error != CL_SUCCESS ){
		cerr << "Failed to load coammand queue\n";
		return -1;
	}

	event.wait();
	error = queue.enqueueReadBuffer(
			outCL,
			CL_TRUE,
			0,
			hw.length()+1,
			outH);
	if( error != CL_SUCCESS ){
		cerr << "Failed to execute kernel\n";
		return -1;
	}
	std::cout << outH;

return 0;
}
