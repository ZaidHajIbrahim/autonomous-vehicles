#include "MAPF-RerouterServer.h"
#include "MAPF-RerouterClient.h"
#include "ns3/log.h"

//#include <pybind11/pybind11.h>
#include <filesystem>
#include "Python.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("MAPFRerouterServer");
NS_OBJECT_ENSURE_REGISTERED(MAPFRerouterServer);

std::unordered_map<int, std::string> m_ccbsToSumoMap; // CCBS ID -> SUMO Vehicle ID

TypeId MAPFRerouterServer::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::MAPFRerouterServer")
        .SetParent<Application>()
        .SetGroupName("Applications")
        .AddConstructor<MAPFRerouterServer>()
        .AddAttribute("Client",
                      "Pointer to the SUMO TraCI client.",
                      PointerValue(),
                      MakePointerAccessor(&MAPFRerouterServer::m_sumoClient),
                      MakePointerChecker<TraciClient>());
    return tid;
}

MAPFRerouterServer::MAPFRerouterServer() {
NS_LOG_FUNCTION(this);
    m_sumoClient = nullptr;
    m_clients.clear();
    m_ccbsToSumoMap.clear();

}

MAPFRerouterServer::~MAPFRerouterServer() {
    NS_LOG_FUNCTION(this);
}

void MAPFRerouterServer::StartApplication() {
    std::cout << "Starting MAPFRerouterServer..." << std::endl;
    NS_LOG_INFO("Starting MAPFRerouterServer...");
    m_id = m_sumoClient->GetStationId (this -> GetNode ());
    if (!m_sumoClient) {
        std::cout << "Error: SUMO TraCI client not found!" << std::endl;
        NS_LOG_ERROR("TraciClient not found on node!");
    }
    createCCBSgraph();
    ScheduleGatherVehicleData();
}

void MAPFRerouterServer::StopApplication() {
    NS_LOG_INFO("Stopping MAPFRerouterServer...");
    m_sumoClient = nullptr;
}

void MAPFRerouterServer::SetClient(Ptr<TraciClient> client) {
    m_sumoClient = client;
}

void MAPFRerouterServer::AddClient(Ptr<MAPFRerouterClient> client) {
    NS_LOG_INFO("Adding client with vehicle ID: " << client->GetVehicleID());
    m_clients.push_back(client);
}

// Framework to call Python functions
void MAPFRerouterServer::CallPythonFunction(const std::string &moduleName, const std::string &functionName, const std::vector<std::string> &args) {
    //const char* pythonPath = getenv("PYTHONPATH");
    //std::cout << "PYTHONPATH at runtime: " << (pythonPath ? pythonPath : "Not set") << std::endl;
    //std::cout << "Attempting to import module: " << moduleName << std::endl;

    Py_Initialize(); // Initialize the Python interpreter

    // Add the correct path to sys.path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/ed-5355-3/sim/ms-van3t/ns-3-dev/src/automotive/model/Applications')");
    PyRun_SimpleString("print('Python sys.path:', sys.path)");

    // Attempt to import the Python module
    std::cout << "Attempting to import module: " << moduleName << std::endl;
    PyObject* pModule = PyImport_ImportModule(moduleName.c_str());
    if (!pModule) {
        PyErr_Print();
        std::cerr << "Failed to load Python module: " << moduleName << std::endl;
        return;
    }
    // Attempt to retrieve the Python function
    std::cout << "Attempting to retrieve function: " << functionName << std::endl;
    PyObject* pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        PyErr_Print();
        std::cerr << "Failed to load Python function: " << functionName << std::endl;
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        return;
    }

    // Prepare arguments for the Python function
    PyObject* pArgs = PyTuple_New(args.size());
    for (size_t i = 0; i < args.size(); ++i) {
        PyTuple_SetItem(pArgs, i, PyUnicode_FromString(args[i].c_str()));
    }

    // Call the Python function
    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
    
    if (pValue) {
        // Check the type of the returned value
        if (PyBool_Check(pValue)) {
            bool result = (pValue == Py_True);
            std::cout << "Python function returned a boolean: " << (result ? "True" : "False") << std::endl;
        } else if (PyUnicode_Check(pValue)) {
            const char* result = PyUnicode_AsUTF8(pValue);
            std::cout << "Python function returned a string: " << result << std::endl;
        } else {
            std::cout << "Python function returned an unsupported type." << std::endl;
        }
        Py_DECREF(pValue);
    } else {
        PyErr_Print();
    }
    
    // Clean up
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    Py_Finalize(); // Finalize the Python interpreter
}

