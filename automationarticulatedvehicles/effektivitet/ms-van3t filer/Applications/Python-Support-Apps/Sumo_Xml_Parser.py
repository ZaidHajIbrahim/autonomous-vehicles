import os

import xml.etree.ElementTree as ET
import math
#import pandas as pd

def parse_xml(finput_file):
    tree = ET.parse(finput_file)
    root = tree.getroot()
    return root

edges = []
junctions = []


def createRoadGraph(input_file, output_file):
    


    root = parse_xml(input_file)
    graphhml = ET.Element("graphml")
    print(ET.SubElement(graphhml, "graph"))
    key0 = ET.SubElement(graphhml, "key", attrib={"id": "key0", "for": "node", "attr.name": "coords", "attr.type": "string"})
    key1 = ET.SubElement(graphhml, "key", attrib={"id": "key0", "for": "edge", "attr.name": "weight", "attr.type": "double"})
    graph_root = ET.SubElement(graphhml, "graph")
    graph_root.set("id", "G")
    graph_root.set("edgedefault", "directed")
    graph_root.set("parse.nodeids", "free")
    graph_root.set("parse.edgeids", "canonical")
    graph_root.set("parse.order", "nodesfirst")
    print(graph_root)

    edge_count = 0
    node_count = 0
    for child in root:

            if child.tag == 'junction' and child.attrib.get('type') != 'internal':
                junctions_info = {
                    'original_id': f"{node_count}",
                    'x': child.attrib.get('x'),
                    'y': child.attrib.get('y'),
                    'sumo_id': child.attrib.get('id')
                }

                junctions.append(junctions_info)
                node_count += 1
            elif child.tag == 'edge' and child.attrib.get('from') != None:
                for lane in child.findall('lane'):
                    lane_info = {
                            'speed': lane.attrib.get('speed'),
                            'length': lane.attrib.get('length')
                        }
                speed = lane_info['speed']
                length = child.attrib.get('length')
                if length == None:
                    length = lane_info['length']
                if speed == None:
                    speed = lane_info['speed']
                #speed = child.attrib.get('speed')

                try:
                    weight = float(length) / float(speed)
                except:
                    weight = 1
                edge_info = {
                    'id': f"e{edge_count}",
                    'from': child.attrib.get('from'),
                    'to': child.attrib.get('to'),
                    'sumo_id': child.attrib.get('id'),
                    'weight': weight
                }
                #print(f"Length: {length}, Speed: {speed} weight: {weight} ")
                edges.append(edge_info)
                edge_count += 1
            elif child.tag == 'lane':
                print('Speed: ',child.attrib.get('speed'))
    #print("\nEdges:", edges)
    #print("\nJunctions:", junctions)


    internal_node_list = []
    new_node_id = 0 # Globala räknaren för noder. Slutsiffran är samma som antalet noder
    end_count = edge_count 
    original_id_loop = junctions[0]['original_id']
    base_node_id = 0

    for junctions_info in junctions:
        connected_edges = [edge for edge in edges if edge['from'] == junctions_info['sumo_id'] or edge['to'] == junctions_info['sumo_id']]
        internal_node_count = 0  # Number of internal nodes for this junction
        internal_node_ids = []  # List to store IDs of internal nodes for this junction

        # Create internal nodes for each connected edge
        for i, edge in enumerate(connected_edges):
            node_id = f"n{new_node_id}"
            internal_node_ids.append(node_id)  # Store the ID of the internal node

            # Calculate the position of the internal node in a circle around the junction
            new_node_angle = (2 * math.pi / len(connected_edges)) * i
            radius = 0.1
            x_offset = float(junctions_info['x']) + radius * math.cos(new_node_angle)
            y_offset = float(junctions_info['y']) + radius * math.sin(new_node_angle)

            # Create the internal node in the XML
            junction_element = ET.SubElement(graph_root, "node", id=node_id, sumo_id=junctions_info['sumo_id'])
            data_element = ET.SubElement(junction_element, "data", key="key0")
            data_element.text = f"{x_offset},{y_offset}"

            # Update the edge to connect to this new internal node
            if edge['from'] == junctions_info['sumo_id']:
                edge['from'] = node_id
            elif edge['to'] == junctions_info['sumo_id']:
                edge['to'] = node_id

            new_node_id += 1
            internal_node_count += 1

        # Create internal edges between all internal nodes
        # Create internal edges between all internal nodes
        for i in range(len(internal_node_ids)):
            for j in range(len(internal_node_ids)):  # Allow connections in both directions
                if i != j:  # Avoid self-loops
                    edge_id = f"e{edge_count}"
                    junction_edge = {
                        "id": edge_id,
                        "from": internal_node_ids[i],
                        "to": internal_node_ids[j],
                        "sumo_id": "NIL",
                        "weight": 1
                    }
                    edges.append(junction_edge)
                    edge_count += 1


    for edge_info in edges:
        edge_element = ET.SubElement(graph_root, "edge", id=edge_info['id'], source=edge_info['from'], target=edge_info['to'], sumo_id=edge_info['sumo_id'])
        data_element = ET.SubElement(edge_element, "data", key="key1")
        data_element.text = f"{edge_info['weight']}"




    tree = ET.ElementTree(graphhml)
    def indent(elem, level=0):
        i = "\n" + level*"  "
        j = "\n" + (level-1)*"  "
        if len(elem):
            if not elem.text or not elem.text.strip():
                elem.text = i + "  "
            if not elem.tail or not elem.tail.strip():
                elem.tail = i
            for subelem in elem:
                indent(subelem, level+1)
            if not elem.tail or not elem.tail.strip():
                elem.tail = j
        else:
            if level and (not elem.tail or not elem.tail.strip()):
                elem.tail = j
        return elem
    indent(graphhml)

    if os.path.exists(output_file):
        os.remove(output_file)
    tree.write(output_file, xml_declaration=True, encoding='utf-8', method="xml")
    print("XML file created successfully!")

if __name__ == "__main__":
    output_file = 'van3t/effektivitet/output1.xml'
    input_file = 'van3t/effektivitet/osm.net.xml'
    createRoadGraph(input_file, output_file)