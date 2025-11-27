import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt

def parse_graph(xml_file):
    tree = ET.parse(xml_file)
    root = tree.getroot()

    namespace = {'gml': 'http://graphml.graphdrawing.org/xmlns'}

    nodes = {}
    edges = []

    for node in root.findall(".//gml:node", namespace):
        node_id = node.get('id')
        position = node.find(".//gml:data[@key='key0']", namespace).text
        x, y = map(float, position.split(','))
        nodes[node_id] = (x, y)

    for edge in root.findall(".//gml:edge", namespace):
        source = edge.get('source')
        target = edge.get('target')
        sumo_id = edge.get('sumo_id')
        edges.append((source, target, sumo_id))

    return nodes, edges

file_name = 'output.xml'

if __name__ == "__main__":
    def plot_graph(nodes, edges):
        fig, ax = plt.subplots()

        for node_id, (x, y) in nodes.items():
            ax.plot(x, y, 'o')
            ax.text(x, y, node_id, fontsize=12, ha='right')


        for source, target, sumo_id in edges:
            x_values = [nodes[source][0], nodes[target][0]]
            y_values = [nodes[source][1], nodes[target][1]]
            color = 'red' if sumo_id == "NIL" else 'black'
            ax.plot(x_values, y_values, color=color,)


        plt.xlabel('X')
        plt.ylabel('Y')
        plt.title('Graph Visualization')
        plt.show()

    if __name__ == "__main__":
        nodes, edges = parse_graph(file_name)
        plot_graph(nodes, edges)