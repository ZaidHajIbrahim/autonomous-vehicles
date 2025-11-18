import xml.etree.ElementTree as ET
from Sumo_Xml_Parser import parse_xml
from xml.dom import minidom

def convertGoaltoCCBS(input_file, output_file, agent_list): #, task_file):
    sumo_root = parse_xml(input_file)
    cbs_root = parse_xml(output_file)
    namespace = {'gml': 'http://graphml.graphdrawing.org/xmlns'}

    '''
    root = ET.Element("root")
    start_id = set()
    goal_id = set()
    '''

    sumo_cbs_dict = {}
    # Create dictionary to avoid looping throu the files more than once
    for CBS_edge in cbs_root.findall('.//gml:edge', namespace):
        sumo_cbs_dict[CBS_edge.attrib['sumo_id']] = CBS_edge.attrib['target']

    # Go throu all agents and edges in SUMO
    for agent in agent_list:
        for sumo_edge in sumo_root.findall('edge'):
            if sumo_edge.get('function') == 'internal':
                continue # Skip all internal nodes
            
            sumo_id = sumo_edge.get('id') # ID of the edge in SUMO
            # Check if the ID can be found in the dictionary and are used by the agent
            if agent['goal'] == sumo_id and sumo_id in sumo_cbs_dict:
                agent['goal'] = sumo_cbs_dict[sumo_id]
                #goal_id.add(sumo_cbs_dict[sumo_id])
            if agent['start'] == sumo_id and sumo_id in sumo_cbs_dict:
                agent['start'] = sumo_cbs_dict[sumo_id]
                #start_id.add(sumo_cbs_dict[sumo_id])

    '''# Om vi vill få det i xml-fil format
    for agent in agent_list:
        agent_lista = ET.SubElement(root,"agent")
        agent_lista.set("start_id", f"{{'{agent['start']}'}}")
        agent_lista.set("goal_id", f"{{'{agent['goal']}'}}")

    rough_string = ET.tostring(root, encoding="utf-8")
    reparsed = minidom.parseString(rough_string)
    pretty_xml = reparsed.toprettyxml(indent="   ")

    with open(task_file, "w", encoding="utf-8") as f:
        f.write(pretty_xml)
    '''

    return agent_list

if __name__ == "__main__":
    output_file = 'output.xml'
    input_file = 'osm.net.xml'
    agent_list = [
    {'id': 'veh1', 'start': '27302781#0', 'goal': '-27302749#10'},
    {'id': 'veh2', 'start': '-148201333#1', 'goal': '-27302773#2'}
    ]
    new_nodes = convertGoaltoCCBS(input_file, output_file, agent_list) #, 'tasks.xml')
    print(new_nodes)