std::string MAPFRerouterServer::runCommand(const std::string &command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

/*
Function to run CCBS and process the solution
1. Gather vehicle data
2. Create the task XML file
3. Run CCBS
4. Convert the solution to SUMO
5. Send the new routes to each vehicle
*/
bool MAPFRerouterServer::RunCCBSAndProcessSolution(const std::string &taskFile, const std::string &graphmlFile) {
    // Implement logic to run CCBS and process the solution
    // Step 1: Gather the vehicle data
    std::vector<std::tuple<std::string, std::string>> vehicleData = GatherVehicleData();

    // Step 2: Create the task XML file
    if (!createTaskFile(taskFile, vehicleData)) {
        NS_LOG_ERROR("Failed to create task file.");
        return false;
    }

    // Step 3: Run CCBS
    if (!runCCBS()) {
        NS_LOG_ERROR("CCBS failed to find a solution.");
        return false;
    }

    // Step 4: Convert the solution to SUMO
    if (!convertSolutionToSUMO(taskFile, graphmlFile)) {
        NS_LOG_ERROR("Failed to convert CCBS solution to SUMO format.");
        return false;
    }

    // Step 5: Send the new routes to each vehicle
    std::vector<std::vector<std::string>> ccbsRoutes; // Placeholder for parsed routes
    // Parse the CCBS solution into ccbsRoutes (implementation depends on solution format)
    // Assuming ccbsRoutes is populated correctly
    SendRoutesToClients(ccbsRoutes);

    return true;

}

void MAPFRerouterServer::ScheduleGatherVehicleData() {
    NS_LOG_INFO("Scheduling GatherVehicleData...");
    Simulator::Schedule(Seconds(10), &MAPFRerouterServer::GatherVehicleDataWrapper, this);
}

void MAPFRerouterServer::GatherVehicleDataWrapper() {
    // createTaskFile and reschedule it
    std::vector<std::tuple<std::string, std::string>> vehicleData;
    vehicleData = GatherVehicleData();
    createTaskFile("src/automotive/model/Applications/XML-files/CCBS_map_task.xml", vehicleData);
    ScheduleGatherVehicleData();
}

std::vector<std::tuple<std::string, std::string>> MAPFRerouterServer::GatherVehicleData() {
    NS_LOG_INFO("Gathering vehicle data...");
    std::cout << "Gathering vehicle data..." << std::endl;
    std::vector<std::tuple<std::string, std::string>> vehicleData;
    m_ccbsToSumoMap.clear();

    // Get the list of active vehicle IDs from SUMO
    std::vector<std::string> activeVehicleIDs = m_sumoClient->vehicle.getIDList();

    // Remove clients that are no longer active
    auto it = m_clients.begin();
    while (it != m_clients.end()) {
        std::string vehID = (*it)->GetVehicleID();
        if (std::find(activeVehicleIDs.begin(), activeVehicleIDs.end(), vehID) == activeVehicleIDs.end()) {
            // Vehicle is no longer active, remove it
            std::cout << "Removing inactive vehicle: " << vehID << std::endl;
            it = m_clients.erase(it);
        } else {
            ++it;
        }
    }

    // Gather data for active vehicles
    int ccbsId = 0;
    for (auto client : m_clients) {
        std::string vehID = client->GetVehicleID();
        std::string roadID = client->GetCurrentRoadID();
        std::string destination = client->GetDestination();

        std::cout << "Vehicle Data - ID: " << vehID << ", Current Road ID: " << roadID << ", Destination: " << destination << std::endl;
        vehicleData.emplace_back(roadID, destination);
        m_ccbsToSumoMap[ccbsId] = vehID;
        ccbsId++;
    }

    return vehicleData;
}

void MAPFRerouterServer::SendRoutesToClients(const std::vector<std::vector<std::string>> &ccbsRoutes) {
    NS_LOG_INFO("Sending routes to clients...");

    for (size_t ccbsId = 0; ccbsId < ccbsRoutes.size(); ++ccbsId) {
        if (m_ccbsToSumoMap.find(ccbsId) == m_ccbsToSumoMap.end()) {
            NS_LOG_WARN("No mapping found for CCBS ID: " << ccbsId);
            continue;
        }

        std::string vehicleID = m_ccbsToSumoMap[ccbsId];
        auto it = std::find_if(m_clients.begin(), m_clients.end(), [&](Ptr<MAPFRerouterClient> client) {
            return client->GetVehicleID() == vehicleID;
        });

        if (it == m_clients.end()) {
            NS_LOG_WARN("No client found for vehicle ID: " << vehicleID);
            continue;
        }

        Ptr<MAPFRerouterClient> client = *it;
        const std::vector<std::string> &newRoute = ccbsRoutes[ccbsId];
        client->ReceiveRoute(newRoute);

        NS_LOG_INFO("Sent new route to client for vehicle " << vehicleID << ": " << newRoute.size() << " edges.");
    }
}

void MAPFRerouterServer::createCCBSgraph() {
    std::string moduleName = "python_support_apps.SUMOmap_to_CCBSgraph";
    std::string functionName = "createRoadGraph";
    std::vector<std::string> args = {m_netFile, m_outputFile};

    CallPythonFunction(moduleName, functionName, args);
}

bool MAPFRerouterServer::convertSolutionToSUMO(const std::string &taskFile, const std::string &graphmlFile) {
    std::string moduleName = "python_support_apps.solution_to_SUMO";
    std::string functionName = "convert_solution_to_sumo_edges";
    std::vector<std::string> args = {taskFile, graphmlFile, "0.5"};

    CallPythonFunction(moduleName, functionName, args);

    // You can add additional logic here to verify the output if needed
    return true;
}



bool MAPFRerouterServer::createTaskFile(const std::string &taskFile, const std::vector<std::tuple<std::string, std::string>> &vehicleData) {
    std::string moduleName = "python_support_apps.goals_to_CCBS";
    std::string functionName = "convert_to_tasks";

    // Prepare the input list as a vector of strings
    std::vector<std::string> inputList;
    for (const auto &vehicle : vehicleData) {
        std::string entry = std::get<0>(vehicle) + "," + std::get<1>(vehicle);
        inputList.push_back(entry);
    }

    // Initialize the Python interpreter
    Py_Initialize();

    // Add the correct path to sys.path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/ed-5355-3/sim/ms-van3t/ns-3-dev/src/automotive/model/Applications')");

    // Import the Python module
    PyObject* pModule = PyImport_ImportModule(moduleName.c_str());
    if (!pModule) {
        PyErr_Print();
        std::cerr << "Failed to load Python module: " << moduleName << std::endl;
        Py_Finalize();
        return false;
    }

    // Get the Python function
    PyObject* pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        PyErr_Print();
        std::cerr << "Failed to load Python function: " << functionName << std::endl;
        Py_DECREF(pModule);
        Py_Finalize();
        return false;
    }

    // Convert the input list to a Python list
    PyObject* pyList = PyList_New(inputList.size());
    for (size_t i = 0; i < inputList.size(); ++i) {
        PyList_SetItem(pyList, i, PyUnicode_FromString(inputList[i].c_str()));
    }

    // Prepare the arguments for the Python function
    PyObject* pyArgs = PyTuple_New(3);
    PyTuple_SetItem(pyArgs, 0, pyList); // Pass the list of strings
    PyTuple_SetItem(pyArgs, 1, PyUnicode_FromString("src/automotive/model/Applications/XML-files/CCBS_map.xml"));
    PyTuple_SetItem(pyArgs, 2, PyUnicode_FromString(taskFile.c_str()));

    // Call the Python function
    PyObject* pValue = PyObject_CallObject(pFunc, pyArgs);
    if (pValue) {
        std::cout << "Task file created successfully: " << taskFile << std::endl;
        Py_DECREF(pValue);
    } else {
        PyErr_Print();
        std::cerr << "Error calling Python function: " << functionName << std::endl;
    }

    // Clean up
    Py_DECREF(pyArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    Py_Finalize();

    return true;
}

bool MAPFRerouterServer::runCCBS() {
    std::string ccbsPath = "./Continous-CBS/CCBS"; // Path to the CCBS executable
    std::string roadmap = "src/automotive/model/Applications/XML-files/CCBS_map.xml"; // Path to the roadmap file
    std::string task = "src/automotive/model/Applications/XML-files/CCBS_map_task.xml"; // Path to the task file

    // Construct the command
    std::string command = ccbsPath + " " + roadmap + " " + task;

    try {
        // Run the command and capture the output
        std::string output = runCommand(command);

        // Check if "Solution found: true" exists in the output
        if (output.find("Solution found: true") != std::string::npos) {
            std::cout << "Solution found!" << std::endl;
            return true; // Solution found
        } else {
            std::cout << "No solution found!" << std::endl;
            return false; // No solution found
        }
    } catch (const std::exception &e) {
        std::cerr << "Error running CCBS: " << e.what() << std::endl;
        return false; // Failure
    }
}

void MAPFRerouterServer::SetFilePaths(const std::string &netFile, const std::string &outputFile) {
    m_netFile = netFile;
    m_outputFile = outputFile;
}

} // namespace ns3




