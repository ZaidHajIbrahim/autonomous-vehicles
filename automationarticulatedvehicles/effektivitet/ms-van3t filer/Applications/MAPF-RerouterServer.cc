
#include "MAPF-RerouterServer.h"
#include "MAPF-RerouterClient.h"
#include "ns3/log.h"

#include <pybind11/embed.h>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE("MAPFRerouterServer");
NS_OBJECT_ENSURE_REGISTERED(MAPFRerouterServer);

TypeId MAPFRerouterServer::GetTypeId(void) {
    static TypeId tid = TypeId("ns3::MAPFRerouterServer")
        .SetParent<Application>()
        .SetGroupName("Applications")
        .AddConstructor<MAPFRerouterServer>();
    return tid;
}

MAPFRerouterServer::MAPFRerouterServer() {}

MAPFRerouterServer::~MAPFRerouterServer() {}

void MAPFRerouterServer::StartApplication() {
    NS_LOG_INFO("Starting MAPFRerouterServer...");
    m_sumoClient = GetNode()->GetObject<TraciClient>();
    if (!m_sumoClient) {
        NS_LOG_ERROR("TraciClient not found on node!");
    }
}

void MAPFRerouterServer::StopApplication() {
    NS_LOG_INFO("Stopping MAPFRerouterServer...");
    m_sumoClient = nullptr;
}

std::vector<std::pair<std::string, std::string>> MAPFRerouterServer::GetVehiclePositions() {
    std::vector<std::pair<std::string, std::string>> vehiclePositions;

    // Iterate through all MAPF-RerouterClients on the node
    for (auto client : m_clients) {
        std::string vehID = client->GetVehicleID();
        std::string roadID = client->GetCurrentRoadID();
        vehiclePositions.emplace_back(vehID, roadID);
    }

    return vehiclePositions;
}

std::unordered_map<std::string, std::string> MAPFRerouterServer::GetVehicleDestinations() {
    std::unordered_map<std::string, std::string> vehicleDestinations;

    // Iterate through all MAPF-RerouterClients on the node
    for (auto client : m_clients) {
        std::string vehID = client->GetVehicleID();
        std::string destination = client->GetDestination();
        vehicleDestinations[vehID] = destination;
    }

    return vehicleDestinations;
}

bool MAPFRerouterServer::RunCCBSAndProcessSolution(const std::string &taskFile, const std::string &graphmlFile) {
    // Implement logic to run CCBS and process the solution
    return true;
}

} // namespace ns3




namespace py = pybind11;
namespace py = pybind11;
namespace fs = std::filesystem;
std::string path = "src/automotive/model/"

std::string runCommand(const std::string &command) {
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

bool runCCBS() {
    std::string ccbsPath = "./Continous-CBS/CCBS"; // Path to the CCBS executable
    std::string roadmap = path + "Applications/XML-files/CCBS_map.xml"; // Path to the roadmap file
    std::string task = path + "Applications/XML-files/CCBS_map_task.xml"; // Path to the task file

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

bool convertSolutionToSUMO(const std::string &taskFile, const std::string &graphmlFile) {
    py::scoped_interpreter guard{}; // Start the Python interpreter

    try {
        // Import the Python module
        py::module_ solutionToSUMO = py::module_::import("Python-Support-Apps.solution_to_SUMO");

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




