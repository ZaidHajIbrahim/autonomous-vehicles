import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt
 
def parse_graph(xml_file):
    tree = ET.parse(xml_file)
    root = tree.getroot()
 
    nodes = {}
    edges = []
 
    for node in root.findall(".//node"):
        node_id = node.get('id')
        position = node.find(".//data[@key='key0']").text
        x, y = map(float, position.split(','))
        nodes[node_id] = (x, y)
 
    for edge in root.findall(".//edge"):
        source = edge.get('source')
        target = edge.get('target')
        edges.append((source, target))
 
    return nodes, edges
 
file_name = 'output1.xml'
 
if __name__ == "__main__":
    def plot_graph(nodes, edges):
        fig, ax = plt.subplots()
 
        for node_id, (x, y) in nodes.items():
            ax.plot(x, y, 'o')
 
        for source, target in edges:
            x_values = [nodes[source][0], nodes[target][0]]
            y_values = [nodes[source][1], nodes[target][1]]
            ax.plot(x_values, y_values, 'k-')
 
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.title('Graph Visualization')
        plt.show()
 
    if __name__ == "__main__":
        nodes, edges = parse_graph(file_name)
        plot_graph(nodes, edges)