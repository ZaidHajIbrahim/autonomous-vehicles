import xml.etree.ElementTree as ET
from Sumo_Xml_Parser import parse_xml
from xml.dom import minidom
from itertools import chain

def convertGoaltoCCBS(input_file, output_file, agent_list, task_file):
    sumo_root = parse_xml(input_file)
    cbs_root = parse_xml(output_file)
    namespace = {'gml': 'http://graphml.graphdrawing.org/xmlns'}
    
    root = ET.Element("root")
    

    sumo_cbs_dict = {}
    # Create dictionary to avoid looping throu the files more than once
    for CBS_edge in cbs_root.findall('.//gml:edge', namespace):
        sumo_cbs_dict[CBS_edge.attrib['sumo_id']] = CBS_edge.attrib['target']

    # Go throu all agents and edges in SUMO
    for agent in agent_list:
        start, goal = agent.split(",")
        mapped_start = None 
        mapped_goal = None
        node = None

        for sumo_edge in chain(sumo_root.findall('edge'), sumo_root.findall('connection')):
            if sumo_edge.get('function') == 'internal':
                continue # Skip all internal nodes

            # Find the edge in SUMO
            if sumo_edge.tag == 'edge':
                sumo_id = sumo_edge.get('id')  # 'edge' har 'id'

            elif sumo_edge.tag == 'connection':
                node = sumo_edge.get('from')  # 'connection' har ofta bara 'to'
                sumo_id = sumo_edge.get('to')

            # Determine if the sumo_id is the start or goal point
            if (start == sumo_id or start == node) and sumo_id in sumo_cbs_dict:
                mapped_start = sumo_cbs_dict[sumo_id]
                if mapped_goal != None:
                    break

            if (goal == sumo_id or goal == node) and sumo_id in sumo_cbs_dict:
                mapped_goal = sumo_cbs_dict[sumo_id]
                if mapped_start != None:
                    break
        
        # If a staring of goal point can't be found
        if mapped_start is None or mapped_goal is None:
            print("Can't find nodes for: ",agent)
            continue
        
        agent_lista = ET.SubElement(root,"agent")
        agent_lista.set("start_id", f"{mapped_start[1:]}")
        agent_lista.set("goal_id", f"{mapped_goal[1:]}")

    rough_string = ET.tostring(root, encoding="utf-8")
    reparsed = minidom.parseString(rough_string)
    pretty_xml = reparsed.toprettyxml(indent="   ")

    with open(task_file, "w", encoding="utf-8") as f:
        f.write(pretty_xml)
    
    
    #return start,goal

if __name__ == "__main__":
    output_file = 'output.xml'
    input_file = 'osm1.net.xml'
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
        ":135500233_9,:138244412_8",
        "-14178971#1,E13",
        "-339502462#3,:138244418_0"
    ]
    
    graphml_file = "output.xml"
    output_file = "output1.xml"

    new_nodes = convertGoaltoCCBS(input_file, output_file, input_list, 'tasks.xml')
    print(new_nodes)