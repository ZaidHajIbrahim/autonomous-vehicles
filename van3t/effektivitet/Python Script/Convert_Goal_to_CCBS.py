import xml.etree.ElementTree as ET
from Sumo_Xml_Parser import parse_xml
from xml.dom import minidom

def convertGoaltoCCBS(input_file, output_file, agent_list, task_file):
    #print(agent_list)
    sumo_root = parse_xml(input_file)
    cbs_root = parse_xml(output_file)
    namespace = {'gml': 'http://graphml.graphdrawing.org/xmlns'}
    
    root = ET.Element("root")
    start_id = set()
    goal_id = set()
    

    sumo_cbs_dict = {}
    # Create dictionary to avoid looping throu the files more than once
    for CBS_edge in cbs_root.findall('.//gml:edge', namespace):
        sumo_cbs_dict[CBS_edge.attrib['sumo_id']] = CBS_edge.attrib['target']

    # Go throu all agents and edges in SUMO
    for agent in agent_list:
        start, goal = agent.split(",")
        for sumo_edge in sumo_root.findall('edge'):
            if sumo_edge.get('function') == 'internal':
                continue # Skip all internal nodes
            
            sumo_id = sumo_edge.get('id') # ID of the edge in SUMO
            # Check if the ID can be found in the dictionary and are used by the agent
            if goal == sumo_id and sumo_id in sumo_cbs_dict:
                goal = sumo_cbs_dict[sumo_id]
                goal_id.add(sumo_cbs_dict[sumo_id])

            if start == sumo_id and sumo_id in sumo_cbs_dict:
                start = sumo_cbs_dict[sumo_id]
                start_id.add(sumo_cbs_dict[sumo_id])

        
        agent_lista = ET.SubElement(root,"agent")
        agent_lista.set("start_id", f"{{'{start}'}}")
        agent_lista.set("goal_id", f"{{'{goal}'}}")

    rough_string = ET.tostring(root, encoding="utf-8")
    reparsed = minidom.parseString(rough_string)
    pretty_xml = reparsed.toprettyxml(indent="   ")

    with open(task_file, "w", encoding="utf-8") as f:
        f.write(pretty_xml)
    
    
    #return start,goal

if __name__ == "__main__":
    output_file = 'output.xml'
    input_file = 'osm.net.xml'
    agent_list = [
    {'id': 'veh1', 'start': '27302781#0', 'goal': '-27302749#10'},
    {'id': 'veh2', 'start': '-148201333#1', 'goal': '-27302773#2'}
    ]

    agent_list = [
    {'id': 'veh1', 'start': '-14191809#0', 'goal': 'w1_to_ww1'},
    {'id': 'veh2', 'start': '-148201333#0', 'goal': '-27302749#10'}
    ]

    input_list = [
        "-14191809#0,w1_to_ww1",
        "-148201333#0,-27302749#10"
    ]

    input_list = [
        "27302781#0,-27302749#10",
        "-148201333#1,-27302773#2"
    ]
    
    input_list = [
        "-27302781#1,27302749#7",
        "w1_to_ww1,27302832#2"
    ]
    
    graphml_file = "output.xml"
    #output_file = "output1.xml"

    new_nodes = convertGoaltoCCBS(input_file, output_file, input_list, 'tasks.xml')
    print(new_nodes)