/*
int convertSUMORoads() {
    py::scoped_interpreter guard{};  // Start Python

    try {
        py::module_ converter = py::module_::import("sumo_converter");
        std::string input_path = path + "examples/sumo_files_v2i_map/map.net.xml";
        std::string output_path = path + "Applications/XML-files/CCBS_map.xml";

        // Check input exists
        if (!fs::exists(input_path)) {
            throw std::runtime_error("Input file not found: " + input_path);
        }

        // Call Python
        bool success = converter.attr("createRoadGraph")(input_path, output_path).cast<bool>();

        if (success && fs::exists(output_path)) {
            std::cout << "XML file generated: " << output_path << std::endl;
        } else {
            throw std::runtime_error("Output file not created");
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}



bool convertSolutionToSUMO(const std::string &taskFile, const std::string &graphmlFile) {
    py::scoped_interpreter guard{}; // Start the Python interpreter

    try {
        // Import the Python module
        py::module_ solutionToSUMO = py::module_::import("python_support_apps.solution_to_SUMO");

        // Derive the task log file name
        std::string taskLogFile = taskFile.substr(0, taskFile.find_last_of('.')) + "_log.xml";

        // Check if the task log file exists
        if (!fs::exists(taskLogFile)) {
            std::cerr << "Error: Task log file not found: " << taskLogFile << std::endl;
            return false;
        }

        // Call the Python function
        py::object result = solutionToSUMO.attr("convert_solution_to_sumo_edges")(taskLogFile, graphmlFile, 0.5);

        // Process the result (if needed)
        std::cout << "Conversion successful. Result:\n" << py::str(result).cast<std::string>() << std::endl;

        return true; // Conversion successful
    } catch (const py::error_already_set &e) {
        std::cerr << "Python error: " << e.what() << std::endl;
        return false; // Failure
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false; // Failure
    }
}



bool createTaskFile(const std::string &taskFile, const std::vector<std::tuple<std::string, std::string, std::string>> &vehicleData) {
    py::scoped_interpreter guard{}; // Start the Python interpreter

    try {
        // Import the Python module
        py::module_ goalsToCCBS = py::module_::import("python_support_apps.goals_to_CCBS");

        // Prepare the input list for the Python function
        py::list inputList;
        for (const auto &vehicle : vehicleData) {
            py::dict agent;
            agent["startID"] = std::get<1>(vehicle); // Current road ID
            agent["goalID"] = std::get<2>(vehicle);  // Destination
            inputList.append(agent);
        }

        // Define the GraphML file and output task file paths
        std::string graphmlFile = "src/automotive/model/Applications/XML-files/CCBS_map.xml";

        // Call the Python function to generate the task file
        goalsToCCBS.attr("convert_to_tasks")(inputList, graphmlFile, taskFile);

        std::cout << "Task file created successfully: " << taskFile << std::endl;
        return true; // Task file creation successful
    } catch (const py::error_already_set &e) {
        std::cerr << "Python error: " << e.what() << std::endl;
        return false; // Failure
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false; // Failure
    }
}

*/