import os
 
import xml.etree.ElementTree as ET
 
def parse_xml(finput_file):
    tree = ET.parse(finput_file)
    root = tree.getroot()
    return root
 
edges = []
junctions = []
 
if __name__ == "__main__":
    input_file = 'testing.net.xml'
    output_file = "hej.net.xml"
 
 
    root = parse_xml(input_file)
    graphhml = ET.Element("graphml")
    key0 = ET.SubElement(graphhml, "key", attrib={"id": "key0", "for": "node", "attr.name": "coords", "attr.type": "string"})
    key1 = ET.SubElement(graphhml, "key", attrib={"id": "key0", "for": "edge", "attr.name": "weight", "attr.type": "double"})
    graph_root = ET.SubElement(graphhml, "graph")
    graph_root.set("id", "G")
    graph_root.set("edgedefault", "directed")
    graph_root.set("parse.nodeids", "free")
    graph_root.set("parse.edgeids", "canonical")
    graph_root.set("parse.order", "nodesfirst")
    
    
    for child in root:
 
            if child.tag == 'junction' and child.attrib.get('type') != 'internal':
                junctions_info = {
                    'id': child.attrib.get('id'),
                    'x': child.attrib.get('x'),
                    'y': child.attrib.get('y')
                }
                junctions.append(junctions_info)
            elif child.tag == 'edge' and child.attrib.get('from') != None:
        
                
                for lane in child.findall('lane'):
                    lane_info = {
                     'speed': lane.attrib.get('speed'),
                     'length': lane.attrib.get('length')
                }
                length = child.attrib.get('length')
                if length == None:
                    length = lane_info['length']
                speed = lane_info['speed']
                try:
                    weight = float(length) / float(speed)
                except:
                    weight = 1
                print(f"Length: {length}, Speed: {speed}, Weight: {weight}")
                edge_info = {
                    'id': child.attrib.get('id'),
                    'from': child.attrib.get('from'),
                    'to': child.attrib.get('to'),
                    'weight': weight
                }
                edges.append(edge_info)
    #print("\nEdges:", edges)
    #print("\nJunctions:", junctions)
    for junctions_info in junctions:
        junction_element = ET.SubElement(graph_root, "node", id=junctions_info['id'])
        data_element = ET.SubElement(junction_element, "data", key="key0")
        data_element.text = f"{junctions_info['x']},{junctions_info['y']}"
    for edge_info in edges:
        edge_element = ET.SubElement(graph_root, "edge", id=edge_info['id'], source=edge_info['from'], target=edge_info['to'])
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