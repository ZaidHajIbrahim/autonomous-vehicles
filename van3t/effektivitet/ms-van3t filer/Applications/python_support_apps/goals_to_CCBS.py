import xml.etree.ElementTree as ET
from xml.dom import minidom

def convert_to_tasks(input_list, graphml_file, output_file):

    """
    Convert a list of start and goal SUMO IDs to a tasks.xml file.
    :param input_list: List of strings in the format "startID,goalID".
    :param graphml_file: Path to the GraphML file (output1.xml).
    :param output_file: Path to the output tasks.xml file.
    """ 
    # Parse the GraphML file
    tree = ET.parse(graphml_file)
    root = tree.getroot()

    # Create a dictionary to map SUMO IDs to their corresponding node IDs
    sumo_to_node = {}
    for edge in root.findall(".//edge"):  
        sumo_id = edge.get("sumo_id")
        target = edge.get("target")
        if sumo_id and target:
            sumo_to_node[sumo_id] = target
        else:
            print(f"Skipping edge with missing attributes: {ET.tostring(edge, encoding='unicode')}")

    #print(f"SUMO to Node Mapping: {sumo_to_node}")

    # Create the root element for the tasks.xml file
    tasks_root = ET.Element("root")

    # Process each entry in the input list
    for entry in input_list:
        start_sumo_id, goal_sumo_id = entry.split(",")

        # Find the corresponding node IDs
        start_node = sumo_to_node.get(start_sumo_id.split("_")[0])  # Remove underscore and number
        goal_node = sumo_to_node.get(goal_sumo_id.split("_")[0])  # Remove underscore and number
        print(f"Start Node: {start_node}, Goal Node: {goal_node}")

        if not start_node:
            raise ValueError(f"Could not find nodes for startID: {start_sumo_id}")
        if not goal_node:
            raise ValueError(f"Could not find nodes for goalID: {goal_sumo_id}")

        # Add the agent to the tasks.xml file
        agent_element = ET.SubElement(tasks_root, "agent")
        agent_element.set("start_id", start_node.lstrip("n"))  # Remove the "n" prefix
        agent_element.set("goal_id", goal_node.lstrip("n"))    # Remove the "n" prefix

    # Convert the XML tree to a pretty-printed string
    rough_string = ET.tostring(tasks_root, encoding="utf-8")
    reparsed = minidom.parseString(rough_string)
    pretty_xml = reparsed.toprettyxml(indent="   ")

    # Write the output to the tasks.xml file
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(pretty_xml)

if __name__ == "__main__":
    # Example usage
    input_list = [
        "-14191809#0,w1_to_ww1",
        "-148201333#0,-27302749#10"
    ]
    graphml_file = "van3t/effektivitet/output1.xml"
    output_file = "van3t/effektivitet/tasks_from_SUMO.xml"

    convert_to_tasks(input_list, graphml_file, output_file)
    print(f"Tasks file generated: {output_file}")
